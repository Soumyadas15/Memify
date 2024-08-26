#ifndef SNAPSHOTFILE_H
#define SNAPSHOTFILE_H

#include "ISnapshot.h"
#include <fstream>

class Snapshot : public ISnapshot
{
public:
    explicit Snapshot(const std::string &filename);
    ~Snapshot() override;
    void Save(const std::unordered_map<std::string, std::pair<std::string, std::chrono::steady_clock::time_point>> &data) override;
    void Load(std::unordered_map<std::string, std::pair<std::string, std::chrono::steady_clock::time_point>> &data) override;

private:
    std::string filename_;
};

#endif