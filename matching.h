#ifndef MATCHING_H
#define MATCHING_H

#include "models.h"
#include <vector>

class MatchingEngine {
public:
    static std::vector<JobMatch> recommend(const Candidate& candidate,
                                          const std::vector<Job>& jobs);
private:
    static void mergeSort(std::vector<JobMatch>& items, int left, int right);
    static void merge(std::vector<JobMatch>& items, int left, int mid, int right);
};

#endif
