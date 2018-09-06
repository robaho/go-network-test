**About**

Similar to the go-concurrency-test project, this project aims to test the performance of the Go networking layer.

Since Go compiles to native code, I would expect similar performance to C.

**Testing Methodology**

The testing process involves creating a server thread/routine, which listens for incoming UDP packets and then echos them back to the sender. 
The timing period is from prior to the client send, to after the client receive of the echo'd packet. All traffic is on localhost, so no
physical transmission occurs. Only UDP is tested, but it is expected to see similar performance with TCP, as it is the same number of system calls.

The tests use 'go bench' for Go, 'jmh' for Java, and internally timed for C.

The tests were run on the same machine, a iMac with core i7, (4 cores, 8 threads). Go 1.11, Java 1.8_181 and C (LVM 9.1.0)

The LLVM used -O3 compilation (also tested with -O2 with no change in performance).

**Results**

After timing multiple runs, the Go and Java showed a high degree of variance.

* Go, 32-33 usecs
* Java, 33-34 usecs
* C, 29-30 usecs

The Go and Java versions show very similar performance, with each being about 10% slower than the C version.
