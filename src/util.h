#pragma once
#include <string>
#include <vector>

namespace util {
std::vector<std::string> split_line(const std::string& chaine);

std::string join_path(std::string p1, const std::string& p2);
}  // namespace util
