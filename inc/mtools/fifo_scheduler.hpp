

#ifndef MTOOLS_FIFO_SCHEDULER_INCLUDE_GUARD
#define MTOOLS_FIFO_SCHEDULER_INCLUDE_GUARD


#include <mtools/scheduler.hpp>

#include <queue>
#include <deque>
#include <vector>


namespace mtools
{


class FifoScheduler
    : public Scheduler
{

public:

    virtual void queue_resource(const size_t resource);

    virtual void queue_job(const Job & job);

    virtual bool idle() const;

    virtual void tick();

private:

    // the resources queue
    std::queue<size_t> m_resources;

    // the jobs queue
    std::deque<Job> m_jobs;

    // the running jobs
    std::vector<Job> m_running;

};


} // namespace mtools


#endif // MTOOLS_FIFO_SCHEDULER_INCLUDE_GUARD
