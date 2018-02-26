#if defined(DO_BENCHMARK_POINTER) || defined(DO_BENCHMARK_ALL)
#include <benchmark/benchmark.h>
#include <memory>

namespace {
constexpr std::size_t N = 100;
}

uint8_t __attribute__((noinline)) c_pointer_argument(const uint8_t *p) { return *p; }
static void BM_c_pointer_argument(benchmark::State &state) {
    int src = 10;
    uint8_t *p = (uint8_t *)&src;
    for (auto _ : state) {
        auto ret = c_pointer_argument(p);
        benchmark::DoNotOptimize(ret);
    }
}
BENCHMARK(BM_c_pointer_argument);

uint8_t __attribute__((noinline)) shared_pointer_argument_ref(const std::shared_ptr<uint8_t> &p) {
    return *p;
}
static void BM_shared_pointer_argument_ref(benchmark::State &state) {
    auto p = std::make_shared<uint8_t>();
    for (auto _ : state) {
        auto ret = shared_pointer_argument_ref(p);
        benchmark::DoNotOptimize(ret);
    }
}
BENCHMARK(BM_shared_pointer_argument_ref);

uint8_t __attribute__((noinline)) shared_pointer_argument(std::shared_ptr<uint8_t> p) { return *p; }
static void BM_shared_pointer_argument(benchmark::State &state) {
    auto p = std::make_shared<uint8_t>();
    for (auto _ : state) {
        auto ret = shared_pointer_argument(p);
        benchmark::DoNotOptimize(ret);
    }
}
BENCHMARK(BM_shared_pointer_argument);

/*---------------*/
static void BM_c_pointer_access(benchmark::State &state) {
    std::size_t p[N];
    for (auto _ : state) {
        // for (std::size_t i = 0; i < N; i++)
        //    p[i] = i;
        p[0] = 0;
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(BM_c_pointer_access);

static void BM_shared_pointer_access(benchmark::State &state) {
    auto p =
        std::shared_ptr<std::size_t>(new std::size_t[N], [](std::size_t *ptr) { delete[] ptr; });
    for (auto _ : state) {
        // for (std::size_t i = 0; i < N; i++)
        //    (p.get())[i] = i;
        (p.get())[0] = 0;
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(BM_shared_pointer_access);

#endif // BENCHMARK_MEMCPY
