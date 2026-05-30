#pragma once
#ifndef IWRITER_H
#define IWRITER_H

#include <thread>
#include <map>
#include <vector>
#include <profiler/Event.h>

class IWriter 
{
public:
    virtual ~IWriter() = default;
    virtual void WriteHeader() = 0;
    virtual void WriteEntry(const std::map<std::thread::id, std::vector<Event>>& AggregatedData, bool WriteAggregationReport) = 0;
    virtual void WriteAggregationReport() = 0; // plan : call from WriteEntry to print (decide data type later)
    virtual void WriteFooter() = 0;
};

#endif