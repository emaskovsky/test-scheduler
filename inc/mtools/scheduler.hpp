

#ifndef MTOOLS_SCHEDULER_INCLUDE_GUARD
#define MTOOLS_SCHEDULER_INCLUDE_GUARD


#include <cstdlib>
#include <vector>


namespace mtools
{


class Scheduler
{

public:

    struct Job
    {

    public:

        Job(const size_t size, const size_t time)
            : m_size(size)
            , m_timeRequired(time)
            , m_timeWaiting(0)
        {}

        size_t m_size;
        size_t m_timeRequired;
        size_t m_timeWaiting;
    };

public:

    Scheduler();

    virtual ~Scheduler();

    virtual void queue_resource(const size_t resource) = 0;

    virtual void queue_job(const Job & job) = 0;

    virtual bool idle() const = 0;

    void tick();

    size_t getLastBusy() const;

    double getAverageLatency() const;

protected:

    virtual void onTick() = 0;

    // the running jobs
    std::vector<Job> m_running;

    size_t m_ticks;

    // last busy tick
    size_t m_lastBusy;

    class LatencyCounter
    {

    public:

        LatencyCounter()
            : m_total(0)
            , m_jobs(0)
        {}

        bool add(const size_t latency)
        {
            const size_t total = m_total + latency;
            if (total < m_total) {
                // overflow
                return false;
            }
            m_total = total;
            ++m_jobs;
            return true;
        }

        double average() const {
            if (!m_jobs) {
                return 0.0;
            } else {
                return static_cast<double>(m_total) / m_jobs;
            }
        }

    private:

        // total latency
        size_t m_total;

        // count of jobs taken into account
        size_t m_jobs;

    };

    LatencyCounter m_latencyCounter;

};


} // namespace mtools


inline mtools::Scheduler::Scheduler()
    : m_running()
    , m_ticks(0)
    , m_lastBusy(0)
{}


inline mtools::Scheduler::~Scheduler()
{}


inline size_t mtools::Scheduler::getLastBusy() const
{
    return m_lastBusy;
}


inline double mtools::Scheduler::getAverageLatency() const
{
    return m_latencyCounter.average();
}


#endif // MTOOLS_SCHEDULER_INCLUDE_GUARD
