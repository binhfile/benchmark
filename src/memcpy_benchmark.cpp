#if defined(DO_BENCHMARK_MEMCPY) || defined(DO_BENCHMARK_ALL)
#include <benchmark/benchmark.h>

#define DEFINE_BM(name) BENCHMARK(BM_##name)->RangeMultiplier(2)->Range(8, 8 << 10);

#include <cstring>
static void BM_memcpy(benchmark::State &state) {
    char *src = new char[state.range(0)];
    char *dst = new char[state.range(0)];
    memset(src, 'x', state.range(0));
    for (auto _ : state)
        memcpy(dst, src, state.range(0));
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
    delete[] src;
    delete[] dst;
}
DEFINE_BM(memcpy)

#include <cstdint>
static void memcpy_c(void *dst, const void *src, std::size_t size) {
    auto dst_p = static_cast<uint8_t *>(static_cast<void *>(dst));
    auto src_p = static_cast<const uint8_t *>(static_cast<const void *>(src));
    for (std::size_t i = 0; i < size; i++) {
        dst_p[i] = src_p[i];
    }
}
static void BM_memcpy_c(benchmark::State &state) {
    char *src = new char[state.range(0)];
    char *dst = new char[state.range(0)];
    memset(src, 'x', state.range(0));
    for (auto _ : state)
        memcpy_c(dst, src, state.range(0));
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
    delete[] src;
    delete[] dst;
}
DEFINE_BM(memcpy_c)

#endif // BENCHMARK_MEMCPY
