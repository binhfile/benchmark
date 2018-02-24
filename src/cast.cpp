#if defined(DO_BENCHMARK_CAST) || defined(DO_BENCHMARK_ALL)
#include <benchmark/benchmark.h>
#include <memory>

namespace {
struct A {
    virtual void f(int &cnt) { ++cnt; }
};
struct B {
    virtual void g(int &cnt) { ++cnt; }
};
struct C : A, B {
    virtual void h(int &cnt) { ++cnt; }
};
} // namespace

static void BM_cast_c(benchmark::State &state) {
    int src = 10;
    for (auto _ : state) {
        uint8_t *p = (uint8_t *)&src;
        *p = *p + 1;
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(BM_cast_c);

static void BM_static_cast(benchmark::State &state) {
    int src = 10;
    for (auto _ : state) {
        uint8_t *p = static_cast<uint8_t *>(static_cast<void *>(&src));
        *p = *p + 1;
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(BM_static_cast);

static void BM_pointer_call(benchmark::State &state) {
    int cnt = 0;
    C c;
    for (auto _ : state) {
        c.f(cnt);
        benchmark::DoNotOptimize(cnt);
    }
}
BENCHMARK(BM_pointer_call);

static void BM_dynamic_cast_call(benchmark::State &state) {
    int cnt = 0;
    C c;
    for (auto _ : state) {
        A *a = dynamic_cast<A *>(&c);
        a->f(cnt);
        benchmark::DoNotOptimize(cnt);
    }
}
BENCHMARK(BM_dynamic_cast_call);

static void BM_smart_pointer_call(benchmark::State &state) {
    int cnt = 0;
    auto c = std::make_shared<C>();
    for (auto _ : state) {
        c->f(cnt);
        benchmark::DoNotOptimize(cnt);
    }
}
BENCHMARK(BM_smart_pointer_call);

static void BM_static_pointer_cast_call(benchmark::State &state) {
    int cnt = 0;
    auto c = std::make_shared<C>();
    for (auto _ : state) {
        auto a = std::static_pointer_cast<A>(c);
        a->f(cnt);
        benchmark::DoNotOptimize(cnt);
    }
}
BENCHMARK(BM_static_pointer_cast_call);

static void BM_dynamic_pointer_cast_call(benchmark::State &state) {
    int cnt = 0;
    auto c = std::make_shared<C>();
    for (auto _ : state) {
        auto a = std::dynamic_pointer_cast<A>(c);
        a->f(cnt);
        benchmark::DoNotOptimize(cnt);
    }
}
BENCHMARK(BM_dynamic_pointer_cast_call);

#endif // BENCHMARK_MEMCPY
