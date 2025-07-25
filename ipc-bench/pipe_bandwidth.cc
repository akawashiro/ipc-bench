#include "pipe_bandwidth.h"

#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <vector>

#include "absl/log/log.h"

#include "barrier.h"
#include "common.h"

namespace {

const std::string BARRIER_ID = "/pipe_benchmark";

void SendProcess(int write_fd, int num_warmups, int num_iterations,
                 uint64_t data_size, uint64_t buffer_size) {
  SenseReversingBarrier barrier(2, BARRIER_ID);

  std::vector<uint8_t> data_to_send = GenerateDataToSend(data_size);
  std::vector<double> durations;

  for (int iteration = 0; iteration < num_warmups + num_iterations;
       ++iteration) {
    bool is_warmup = iteration < num_warmups;

    if (is_warmup) {
      VLOG(1) << SendPrefix(iteration) << "Warm-up " << iteration << "/"
              << num_warmups;
    } else {
      VLOG(1) << SendPrefix(iteration) << "Starting iteration " << iteration
              << "/" << num_iterations;
    }

    barrier.Wait();
    size_t total_sent = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    while (total_sent < data_size) {
      size_t bytes_to_send = std::min(buffer_size, data_size - total_sent);
      ssize_t bytes_written =
          write(write_fd, data_to_send.data() + total_sent, bytes_to_send);
      if (bytes_written == -1) {
        LOG(FATAL) << "send: write: " << strerror(errno);
      }
      total_sent += bytes_written;
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    if (!is_warmup) {
      std::chrono::duration<double> elapsed_time = end_time - start_time;
      durations.push_back(elapsed_time.count());
      VLOG(1) << SendPrefix(iteration)
              << "Time taken: " << elapsed_time.count() * 1000 << " ms.";
    }
  }

  double bandwidth = CalculateBandwidth(durations, num_iterations, data_size);

  double bandwidth_gibps = bandwidth / (1024.0 * 1024.0 * 1024.0);
  LOG(INFO) << "Send bandwidth: " << bandwidth_gibps << GIBYTE_PER_SEC_UNIT
            << ".";

  close(write_fd);
  VLOG(1) << SendPrefix(-1) << "Exiting.";
}

double ReceiveProcess(int read_fd, int num_warmups, int num_iterations,
                      uint64_t data_size, uint64_t buffer_size) {
  SenseReversingBarrier barrier(2, BARRIER_ID);

  std::vector<double> durations;

  for (int iteration = 0; iteration < num_warmups + num_iterations;
       ++iteration) {
    bool is_warmup = iteration < num_warmups;

    if (is_warmup) {
      VLOG(1) << ReceivePrefix(iteration) << "Warm-up " << iteration << "/"
              << num_warmups;
    } else {
      VLOG(1) << ReceivePrefix(iteration) << "Starting iteration " << iteration
              << "/" << num_iterations;
    }

    std::vector<uint8_t> recv_buffer(buffer_size);
    std::vector<uint8_t> received_data;
    received_data.reserve(data_size);

    barrier.Wait();
    size_t total_received = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    while (total_received < data_size) {
      ssize_t bytes_read = read(read_fd, recv_buffer.data(), buffer_size);
      if (bytes_read == -1) {
        LOG(FATAL) << "receive: read: " << strerror(errno);
      }
      if (bytes_read == 0) {
        if (!is_warmup) {
          VLOG(1) << ReceivePrefix(iteration)
                  << "Sender closed the pipe prematurely.";
        }
        break;
      }
      total_received += bytes_read;
      received_data.insert(received_data.end(), recv_buffer.begin(),
                           recv_buffer.begin() + bytes_read);
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    if (!is_warmup) {
      std::chrono::duration<double> elapsed_time = end_time - start_time;
      durations.push_back(elapsed_time.count());

      VLOG(1) << ReceivePrefix(iteration)
              << "Time taken: " << elapsed_time.count() * 1000 << " ms.";
    }

    if (!VerifyDataReceived(received_data, data_size)) {
      LOG(FATAL) << ReceivePrefix(iteration) << "Data verification failed!";
    } else {
      VLOG(1) << ReceivePrefix(iteration) << "Data verification passed.";
    }
  }

  double bandwidth = CalculateBandwidth(durations, num_iterations, data_size);
  LOG(INFO) << "Receive bandwidth: " << bandwidth / (1 << 30)
            << GIBYTE_PER_SEC_UNIT << ".";

  close(read_fd);
  VLOG(1) << ReceivePrefix(-1) << "Exiting.";

  return bandwidth;
}

} // namespace

double RunPipeBenchmark(int num_iterations, int num_warmups, uint64_t data_size,
                        uint64_t buffer_size) {
  int pipe_fds[2];
  if (pipe(pipe_fds) == -1) {
    LOG(FATAL) << "pipe: " << strerror(errno);
  }

  int read_fd = pipe_fds[0];
  int write_fd = pipe_fds[1];

  pid_t pid = fork();

  if (pid == -1) {
    LOG(FATAL) << "fork: " << strerror(errno);
  }

  if (pid == 0) {
    close(read_fd);
    SendProcess(write_fd, num_warmups, num_iterations, data_size, buffer_size);
    exit(0);
  } else {
    close(write_fd);
    double bandwidth = ReceiveProcess(read_fd, num_warmups, num_iterations,
                                      data_size, buffer_size);
    waitpid(pid, nullptr, 0);
    return bandwidth;
  }
}
