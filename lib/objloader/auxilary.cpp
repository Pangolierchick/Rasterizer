#include "auxilary.h"
#include <sstream>
#include <vector>

#include <iostream>

std::vector<std::string> splitString(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while (s.substr(pos_start, pos_start + delim_len) == delimiter)
        pos_start++;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;

        if (token.empty()) {
            res.emplace_back();
        } else {
            res.push_back(token);
        }
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