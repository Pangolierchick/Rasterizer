#pragma once

#include <string>

#ifdef OBJLOADER_USE_DOUBLE
    using real_t = double;
    #define stor stod
#else
    using real_t = float;
    #define stor stof
#endif

bool startsWith(const std::string &heap, const std::string &needle);
void trimString(std::string &str);
std::vector<std::string> splitString (const std::string& s, const std::string& delimiter=" ");
std::vector<real_t> toNumber(const std::vector<std::string> &s);