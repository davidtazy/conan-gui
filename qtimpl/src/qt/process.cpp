#include "process.h"
#include <QFileInfo>
#include <QProcess>

#include <algorithm>
namespace conanqt {

int Process::long_call(const Args& args) {
  throw std::runtime_error("conanqt::Process::long_call not implemented");
  return 255;
}

void Process::set_executable(std::string path) {
  _path = path;
}

using namespace std::literals::string_literals;

OutProcess Process::call(const Args& args) {
  QProcess p;

  QStringList myList;
  for (auto arg : args) {
    myList.push_back(QString::fromStdString(arg));
  }

  p.start(QString::fromStdString(_path), myList);
  if (!p.waitForStarted())
    return {-1, "", "cannot start process ["s + _path + "]"s};

  if (!p.waitForFinished())
    return {-1, p.readAllStandardOutput().toStdString(),
            " finish process with error"s + p.readAllStandardError().toStdString()};

  return {p.exitCode(), p.readAllStandardOutput().toStdString(),
          p.readAllStandardError().toStdString()};
}

std::string Process::which() {
  return _path;
}

}  // namespace conanqt
