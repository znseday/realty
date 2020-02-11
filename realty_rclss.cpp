#include <iostream>
#include <cassert>
#include <fstream>
#include <chrono>
#include <thread>

#include "realty_rclss.h"

using namespace std;

bool IsDebugOutput = false;


void TaskAnalyze::LoadSampleMapFromFile()
{
    ifstream i_stream = ifstream(fn+string(".txt"));
    if (!i_stream)
    {
        cout << "My error: the SampleMap file not found" << endl;
        exit(0);
    }

    string line;
    while (getline(i_stream, line))
    {  
        Sample res; int label;
        sscanf(line.c_str(), "%i;%lf;%lf;%lf;%lf;%lf;%lf;%lf", &label,
                     &res[0], &res[1], &res[2], &res[3], &res[4], &res[5], &res[6]);

        sample_map[label].emplace_back(res);

        MY_DEBUG_ONLY( cout << "echo:  " << line << endl; ) // just echo

    }
    //sample_map.rehash(sample_map.size());
}

void TaskAnalyze::AnalyzeStream()
{
    #if (defined WIN32) || (defined WIN64)
        cout << MY_P_FUNC << endl;                  // for debugging
        ifstream i_stream = ifstream("my_own_test.txt");
        if (!i_stream)
        {
            cout << "My error: the input file not found" << endl;
            exit(0);
        }
    #else
        istream &i_stream = cin;
    #endif

    dlib::one_vs_one_decision_function<ovo_trainer,
               dlib::decision_function<poly_kernel>,  // This is the output of the poly_trainer
               dlib::decision_function<rbf_kernel>    // This is the output of the rbf_trainer
           > df3;

    dlib::deserialize(fn+string(".dat")) >> df3;

    string line;
    while (getline(i_stream, line))
    {
        Sample point;
        sscanf(line.c_str(), "%lf;%lf;%lf;%lf;%lf;%lf;%lf",
                     &point[0], &point[1], &point[2], &point[3], &point[4], &point[5], &point[6]);

        MY_DEBUG_ONLY( cout << "echo (original): " << line << endl; ) // just echo
        MY_DEBUG_ONLY( cout << "echo (check): " << point[0] << point[1] << point[2] << point[3] << point[4] << point[5] << point[6] << endl; ) // just echo

        sample_type sample;

        for (int i = 0; i < sample.size(); i++)
            sample(i) = point[i];

        int predictLabel = (int)df3(sample);

        Samples res = sample_map.at(predictLabel);
        sort(res.begin(), res.end(), [&point](const auto &v1, const auto &v2) {
                double r1 = (v1[0]-point[0])*(v1[0]-point[0]) + (v1[1]-point[1])*(v1[1]-point[1]);
                double r2 = (v2[0]-point[0])*(v2[0]-point[0]) + (v2[1]-point[1])*(v2[1]-point[1]);
                return r1 < r2;
            });

        for (const auto &s : res)
        {
            cout << s[0] << ";";
            cout << s[1] << ";";
            cout << (int)s[2] << ";";
            cout << s[3] << ";";
            cout << s[4] << ";";
            cout << s[5] << ";";
            cout << (int)s[6] << endl;
        }
    }
}
