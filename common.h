#ifndef COMMON_H
#define COMMON_H

#include <array>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

#if ((defined WIN32) || (defined WIN64))
#include "C:/dlib-19.19/dlib-19.19/dlib/clustering.h"
#include "C:/dlib-19.19/dlib-19.19/dlib/svm/krr_trainer_abstract.h"
#include "C:/dlib-19.19/dlib-19.19/dlib/svm_threaded.h"
#include "C:/dlib-19.19/dlib-19.19/dlib/svm/krr_trainer.h"
#else
#include <dlib/svm_threaded.h>
#include <dlib/clustering.h>
#endif


// __FUNCSIG__ is for VS, but Qt (mingw) works with __PRETTY_FUNCTION__
#if ((defined WIN32) || (defined WIN64)) && (defined _MSC_VER)
#define MY_P_FUNC __FUNCSIG__
#else
#define MY_P_FUNC __PRETTY_FUNCTION__
#endif

extern bool IsDebugOutput;

#define MY_DEBUG_ONLY(x) { if(IsDebugOutput) {x} }

using Sample  = std::array<double,7>;
using Samples = std::vector<Sample>;
using SampleMap = std::unordered_map<int, Samples>;


using sample_type = dlib::matrix<double,6,1>;

using ovo_trainer = dlib::one_vs_one_trainer<dlib::any_trainer<sample_type>>;
using poly_kernel = dlib::polynomial_kernel<sample_type>;
using rbf_kernel  = dlib::radial_basis_kernel<sample_type>;



std::string ConvrtDataLabelsToString(const Sample &sample, double label)
{
    std::stringstream ss;
    ss << (int)label << ";";
    ss << sample[0] << ";";
    ss << sample[1] << ";";
    ss << (int)sample[2] << ";";
    ss << sample[3] << ";";
    ss << sample[4] << ";";
    ss << sample[5] << ";";
    ss << (int)sample[6] << ";";
    return ss.str();
}

#endif // COMMON_H
