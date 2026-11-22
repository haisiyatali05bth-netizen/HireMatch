#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

struct Candidate {
    int id = 0;
    std::string name;
    std::string education;
    int experienceYears = 0;
    std::vector<std::string> skills;
};

struct Job {
    int id = 0;
    std::string company;
    std::string title;
    std::string location;
    int experienceRequired = 0;
    std::vector<std::string> skills;
};

struct Application {
    int id = 0;
    int jobId = 0;
    std::string status = "Pending";
};

struct JobMatch {
    Job job;
    double score = 0.0;
    std::vector<std::string> matchedSkills;
    std::vector<std::string> missingSkills;
};

#endif
