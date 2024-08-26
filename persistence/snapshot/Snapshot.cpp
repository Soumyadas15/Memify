#include "Snapshot.h"
#include <sstream>
#include <iostream>

Snapshot::Snapshot(const std::string &filename) : filename_(filename) {}

Snapshot::~Snapshot() = default;

void Snapshot::Save(const std::unordered_map<std::string, std::pair<std::string, std::chrono::steady_clock::time_point>> &data)
{
    std::ofstream ofs(filename_, std::ios::binary);
    if (!ofs)
    {
        std::cerr << "Failed to open file for writing: " << filename_ << std::endl;
        return;
    }

    for (const auto &[key, value] : data)
    {
        size_t key_size = key.size();
        ofs.write(reinterpret_cast<const char *>(&key_size), sizeof(key_size));
        ofs.write(key.data(), key_size);

        size_t value_size = value.first.size();
        ofs.write(reinterpret_cast<const char *>(&value_size), sizeof(value_size));
        ofs.write(value.first.data(), value_size);

        auto expiration = std::chrono::duration_cast<std::chrono::seconds>(value.second.time_since_epoch()).count();
        ofs.write(reinterpret_cast<const char *>(&expiration), sizeof(expiration));
    }
}

void Snapshot::Load(std::unordered_map<std::string, std::pair<std::string, std::chrono::steady_clock::time_point>> &data)
{
    std::ifstream ifs(filename_, std::ios::binary);
    if (!ifs)
    {
        std::cerr << "Failed to open file for reading: " << filename_ << std::endl;
        return;
    }

    while (ifs)
    {
        size_t key_size;
        ifs.read(reinterpret_cast<char *>(&key_size), sizeof(key_size));
        if (ifs.eof()) break;

        std::string key(key_size, '\0');
        ifs.read(key.data(), key_size);

        size_t value_size;
        ifs.read(reinterpret_cast<char *>(&value_size), sizeof(value_size));
        std::string value(value_size, '\0');
        ifs.read(value.data(), value_size);

        std::chrono::steady_clock::time_point expiration;
        long long expiration_seconds;
        ifs.read(reinterpret_cast<char *>(&expiration_seconds), sizeof(expiration_seconds));
        expiration = std::chrono::steady_clock::time_point(std::chrono::seconds(expiration_seconds));

        data[key] = {value, expiration};
    }
}