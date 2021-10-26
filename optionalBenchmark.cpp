#include <benchmark/benchmark.h>
#include <optional>
#include <string>
#include "markable.hpp"
#include <random>

using namespace ak_toolkit;

using TNum = int64_t;

const TNum MAGIC = 7;
const TNum ITERS = 10'000;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(1, 1000);

typedef markable<mark_int<TNum, std::numeric_limits<TNum>::min()>> opt_compact_int;

TNum get_int(TNum n) {
    return n & 0xFF;
}

TNum get_rand_int() {
    return dist(rng);
}

TNum sum_simple() {
    TNum sum = 0;
    for (TNum i = 0; i < ITERS; ++i) {
        const auto n = get_int(i);
        if (n != MAGIC)
            sum += n;
    }
    return sum;
}

TNum sum_rand_simple() {
	TNum sum = 0;
	for (TNum i = 0; i < ITERS; ++i) {
		const auto n = get_rand_int();
		if (n != MAGIC)
			sum += n;
	}
	return sum;
}

static void SumSimple(benchmark::State& state) {
    // Code inside this loop is measured repeatedly
    for (auto _ : state) {
        auto s = sum_simple();
        benchmark::DoNotOptimize(s);
    }
}

static void SumRandSimple(benchmark::State& state) {
	// Code inside this loop is measured repeatedly
	for (auto _ : state) {
		auto s = sum_rand_simple();
		benchmark::DoNotOptimize(s);
	}
}

// Register the function as a benchmark
BENCHMARK(SumSimple);
BENCHMARK(SumRandSimple);

std::optional<TNum> get_optional_int(TNum n) {
    const auto i = n & 0xFF;
    return i != MAGIC ? std::optional<TNum>{i} : std::nullopt;
}

std::optional<TNum> get_rand_optional_int() {
	const auto i = get_rand_int();
	return i != MAGIC ? std::optional<TNum>{i} : std::nullopt;
}

TNum sum_optional() {
    TNum sum = 0;
    for (TNum i = 0; i < ITERS; ++i) {
        const auto n = get_optional_int(i);
        if (n)
            sum += n.value();
    }
    return sum;
}

TNum sum_rand_optional() {
	TNum sum = 0;
	for (TNum i = 0; i < ITERS; ++i) {
		const auto n = get_rand_optional_int();
		if (n)
			sum += n.value();
	}
	return sum;
}

static void SumOptional(benchmark::State& state) {
    // Code inside this loop is measured repeatedly
    for (auto _ : state) {
        auto s = sum_optional();
        benchmark::DoNotOptimize(s);
    }
}

static void SumRandOptional(benchmark::State& state) {
	// Code inside this loop is measured repeatedly
	for (auto _ : state) {
		auto s = sum_rand_optional();
		benchmark::DoNotOptimize(s);
	}
}

// Register the function as a benchmark
BENCHMARK(SumOptional);
BENCHMARK(SumRandOptional);

opt_compact_int get_compact_optional_int(TNum n) {
    const auto i = n & 0xFF;
    return i != MAGIC ? opt_compact_int{ i } : opt_compact_int{};
}

TNum sum_compact_optional() {
    TNum sum = 0;
    for (TNum i = 0; i < ITERS; ++i) {
        const auto n = get_compact_optional_int(i);
        if (n.has_value())
            sum += n.value();
    }
    return sum;
}

static void SumCompactOptional(benchmark::State& state) {
    // Code inside this loop is measured repeatedly
    for (auto _ : state) {
        auto s = sum_compact_optional();
        benchmark::DoNotOptimize(s);
    }
}
// Register the function as a benchmark
//BENCHMARK(SumCompactOptional);

static void CreateOptional(benchmark::State& state) {
    // Code inside this loop is measured repeatedly
    for (auto _ : state) {
        std::optional<std::string> opt;
        opt = "Hello World";
        benchmark::DoNotOptimize(opt);
    }
}
// Register the function as a benchmark
 //BENCHMARK(CreateOptional);

struct string_marked_value                           // a policy which defines the representaioion of the
    : ak_toolkit::markable_type<std::string>           // 'marked' (special) std::string value
{
    static std::string marked_value() {                // create the marked value
        return std::string("\0\0", 2);
    }
    static bool is_marked_value(const std::string& v) { // test if a given value is considered marked
        return v.compare(0, v.npos, "\0\0", 2) == 0;
    }
};

typedef ak_toolkit::markable<string_marked_value> opt_compact_str;


static void CreateCompactOptional(benchmark::State& state) {
    // Code inside this loop is measured repeatedly
    for (auto _ : state) {
        opt_compact_str opt("Hello World");
        //opt.assign("Hello World");
        benchmark::DoNotOptimize(opt);
    }
}
// Register the function as a benchmark
//BENCHMARK(CreateCompactOptional);

static void CreateString(benchmark::State& state) {
    // Code inside this loop is measured repeatedly
    for (auto _ : state) {
        std::string str;
        str = "Hello World";
        benchmark::DoNotOptimize(str);
    }
}
// Register the function as a benchmark
//BENCHMARK(CreateString);

