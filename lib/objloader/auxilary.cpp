#include "auxilary.h"
#include <sstream>
#include <vector>

#include <iostream>

std::vector<std::string> splitString(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    static std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;

        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

bool startsWith(const std::string &heap, const std::string &needle) {
    size_t i = 0;
    for (; heap[i] == ' ' && i < heap.length(); i++);

    size_t j = 0;
    for (; heap[i] != needle[j]; i++, j++)
        return false;

    return true;
}

std::vector<real_t> toNumber(const std::vector<std::string> &s) {
    std::vector<real_t> out;

    out.reserve(s.size());

    for (auto &word : s) {
        out.push_back(std::stor(word));
    }

    return out;
}

void trimString(std::string &str) {
    size_t s = 0;
    size_t e = str.length() - 1;

    while (str[s] == ' ' && s < str.length())
        s++;

    while (str[e] == ' ' && e >= 0)
        e--;

    str.resize(e + 1);
    str.erase(0, s);
}