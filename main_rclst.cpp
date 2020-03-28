#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <thread>

#include "realty_rclst.h"

using namespace std;

constexpr int MAX_CLUSTERS = 100000;

int main(int argc, const char **argv)
{
    if (argc < 3)
    {
        cout << "My error: there must be al least 2 parametrs: <ClusterCount> <FileName>" << endl;
        exit(0);
    }

    const int ClusterCount = atoi(argv[1]);
    const string fn = argv[2];

    if (fn.empty() || ClusterCount < 1 || ClusterCount > MAX_CLUSTERS)
    {
        cout << "My error: something's wrong with arguments" << endl;
        exit(0);
    }

    double g = 0.1;
    double c = 1;
    double d = 2;

    if (argc >= 6)
    {
        sscanf(argv[3], "%lf", &g);
        sscanf(argv[4], "%lf", &c);
        sscanf(argv[5], "%lf", &d);
    }

    for(int i = 3; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "d") == 0 ||
            strcmp(argv[i], "-D") == 0 || strcmp(argv[i], "D") == 0)
            IsDebugOutput = true;

        if (strcmp(argv[i], "-cv") == 0 || strcmp(argv[i], "cv") == 0 ||
            strcmp(argv[i], "-CV") == 0 || strcmp(argv[i], "CV") == 0)
            IsCrossValidation = true;
    }

    MY_DEBUG_ONLY(cout << "g = " << g << endl;)
    MY_DEBUG_ONLY(cout << "c = " << c << endl;)
    MY_DEBUG_ONLY(cout << "d = " << d << endl;)

    #if (defined WIN32) || (defined WIN64)
        cout << MY_P_FUNC << endl;                  // for debugging
    #else
        //
    #endif

    MY_DEBUG_ONLY(cout << "Homework realty crlst (DEBUG detected)" << endl;)

    TaskTrain task;

    task.LoadDataFromInputStream();

    task.TrainAndSave(ClusterCount, fn, g, c, d);

    return 0;
}

