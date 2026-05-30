#include "../../include/profiler/console_writer.h"

void ConsoleWriter::WriteHeader()
{
    std::cout << "\n======== Kollibri ========\n";
    std::cout << "\n==== Profiler Report ====\n";

    std::cout << std::setw(20) << "Thread ID" 
            << std::setw(25) << "Event Name" 
            << std::setw(20) << "Duration (ns)"
            << std::setw(5) << "Depth"  << "\n";
    std::cout << std::string(75, '-') << "\n";
}


void ConsoleWriter::WriteEntry(const std::map<std::thread::id, std::vector<Event>>& AggregatedData, bool WriteAggregationReport)
{
    int noOfEvents = 0;
    std::chrono::nanoseconds totalMeasuredTime{0};

    for (auto& pair : AggregatedData) 
    {
        auto& threadId = pair.first;
        auto& events = pair.second;
        for (auto& event : events)
        {
            std::cout << std::setw(20) << threadId
            << std::setw(25) << event.name
            << std::setw(20) << event.duration.count()
            << std::setw(5) << event.depth + 1<< "\n";
            noOfEvents++;
            totalMeasuredTime += event.duration;
        }
    }
    if(WriteAggregationReport)
    {
        this->WriteAggregationReport();
        std::cout <<  "No of measurements: " << noOfEvents;
        std::cout <<  "\nTotal measured time: " << totalMeasuredTime.count() << "ns \n";
    }
}

void ConsoleWriter::WriteAggregationReport()
{
    std::cout << "\n=== Aggregation Report ===\n";
}

void ConsoleWriter::WriteFooter()
{
}
