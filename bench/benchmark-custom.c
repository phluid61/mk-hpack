#define _GNU_SOURCE
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sched.h>
#include <string.h>

#define RDTSC(lo,hi) \
	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi))

#define cycles(lo,hi) \
	(((uint64_t)(hi) << 32) | lo)

double cycles_per_ns = 2.0045676; /* default = 2004.5676 MHz */
unsigned int __start_lo, __start_hi, __end_lo, __end_hi;

inline void bench_start() { RDTSC(__start_lo,__start_hi); }

inline uint64_t bench_end() {
	RDTSC(__end_lo, __end_hi);
	return (cycles(__end_lo,__end_hi) - cycles(__start_lo,__start_hi)) / cycles_per_ns;
}

void init_bench() {
	FILE *cpuinfo; char *arg=0; size_t size; char *ptr; int match; double cpms;

	/* Bind to CPU 0 */
	cpu_set_t cpu_mask;
	CPU_SET(1, &cpu_mask);
	sched_setaffinity(0, sizeof(cpu_mask), &cpu_mask);

	/* Get CPU frequency */
	cpuinfo = fopen("/proc/cpuinfo", "rb");
	while (getdelim(&arg, &size, 0x10, cpuinfo) != -1) {
		match = 0;
		ptr = strstr(arg, "cpu MHz");
		if (ptr) {
			match = sscanf(ptr, "cpu MHz : %lf", &cpms);
		}
		free(arg); arg=0;
		if (match > 0 && match != EOF) {
			cycles_per_ns = cpms / 1000.0;
			break;
		}
	}
	fclose(cpuinfo);
}

