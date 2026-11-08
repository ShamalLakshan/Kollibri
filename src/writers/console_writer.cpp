#include "../../include/profiler/console_writer.h"
#include "../../include/profiler/session.h"
#include <algorithm>

void ConsoleWriter::WriteHeader()
{
    std::cout << "\n======== Kollibri ========\n";
    std::cout << "==== Profiler Report ====\n\n";

    // Column widths match the WriteEntry data row exactly
    std::cout << std::left 
                << std::setw(10) << "Depth" 
                << std::setw(30) << "Event Name" 
                << std::right
                << std::setw(20) << "Duration (ns)"
                << "   "
                << std::left
                << std::setw(20) << "Thread"  << "\n";
    std::cout << std::string(85, '-') << "\n";
}

void ConsoleWriter::WriteEntry(std::map<std::thread::id, std::vector<Event>> AggregatedData, bool WriteReport)
{
    m_data = AggregatedData;

    int noOfEvents = 0;
    std::chrono::nanoseconds totalMeasuredTime{0};

    for (auto pairIt = AggregatedData.rbegin(); pairIt != AggregatedData.rend(); ++pairIt) 
    {
        auto& threadId = pairIt->first;
        auto& events = pairIt->second;
        
        for (auto eventIt = events.rbegin(); eventIt != events.rend(); ++eventIt)
        {
            auto& event = *eventIt;
            
            if (event.depth == 0)
                std::cout << std::string(85, '-') << "\n"; // Clean full-width separator

            // Create a single indented string for the Depth column
            std::string depthStr = std::string(event.depth * 2, ' ') + std::to_string(event.depth + 1);

            // Get thread name if set, otherwise thread id
            std::string threadDisplay = profiler::ProfilerSession::GetThreadName(threadId);

            // Print aligned columns (Left-align text, Right-align numbers)
            std::cout << std::left 
                        << std::setw(10) << depthStr
                        << std::setw(30) << event.name
                        << std::right
                        << std::setw(20) << event.duration.count()
                        << "   "
                        << std::left
                        << std::setw(20) << threadDisplay << "\n";

            noOfEvents++;
            totalMeasuredTime += event.duration;
        }
    }
    if(WriteReport)
    {
        this->WriteAggregationReport();
    }
}

void ConsoleWriter::WriteAggregationReport()
{
    std::cout << "\n=================================== Aggregation Report ===================================\n";
    std::cout << "==== (Grouped by Scope Name, Sorted by Total Time Descending) ====\n\n";

    std::cout << std::left 
              << std::setw(30) << "Event Name" 
              << std::right
              << std::setw(10) << "Calls"
              << std::setw(18) << "Total (ns)"
              << std::setw(18) << "Min (ns)"
              << std::setw(18) << "Max (ns)"
              << std::setw(18) << "Avg (ns)" << "\n";
    std::cout << std::string(112, '-') << "\n";

    // group events by name
    std::map<std::string, EventStats> statsMap;

    for (const auto& pair : m_data)
    {
        const auto& events = pair.second;
        for (const auto& event : events)
        {
            std::string name = event.name;
            long long duration = event.duration.count();

            auto it = statsMap.find(name);
            if (it == statsMap.end())
            {
                EventStats stats;
                stats.name = name;
                stats.callCount = 1;
                stats.totalDurationNs = duration;
                stats.minDurationNs = duration;
                stats.maxDurationNs = duration;
                stats.avgDurationNs = duration;
                statsMap[name] = stats;
            }
            else
            {
                auto& stats = it->second;
                stats.callCount++;
                stats.totalDurationNs += duration;
                if (duration < stats.minDurationNs) stats.minDurationNs = duration;
                if (duration > stats.maxDurationNs) stats.maxDurationNs = duration;
            }
        }
    }

    if (statsMap.empty())
    {
        std::cout << "No profiling events recorded.\n";
        return;
    }

    // calculate averages
    std::vector<EventStats> statsList;
    statsList.reserve(statsMap.size());
    for (auto& pair : statsMap)
    {
        auto& stats = pair.second;
        if (stats.callCount > 0)
        {
            stats.avgDurationNs = stats.totalDurationNs / stats.callCount;
        }
        statsList.push_back(stats);
    }

    // sort by total time descending
    std::sort(statsList.begin(), statsList.end(), [](const EventStats& a, const EventStats& b) {
        return a.totalDurationNs > b.totalDurationNs;
    });

    for (const auto& stats : statsList)
    {
        std::cout << std::left 
                  << std::setw(30) << stats.name
                  << std::right
                  << std::setw(10) << stats.callCount
                  << std::setw(18) << stats.totalDurationNs
                  << std::setw(18) << stats.minDurationNs
                  << std::setw(18) << stats.maxDurationNs
                  << std::setw(18) << stats.avgDurationNs << "\n";
    }

    std::cout << std::string(112, '-') << "\n";
}

void ConsoleWriter::WriteFooter()
{
}
