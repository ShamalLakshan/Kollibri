#pragma once
#ifndef CONSOLE_WRITER_H
#define CONSOLE_WRITER_H

#include <iostream>
#include <iomanip>
#include <chrono>

#include "../../include/profiler/IWriter.h"

class ConsoleWriter : public IWriter
{
public:
    void WriteHeader() override;
    void WriteEntry(const std::map<std::thread::id, std::vector<Event>>& AggregatedData, bool WriteAggregationReport) override;
    void WriteAggregationReport() override;
    void WriteFooter() override;
};

#endif