#pragma once
#ifndef CONSOLE_WRITER_H
#define CONSOLE_WRITER_H

#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>

#include "../../include/profiler/IWriter.h"

class ConsoleWriter : public IWriter
{
public:
    void WriteHeader() override;
    void WriteEntry(std::map<std::thread::id, std::vector<Event>> AggregatedData, bool WriteReport) override;
    void WriteAggregationReport() override;
    void WriteFooter() override;
};

#endif