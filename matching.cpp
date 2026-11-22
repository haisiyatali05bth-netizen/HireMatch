#include "matching.h"
#include <unordered_set>
#include <algorithm>
#include <cctype>

std::vector<JobMatch> MatchingEngine::recommend(const Candidate& candidate,
                                                 const std::vector<Job>& jobs) {
    std::unordered_set<std::string> candidateSkills;
    for (auto skill : candidate.skills) {
        std::transform(skill.begin(), skill.end(), skill.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
        candidateSkills.insert(skill);
    }

    std::vector<JobMatch> results;
    for (const auto& job : jobs) {
        JobMatch match;
        match.job = job;
        for (auto skill : job.skills) {
            std::transform(skill.begin(), skill.end(), skill.begin(),
                           [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
            if (candidateSkills.count(skill))
                match.matchedSkills.push_back(skill);
            else
                match.missingSkills.push_back(skill);
        }
        match.score = job.skills.empty() ? 0.0
            : 100.0 * match.matchedSkills.size() / job.skills.size();
        results.push_back(match);
    }

    if (!results.empty())
        mergeSort(results, 0, static_cast<int>(results.size()) - 1);
    return results;
}

void MatchingEngine::mergeSort(std::vector<JobMatch>& a, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(a, left, mid);
    mergeSort(a, mid + 1, right);
    merge(a, left, mid, right);
}

void MatchingEngine::merge(std::vector<JobMatch>& a, int left, int mid, int right) {
    std::vector<JobMatch> temp;
    int i = left, j = mid + 1;
    while (i <= mid && j <= right) {
        if (a[i].score >= a[j].score) temp.push_back(a[i++]);
        else temp.push_back(a[j++]);
    }
    while (i <= mid) temp.push_back(a[i++]);
    while (j <= right) temp.push_back(a[j++]);
    for (int k = 0; k < static_cast<int>(temp.size()); ++k)
        a[left + k] = temp[k];
}
