#include <core/scheduling/process.hpp>

namespace legion::core::scheduling
{
    Process::Process(const std::string& name, id_type nameHash, time::span interval, size_type maxIterations) : m_name(name), m_nameHash(nameHash)
    {
        setInterval(interval, maxIterations);
    }

    id_type Process::id() const noexcept
    {
        return m_nameHash;
    }

    bool Process::inUse() const noexcept
    {
        return m_hooks.size();
    }

    void Process::setOperation(delegate<void(time::time_span<fast_time>)>&& operation) noexcept
    {
        m_operation = operation;
    }

    void Process::setInterval(time::span interval, size_type maxIterations) noexcept
    {
        m_fixedTimeStep = interval != time::span::zero();
        m_interval = interval;
        m_maxIterations = maxIterations;
    }

    std::unordered_set<id_type>& Process::hooks() noexcept
    {
        return m_hooks;
    }

    void Process::execute(time::span deltaTime)
    {
        if (!m_fixedTimeStep)
        {
            m_operation(deltaTime);
        }
        else
        {
            m_timebuffer += deltaTime;

            size_type iterations = 0;
            while (m_timebuffer >= m_interval)
            {
                m_operation(m_interval);
                m_timebuffer -= m_interval;
                iterations++;
                if (iterations >= m_maxIterations)
                    break;
            }
        }
    }
}
