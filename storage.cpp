#include "storage.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace {
std::vector<std::string> split(const std::string& value, char delimiter) {
    std::vector<std::string> result;
    std::stringstream stream(value);
    std::string part;
    while (std::getline(stream, part, delimiter)) {
        if (!part.empty()) result.push_back(part);
    }
    return result;
}

std::string join(const std::vector<std::string>& values, char delimiter) {
    std::string result;
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i) result += delimiter;
        result += values[i];
    }
    return result;
}

int safeInt(const std::string& value) {
    try { return std::stoi(value); } catch (...) { return 0; }
}

int nextIdFrom(const std::vector<int>& ids) {
    return ids.empty() ? 1 : *std::max_element(ids.begin(), ids.end()) + 1;
}
}

Storage::Storage(const std::string& directory) : dataDirectory(directory) {
    ensureDirectory();
}

void Storage::ensureDirectory() const {
    std::filesystem::create_directories(dataDirectory);
}

std::string Storage::candidatesPath() const { return dataDirectory + "/candidates.txt"; }
std::string Storage::jobsPath() const { return dataDirectory + "/jobs.txt"; }
std::string Storage::applicationsPath() const { return dataDirectory + "/applications.txt"; }

std::vector<Candidate> Storage::loadCandidates() const {
    std::vector<Candidate> items;
    std::ifstream in(candidatesPath());
    std::string line;
    while (std::getline(in, line)) {
        auto p = split(line, '|');
        if (p.size() < 5) continue;
        Candidate c;
        c.id = safeInt(p[0]);
        c.name = p[1];
        c.education = p[2];
        c.experienceYears = safeInt(p[3]);
        c.skills = split(p[4], ',');
        items.push_back(c);
    }
    return items;
}

std::vector<Job> Storage::loadJobs() const {
    std::vector<Job> items;
    std::ifstream in(jobsPath());
    std::string line;
    while (std::getline(in, line)) {
        auto p = split(line, '|');
        if (p.size() < 6) continue;
        Job j;
        j.id = safeInt(p[0]);
        j.company = p[1];
        j.title = p[2];
        j.location = p[3];
        j.experienceRequired = safeInt(p[4]);
        j.skills = split(p[5], ',');
        items.push_back(j);
    }
    return items;
}

std::vector<Application> Storage::loadApplications() const {
    std::vector<Application> items;
    std::ifstream in(applicationsPath());
    std::string line;
    while (std::getline(in, line)) {
        auto p = split(line, '|');
        if (p.size() < 3) continue;
        items.push_back({safeInt(p[0]), safeInt(p[1]), p[2]});
    }
    return items;
}

bool Storage::saveCandidates(const std::vector<Candidate>& items) const {
    ensureDirectory();
    std::ofstream out(candidatesPath());
    if (!out) return false;
    for (const auto& c : items)
        out << c.id << '|' << c.name << '|' << c.education << '|'
            << c.experienceYears << '|' << join(c.skills, ',') << '\n';
    return true;
}

bool Storage::saveJobs(const std::vector<Job>& items) const {
    ensureDirectory();
    std::ofstream out(jobsPath());
    if (!out) return false;
    for (const auto& j : items)
        out << j.id << '|' << j.company << '|' << j.title << '|'
            << j.location << '|' << j.experienceRequired << '|'
            << join(j.skills, ',') << '\n';
    return true;
}

bool Storage::saveApplications(const std::vector<Application>& items) const {
    ensureDirectory();
    std::ofstream out(applicationsPath());
    if (!out) return false;
    for (const auto& a : items)
        out << a.id << '|' << a.jobId << '|' << a.status << '\n';
    return true;
}

int Storage::nextCandidateId() const {
    std::vector<int> ids;
    for (const auto& c : loadCandidates()) ids.push_back(c.id);
    return nextIdFrom(ids);
}
int Storage::nextJobId() const {
    std::vector<int> ids;
    for (const auto& j : loadJobs()) ids.push_back(j.id);
    return nextIdFrom(ids);
}
int Storage::nextApplicationId() const {
    std::vector<int> ids;
    for (const auto& a : loadApplications()) ids.push_back(a.id);
    return nextIdFrom(ids);
}
