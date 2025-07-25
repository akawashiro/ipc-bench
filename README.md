# IPC benchmark

## How to build
```bash
$ ./scripts/build.sh
```

## How to run
```bash
$ ./build/ipc-bench/bandwidth --type=all --data_size=$((1 << 30)) --num_iterations=10 --num_warmups=3
...
memcpy: 16.7302 GiByte/sec
memcpy_mt (1 threads): 16.5431 GiByte/sec
memcpy_mt (2 threads): 17.3747 GiByte/sec
memcpy_mt (3 threads): 18.4483 GiByte/sec
memcpy_mt (4 threads): 17.9042 GiByte/sec
tcp: 5.5369 GiByte/sec
uds: 6.15441 GiByte/sec
pipe: 2.09526 GiByte/sec
fifo: 2.06931 GiByte/sec
mq: 1.77624 GiByte/sec
mmap: 10.5466 GiByte/sec
shm: 10.5029 GiByte/sec
```

```
$ ./build/ipc-bench/latency --type=all
atomic: 39.1009 ns
condition_variable: 3885.55 ns
statfs: 1247.57 ns
fstatfs: 830.662 ns
getpid: 121.154 ns
```

## Machine information which was used to run the benchmark below

```
$ sudo lshw -class memory
  *-firmware
       description: BIOS
       vendor: American Megatrends International, LLC.
       physical id: 0
       version: P2.10
       date: 08/04/2021
       size: 64KiB
       capacity: 16MiB
       capabilities: pci upgrade shadowing cdboot bootselect socketedrom edd int13floppynec int13floppytoshiba int13floppy360 int13floppy1200 int13floppy720 int13floppy2880 int5printscreen int9keyboard int14serial int17printer int10video acpi usb biosbootspecification uefi
  *-memory
       description: System Memory
       physical id: 10
       slot: System board or motherboard
       size: 64GiB
     *-bank:0
          description: [empty]
          product: Unknown
          vendor: Unknown
          physical id: 0
          serial: Unknown
          slot: DIMM 0
     *-bank:1
          description: DIMM DDR4 Synchronous Unbuffered (Unregistered) 3200 MHz (0.3 ns)
          product: CT32G4DFD832A.C16FE
          vendor: Unknown
          physical id: 1
          serial: E60BBC35
          slot: DIMM 1
          size: 32GiB
          width: 64 bits
          clock: 3200MHz (0.3ns)
     *-bank:2
          description: [empty]
          product: Unknown
          vendor: Unknown
          physical id: 2
          serial: Unknown
          slot: DIMM 0
     *-bank:3
          description: DIMM DDR4 Synchronous Unbuffered (Unregistered) 3200 MHz (0.3 ns)
          product: CT32G4DFD832A.C16FE
          vendor: Unknown
          physical id: 3
          serial: E60BC28D
          slot: DIMM 1
          size: 32GiB
          width: 64 bits
          clock: 3200MHz (0.3ns)
  *-cache:0
       description: L1 cache
       physical id: 13
       slot: L1 - Cache
       size: 1MiB
       capacity: 1MiB
       clock: 1GHz (1.0ns)
       capabilities: pipeline-burst internal write-back unified
       configuration: level=1
  *-cache:1
       description: L2 cache
       physical id: 14
       slot: L2 - Cache
       size: 8MiB
       capacity: 8MiB
       clock: 1GHz (1.0ns)
       capabilities: pipeline-burst internal write-back unified
       configuration: level=2
  *-cache:2
       description: L3 cache
       physical id: 15
       slot: L3 - Cache
       size: 64MiB
       capacity: 64MiB
       clock: 1GHz (1.0ns)
       capabilities: pipeline-burst internal write-back unified
       configuration: level=3
```

```
$ lscpu
Architecture:                         x86_64
CPU op-mode(s):                       32-bit, 64-bit
Address sizes:                        48 bits physical, 48 bits virtual
Byte Order:                           Little Endian
CPU(s):                               32
On-line CPU(s) list:                  0-31
Vendor ID:                            AuthenticAMD
Model name:                           AMD Ryzen 9 5950X 16-Core Processor
CPU family:                           25
Model:                                33
Thread(s) per core:                   2
Core(s) per socket:                   16
Socket(s):                            1
Stepping:                             0
Frequency boost:                      enabled
CPU(s) scaling MHz:                   50%
CPU max MHz:                          5086.0000
CPU min MHz:                          550.0000
BogoMIPS:                             6786.93
Flags:                                fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid extd_apicid aperfmperf rapl pni pclmulqdq monitor ssse3 fma cx16 sse4_1 sse4_2 movbe popcnt aes xsave avx f16c rdrand lahf_lm cmp_legacy svm extapic cr8_legacy abm sse4a misalignsse 3dnowprefetch osvw ibs skinit wdt tce topoext perfctr_core perfctr_nb bpext perfctr_llc mwaitx cpb cat_l3 cdp_l3 hw_pstate ssbd mba ibrs ibpb stibp vmmcall fsgsbase bmi1 avx2 smep bmi2 invpcid cqm rdt_a rdseed adx smap clflushopt clwb sha_ni xsaveopt xsavec xgetbv1 xsaves cqm_llc cqm_occup_llc cqm_mbm_total cqm_mbm_local user_shstk clzero irperf xsaveerptr rdpru wbnoinvd arat npt lbrv svm_lock nrip_save tsc_scale vmcb_clean flushbyasid decodeassists pausefilter pfthreshold avic v_vmsave_vmload vgif v_spec_ctrl umip pku ospke vaes vpclmulqdq rdpid overflow_recov succor smca debug_swap
Virtualization:                       AMD-V
L1d cache:                            512 KiB (16 instances)
L1i cache:                            512 KiB (16 instances)
L2 cache:                             8 MiB (16 instances)
L3 cache:                             64 MiB (2 instances)
NUMA node(s):                         1
NUMA node0 CPU(s):                    0-31
Vulnerability Gather data sampling:   Not affected
Vulnerability Ghostwrite:             Not affected
Vulnerability Itlb multihit:          Not affected
Vulnerability L1tf:                   Not affected
Vulnerability Mds:                    Not affected
Vulnerability Meltdown:               Not affected
Vulnerability Mmio stale data:        Not affected
Vulnerability Reg file data sampling: Not affected
Vulnerability Retbleed:               Not affected
Vulnerability Spec rstack overflow:   Mitigation; Safe RET
Vulnerability Spec store bypass:      Mitigation; Speculative Store Bypass disabled via prctl
Vulnerability Spectre v1:             Mitigation; usercopy/swapgs barriers and __user pointer sanitization
Vulnerability Spectre v2:             Mitigation; Retpolines; IBPB conditional; IBRS_FW; STIBP always-on; RSB filling; PBRSB-eIBRS Not affected; BHI Not affected
Vulnerability Srbds:                  Not affected
Vulnerability Tsx async abort:        Not affected
```

```
$ uname -a
Linux masumi 6.14.0-24-generic #24~24.04.3-Ubuntu SMP PREEMPT_DYNAMIC Mon Jul  7 16:39:17 UTC 2 x86_64 x86_64 x86_64 GNU/Linux
```

## Benchmark results using existing tools

### memcpy

```
$ perf bench mem memcpy --size 1GB
# Running 'mem/memcpy' benchmark:
# function 'default' (Default memcpy() provided by glibc)
# Copying 1GB bytes ...

      17.304930 GB/sec
# function 'x86-64-unrolled' (unrolled memcpy() in arch/x86/lib/memcpy_64.S)
# Copying 1GB bytes ...

       9.950447 GB/sec
# function 'x86-64-movsq' (movsq-based memcpy() in arch/x86/lib/memcpy_64.S)
# Copying 1GB bytes ...

      19.133263 GB/sec
```

### TCP

#### localhost

```
$ iperf3 -c localhost --format g -bytes 1g
Connecting to host localhost, port 5201
[  5] local 127.0.0.1 port 59876 connected to 127.0.0.1 port 5201
[ ID] Interval           Transfer     Bitrate         Retr  Cwnd
[  5]   0.00-1.00   sec  4.83 GBytes  4.82 GBytes/sec    0   6.56 MBytes       
[  5]   1.00-2.00   sec  3.09 GBytes  3.10 GBytes/sec    0   6.56 MBytes       
[  5]   2.00-3.00   sec  1.84 GBytes  1.84 GBytes/sec    0   6.56 MBytes       
[  5]   3.00-4.00   sec  1.90 GBytes  1.90 GBytes/sec    0   6.56 MBytes       
[  5]   4.00-5.00   sec  4.30 GBytes  4.30 GBytes/sec    0   6.56 MBytes       
[  5]   5.00-6.00   sec  5.37 GBytes  5.37 GBytes/sec    0   6.56 MBytes       
[  5]   6.00-7.00   sec  5.00 GBytes  5.00 GBytes/sec    0   6.56 MBytes       
[  5]   7.00-8.00   sec  4.23 GBytes  4.23 GBytes/sec    0   6.56 MBytes       
[  5]   8.00-9.00   sec  4.17 GBytes  4.17 GBytes/sec    0   6.56 MBytes       
[  5]   9.00-10.00  sec  5.35 GBytes  5.35 GBytes/sec    0   6.56 MBytes       
- - - - - - - - - - - - - - - - - - - - - - - - -
[ ID] Interval           Transfer     Bitrate         Retr
[  5]   0.00-10.00  sec  51.8 GBytes  5.18 GBytes/sec    0             sender
[  5]   0.00-10.00  sec  51.8 GBytes  5.18 GBytes/sec                  receiver
```
