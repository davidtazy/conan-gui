#include "util.h"
#include <sstream>
#include <vector>

namespace util {
std::vector<std::string> split_line(const std::string& chaine) {
  std::vector<std::string> ret;
  std::stringstream ss(chaine);
  std::string sousChaine;
  while (std::getline(ss, sousChaine)) {
    ret.push_back(sousChaine);
  }
  return ret;
}

std::string join_path(std::string p1, const std::string& p2) {
  char sep = '/';

#ifdef _WIN32
  sep = '\\';
#endif

  if (p1[p1.length()] != sep) {  // Need to add a
    p1 += sep;                   // path separator
  }
  return (p1 + p2);
}

}  // namespace util
