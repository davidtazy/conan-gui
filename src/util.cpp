#include "util.h"
#include <sstream>
#include <vector>

namespace util {

std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}



std::vector<std::string> split_line(const std::string& chaine) {
  std::vector<std::string> ret;
  std::stringstream ss(chaine);
  std::string sousChaine;
  while (safeGetline(ss, sousChaine)) {
    if(sousChaine.size() > 0){
      ret.push_back(sousChaine);
    }
  }
  return ret;
}

std::string join_path(std::string p1, const std::string& p2) {
  char sep = '/';

#ifdef _WIN32
  sep = '\\';
#endif

  if (p1[p1.length() - 1] != sep && p2.size() > 0) {  // Need to add a
    p1 += sep;                   // path separator
  }
  return (p1 + p2);
}

std::string join_path(){
    return "";
}


}  // namespace util
