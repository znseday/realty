#pragma once

#include <iostream>
#include <fstream>

#include <iterator>
#include <algorithm>
#include <memory>

#include <iostream>

#include "common.h"

//#if ((defined WIN32) || (defined WIN64))
//#include "C:/dlib-19.19/dlib-19.19/dlib/clustering.h"
//#else
//    #include <dlib/clustering.h>
//    #include <dlib/rand.h>
//#endif

extern bool IsCrossValidation;

class TaskTrain
{
private:
    Samples data;
    std::vector<double> labels;

public:
    TaskTrain() = default;

    static Sample DataStringToSample(std::string line);

    static std::string ConvrtDataLabelsToString(const Sample &sample, double label);

    void LoadDataFromInputStream();

    void TrainAndSave(int ClusterCount, const std::string &fn, double g, double c, double d);
};



