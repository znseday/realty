#include <iostream>
#include <cassert>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstring>

//#include "common.h"
#include "realty_rclst.h"

using namespace std;

bool IsDebugOutput = false;

Sample TaskTrain::DataStringToSample(std::string line)
{
    Sample res = {0};
    int t1 = -1, t2 = -1;

    for (size_t i = 0; i < line.length(); i++)
    {
        size_t p = line.find(";;", i);
        if (p != string::npos)
        {
            line.insert(p+1, 1, '0');
            i = p + 1;
        }
    }
    sscanf(line.c_str(), "%lf;%lf;%lf;%lf;%lf;%lf;%i;%i",
                 &res[0], &res[1], &res[2], &res[3], &res[4], &res[5], &t1, &t2);



    MY_DEBUG_ONLY( cout << "echo (check full): " << res[0] << "; " << res[1]
                                                              << "; " << res[2]
                                                              << "; " << res[3]
                                                              << "; " << res[4]
                                                              << "; " << res[5]
                                                              << "; " << t1 << "; " << t2 << endl; ) // just echo

    if (t1 == 1 || t1 == t2)
        res[6] = 0;
    else
        res[6] = 1;
    return res;
}

std::string TaskTrain::ConvrtDataLabelsToString(const Sample &sample, double label)
{
    std::stringstream ss;
    ss << (int)label << ";";
    ss << sample[0] << ";";
    ss << sample[1] << ";";
    ss << (int)sample[2] << ";";
    ss << sample[3] << ";";
    ss << sample[4] << ";";
    ss << sample[5] << ";";
    ss << (int)sample[6];
    return ss.str();
}


void TaskTrain::LoadDataFromInputStream()
{
    #if (defined WIN32) || (defined WIN64)
        cout << MY_P_FUNC << endl;                  // for debugging
        ifstream i_stream = ifstream("16.dataset.csv");
        if (!i_stream)
        {
            cout << "My error: the input file not found" << endl;
            exit(0);
        }
    #else
        istream &i_stream = cin;
    #endif

    string line;
    while (getline(i_stream, line))
    {
        Sample sample = DataStringToSample(line);
        data.emplace_back(std::move(sample));

        MY_DEBUG_ONLY( cout << "echo (original): " << line << endl; ) // just echo
        MY_DEBUG_ONLY( cout << "echo (check): " << data.back()[0] << "; " << data.back()[1]
                                                                  << "; " << data.back()[2]
                                                                  << "; " << data.back()[3]
                                                                  << "; " << data.back()[4]
                                                                  << "; " << data.back()[5]
                                                                  << "; " << data.back()[6] << endl; ) // just echo
    }
}

void TaskTrain::TrainAndSave(int ClusterCount, const std::string &fn, double g, double c, double d)
{
    // Возможно, выбрать другой кернел
    //typedef dlib::radial_basis_kernel<sample_type> kernel_type;
    //using  kernel_type = poly_kernel;
    using  kernel_type = rbf_kernel;

//    dlib::kcentroid<kernel_type> kc(kernel_type(g, c, d));
    dlib::kcentroid<kernel_type> kc(kernel_type{g});
    dlib::kkmeans<kernel_type> test(kc);

    vector<sample_type> samples;
    vector<sample_type> initial_centers;

    sample_type m;

    for (auto point : data)
    {
        for (int i = 0; i < m.size(); i++)
            m(i) = point[i];

        samples.push_back(m);
    }

    test.set_number_of_centers(ClusterCount);
    pick_initial_centers(ClusterCount, initial_centers, samples, test.get_kernel());
    test.train(samples, initial_centers);

    labels.reserve(samples.size());
    for (const auto& sample : samples) {
        labels.push_back(test(sample));
    }

    //---

    ovo_trainer trainer;

    dlib::krr_trainer<rbf_kernel> rbf_trainer;
    dlib::svm_nu_trainer<poly_kernel> poly_trainer;
    poly_trainer.set_kernel(poly_kernel(g, c, d));
    rbf_trainer.set_kernel(rbf_kernel(g));

    // Now tell the one_vs_one_trainer that, by default, it should use the rbf_trainer
    // to solve the individual binary classification subproblems.
    trainer.set_trainer(rbf_trainer);
    // We can also get more specific.  Here we tell the one_vs_one_trainer to use the
    // poly_trainer to solve the class 1 vs class 2 subproblem.  All the others will
    // still be solved with the rbf_trainer.
    trainer.set_trainer(poly_trainer, 1, 2);
    // Next, if you wanted to obtain the decision rule learned by a one_vs_one_trainer you
    // would store it into a one_vs_one_decision_function.
    dlib::one_vs_one_decision_function<ovo_trainer> df = trainer.train(samples, labels);
    // If you want to save a one_vs_one_decision_function to disk, you can do
    // so.  However, you must declare what kind of decision functions it contains.
    dlib::one_vs_one_decision_function<ovo_trainer,
            dlib::decision_function<poly_kernel>,  // This is the output of the poly_trainer
            dlib::decision_function<rbf_kernel>    // This is the output of the rbf_trainer
        > df2;

    // Put df into df2 and then save df2 to disk.  Note that we could have also said
    // df2 = trainer.train(samples, labels);  But doing it this way avoids retraining.
    df2 = df;
    dlib::serialize(fn+string(".dat")) << df2;

    ofstream f(fn+string(".txt"));

    for (size_t i = 0; i < samples.size(); i++)
        f << ConvrtDataLabelsToString(data[i], labels[i]) << endl;

    f.close();

    // Now let's do 5-fold cross-validation using the one_vs_one_trainer we just setup.
    // As an aside, always shuffle the order of the samples before doing cross validation.
    // For a discussion of why this is a good idea see the svm_ex.cpp example.
    MY_DEBUG_ONLY(
        //std::vector<double> temp_labels = labels;
        randomize_samples(samples, labels);
        cout << "cross validation: " << endl;
        cout << cross_validate_multiclass_trainer(trainer, samples, labels, 5) << endl;
    )

}
