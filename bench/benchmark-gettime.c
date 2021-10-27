#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define BENCH_CLOCK CLOCK_PROCESS_CPUTIME_ID
/*#define BENCH_CLOCK CLOCK_MONOTONIC*/

#define nanoseconds(tm) ((uint64_t)((tm).tv_sec) * UINT64_C(1000000000) + (uint64_t)(tm).tv_nsec)

struct timespec __bench_start, __bench_end;

void bench_start() { clock_gettime(BENCH_CLOCK, &__bench_start); }

uint64_t bench_end() {
	clock_gettime(BENCH_CLOCK, &__bench_end);
	return nanoseconds(__bench_end) - nanoseconds(__bench_start);
}

void init_bench() {
#if 0
	unsigned long cpu_mask = 1; /* bind to cpu 0 */
	sched_setaffinity(0, sizeof(cpu_mask), &cpu_mask);
#endif
}

