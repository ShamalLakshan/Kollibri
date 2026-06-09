#include "../../include/profiler/console_writer.h"

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
                << std::setw(15) << "Thread ID"  << "\n";
    std::cout << std::string(75, '-') << "\n";
}

void ConsoleWriter::WriteEntry(std::map<std::thread::id, std::vector<Event>> AggregatedData, bool WriteReport)
{
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
                std::cout << std::string(75, '-') << "\n"; // Clean full-width separator

            // Create a single indented string for the Depth column
            std::string depthStr = std::string(event.depth * 2, ' ') + std::to_string(event.depth + 1);

            // Convert Thread ID to string to format it properly
            std::stringstream ss;
            ss << threadId;
            std::string threadIdStr = ss.str();

            // Print aligned columns (Left-align text, Right-align numbers)
            std::cout << std::left 
                        << std::setw(10) << depthStr
                        << std::setw(30) << event.name
                        << std::right
                        << std::setw(20) << event.duration.count()
                        << std::setw(15) << threadIdStr << "\n";

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
    std::cout << "\n=== Aggregation Report ===\n";
}

void ConsoleWriter::WriteFooter()
{
}
