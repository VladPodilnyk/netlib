#include <benchmark/benchmark.h>
#include "threadpool.h"
#include "netlib/threadpool.h"

class ThreadPoolBenchmark : public ::benchmark::Fixture {};

BENCHMARK_DEFINE_F(ThreadPoolBenchmark, NaivePoolTest)(benchmark::State &st)
{
    refThreadPool::num_of_tasks = st.range(0);

    while (st.KeepRunning()) {
        refThreadPool::ThreadPool tp;
        tp.run();
        st.PauseTiming();
    }
}
BENCHMARK_REGISTER_F(ThreadPoolBenchmark, NaivePoolTest)->Arg(5)->Arg(10)->Arg(25)->Arg(40)->Arg(50);


BENCHMARK_DEFINE_F(ThreadPoolBenchmark, OptimizedPoolTest)(benchmark::State &st)
{
    refThreadPool::num_of_tasks = st.range(0);

    while (st.KeepRunning()) {
        netpod::ThreadPool *tp = new netpod::ThreadPool(8);
        for (size_t i = 0 ; i < refThreadPool::num_of_tasks; ++i)
            tp->push(std::bind([](int order)->void {
                                    int fibonacci_num = refThreadPool::fib(order); 
                                }, i));
    
        delete tp;
        st.PauseTiming();
    }
}
BENCHMARK_REGISTER_F(ThreadPoolBenchmark, OptimizedPoolTest)->Arg(5)->Arg(10)->Arg(25)->Arg(40)->Arg(50);


int main (int argc, char * argv [])
{
    benchmark::Initialize (&argc, argv);
    benchmark::RunSpecifiedBenchmarks ();
}
