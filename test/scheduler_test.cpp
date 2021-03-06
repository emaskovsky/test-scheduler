
#include <mtools/fifo_scheduler.hpp>
#include <mtools/optimized_scheduler.hpp>


#include <cstdlib>
#include <cassert>

#include <memory>
#include <iostream>
#include <iomanip>


namespace mtools
{


class SchedulerTester
{

public:

    class Resources
    {

    public:

        Resources(
            FifoScheduler * const fifoScheduler,
            OptimizedScheduler * const optimizedScheduler)
            : m_fifoScheduler(fifoScheduler)
            , m_optimizedScheduler(optimizedScheduler)
        {
            assert(fifoScheduler != NULL);
            assert(optimizedScheduler != NULL);
        }

        Resources & operator << (const size_t resource)
        {
            std::cout << "A resource unit is available on compute node #"
                << resource << std::endl;
            assert(m_fifoScheduler != NULL);
            assert(m_optimizedScheduler != NULL);
            m_fifoScheduler->queue_resource(resource);
            m_optimizedScheduler->queue_resource(resource);
            return *this;
        }

    private:

        FifoScheduler * m_fifoScheduler;
        OptimizedScheduler * m_optimizedScheduler;

    };

    class Jobs
    {

    public:

        Jobs(
            FifoScheduler * const fifoScheduler,
            OptimizedScheduler * const optimizedScheduler)
            : m_fifoScheduler(fifoScheduler)
            , m_optimizedScheduler(optimizedScheduler)
        {
            assert(fifoScheduler != NULL);
            assert(optimizedScheduler != NULL);
        }

        Jobs & operator << (const Scheduler::Job & job)
        {
            std::cout << "Added job of size: "
                << job.m_size << std::endl;
            assert(m_fifoScheduler != NULL);
            assert(m_optimizedScheduler != NULL);
            m_fifoScheduler->queue_job(job);
            m_optimizedScheduler->queue_job(job);
            return *this;
        }

    private:

        FifoScheduler * m_fifoScheduler;
        OptimizedScheduler * m_optimizedScheduler;

    };

public:

    SchedulerTester()
        : m_fifoScheduler(new FifoScheduler)
        , m_optimizedScheduler(new OptimizedScheduler())
        , m_resources(m_fifoScheduler.get(), m_optimizedScheduler.get())
        , m_jobs(m_fifoScheduler.get(), m_optimizedScheduler.get())
    {}

    ~SchedulerTester()
    {
        finish();
    }

public:

    Resources & resources()
    {
        return m_resources;
    }

    Jobs & jobs()
    {
        return m_jobs;
    }

    void tick(const size_t ticks = 1)
    {
        std::cout << "... tick ..." << std::endl;
        for (size_t i = 0; i < ticks; ++i) {
            m_fifoScheduler->tick();
            m_optimizedScheduler->tick();
        }
    }

    void finish()
    {
        while (!m_fifoScheduler->idle() || !m_optimizedScheduler->idle()) {
            tick();
        }
        std::cout << std::endl;
        std::cout << "[FIFO Scheduler] Average latency: "
            << std::fixed << std::setw( 5 ) << std::setprecision( 3 )
            << m_fifoScheduler->getAverageLatency() << std::endl;
        std::cout << "[OPTIMIZING Scheduler] Average latency: "
            << std::fixed << std::setw( 5 ) << std::setprecision( 3 )
            << m_optimizedScheduler->getAverageLatency() << std::endl;
    }

private:

    std::auto_ptr< FifoScheduler > m_fifoScheduler;
    std::auto_ptr< OptimizedScheduler > m_optimizedScheduler;

    Resources m_resources;
    Jobs m_jobs;

};


} // namespace mtools


namespace
{


void test1()
{
    using mtools::Scheduler;

    mtools::SchedulerTester tester;

    tester.tick();
    tester.resources() << 2;
    tester.tick();
    tester.jobs() << Scheduler::Job(3, 5) << Scheduler::Job(1, 2) << Scheduler::Job(1, 3);
    tester.tick(2);
    tester.resources() << 7;
    tester.tick(3);
    tester.jobs() << Scheduler::Job(4, 5) << Scheduler::Job(5, 2);
    tester.resources() << 6 << 2 << 8 << 1;
    tester.tick(2);
    tester.jobs() << Scheduler::Job(8, 10);
    tester.resources() << 2 << 8 << 3;
    tester.tick(2);
    tester.jobs() << Scheduler::Job(2, 3);
    tester.resources() << 2 << 8 << 4 << 5 << 9 << 0 << 4 << 5;
    tester.tick(2);
    tester.resources() << 3 << 5;
    tester.tick(2);
    tester.resources() << 2 << 8;
    tester.tick(2);
    tester.resources() << 4 << 5 << 9;
    tester.tick(2);
    tester.resources() << 2 << 8 << 4 << 5 << 9 << 0 << 4 << 5 << 2;
}


void test2()
{
    using mtools::Scheduler;

    mtools::SchedulerTester tester;

    tester.resources() << 2 << 7;
    tester.tick();
    tester.jobs() << Scheduler::Job(3, 5) << Scheduler::Job(1, 2);
    tester.tick(2);
    tester.jobs() << Scheduler::Job(1, 3);
    tester.tick();
    tester.resources() << 1;
    tester.tick();
    tester.resources() << 8;
    tester.jobs() << Scheduler::Job(4, 5);
    tester.resources() << 6 << 2;
    tester.tick(2);
    tester.jobs() << Scheduler::Job(5, 2);
    tester.tick(2);
    tester.resources() << 2 << 8;
    tester.tick(2);
    tester.resources() << 1 << 8;
    tester.tick(2);
    tester.jobs() << Scheduler::Job(9, 8);
    tester.resources() << 2 << 8 << 4 << 5 << 9 << 0 << 4 << 5 << 2;
    tester.tick(2);
    tester.resources() << 3 << 5 << 3 << 6;
    tester.tick(2);
    tester.resources() << 2 << 8 << 4 << 5 << 9 << 0 << 4 << 5 << 2;
    tester.tick(2);
    tester.jobs() << Scheduler::Job(2, 3);
    tester.resources() << 2 << 8 << 4 << 5 << 9 << 0 << 4 << 5 << 2;
}


} // anonymous namespace


int main()
{
    test1();
    test2();

    return EXIT_SUCCESS;
}
