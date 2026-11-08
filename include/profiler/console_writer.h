#pragma once
#ifndef CONSOLE_WRITER_H
#define CONSOLE_WRITER_H

#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "../../include/profiler/IWriter.h"

// Statistics for aggregated scopes
struct EventStats {
    std::string name;
    size_t callCount = 0;
    long long totalDurationNs = 0;
    long long minDurationNs = 0;
    long long maxDurationNs = 0;
    long long avgDurationNs = 0;
};

class ConsoleWriter : public IWriter
{
private:
    std::map<std::thread::id, std::vector<Event>> m_data;

public:
    void WriteHeader() override;
    void WriteEntry(std::map<std::thread::id, std::vector<Event>> AggregatedData, bool WriteReport) override;
    void WriteAggregationReport() override;
    void WriteFooter() override;
};

#endif