#pragma once

#include <iostream>
#include <fstream>

#include <iterator>
#include <algorithm>
#include <memory>

#include <iostream>

#include "common.h"

#if ((defined WIN32) || (defined WIN64))
#include "C:/dlib-19.19/dlib-19.19/dlib/clustering.h"
#else
    #include <dlib/clustering.h>
    #include <dlib/rand.h>
#endif


class TaskAnalyze
{
private:
//    Samples data;
//    std::vector<double> &labels;

    SampleMap sample_map;

    std::string fn;

public:
    TaskAnalyze() = delete;

    TaskAnalyze(const std::string &_fn) : fn(_fn) {}

    void LoadSampleMapFromFile();

    void AnalyzeStream();
};



