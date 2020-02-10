#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <thread>

#include "realty_rclss.h"

using namespace std;


int main(int argc, const char **argv)
{
    if (argc < 2)
    {
        cout << "My error: there must be 1 parametrs: <ModelFileName>" << endl;
        exit(0);
    }

    const string fn = argv[1];

    if (fn.empty())
    {
        cout << "My error: something's wrong with arguments" << endl;
        exit(0);
    }

    for(int i = 2; i < argc; i++)
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "d") == 0)
            IsDebugOutput = true;

    #if (defined WIN32) || (defined WIN64)
        cout << MY_P_FUNC << endl;                  // for debugging
    #else
        //
    #endif

    MY_DEBUG_ONLY(cout << "Homework realty crlss (DEBUG detected)" << endl;)


    TaskAnalyze task(fn);

    task.LoadSampleMapFromFile();

    task.AnalyzeStream();

    return 0;
}

