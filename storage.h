#ifndef STORAGE_H
#define STORAGE_H

#include "models.h"
#include <string>
#include <vector>

class Storage {
public:
    explicit Storage(const std::string& directory = "data");

    std::vector<Candidate> loadCandidates() const;
    std::vector<Job> loadJobs() const;
    std::vector<Application> loadApplications() const;

    bool saveCandidates(const std::vector<Candidate>& items) const;
    bool saveJobs(const std::vector<Job>& items) const;
    bool saveApplications(const std::vector<Application>& items) const;

    int nextCandidateId() const;
    int nextJobId() const;
    int nextApplicationId() const;

private:
    std::string dataDirectory;
    std::string candidatesPath() const;
    std::string jobsPath() const;
    std::string applicationsPath() const;
    void ensureDirectory() const;
};

#endif
