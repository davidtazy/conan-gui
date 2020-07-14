#pragma once
#include <string>
#include <vector>

namespace util {
std::vector<std::string> split_line(const std::string& chaine);

template<class ...Args>
std::string join_path(std::string p1, Args... args){
    return join_path(p1,join_path(args...));
    
}
std::string join_path();

std::string join_path(std::string p1, const std::string& p2);
}  // namespace util
