#include "process.h"
#include <QFileInfo>
#include <QProcess>

#include <algorithm>

using namespace std::literals::string_literals;

namespace conanqt {

Process::Process(std::string path) : _path{QString::fromStdString(path)} {
  connect(&_process, &QProcess::readyReadStandardOutput, this, &Process::canReadStandardOutput);

  connect(&_process, &QProcess::readyReadStandardError, this, &Process::canReadStandardError);

  connect(&_process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this,
          &Process::finished);
}

QStringList toQStringList(const Args& args) {
  QStringList myList;
  for (auto arg : args) {
    myList.push_back(QString::fromStdString(arg));
  }
  return myList;
}

int Process::long_call(const Args& args, OutputStream callback_p) {
  _callback = callback_p;
  QProcess& p = _process;

  QStringList myList = toQStringList(args);

  p.start(_path, myList);
  if (!p.waitForStarted())
    throw std::runtime_error("cannot start process ["s + _path.toStdString() + "]"s);

  _callback(QString("%1 %2").arg(_path).arg(myList.join(" ")).toStdString(), Std::Cmd);

  return 255;
}

void Process::set_executable(std::string path) {
  _path = QString::fromStdString(path);
}

OutProcess Process::call(const Args& args) {
  QProcess p;

  QStringList myList = toQStringList(args);

  p.start(_path, myList);
  if (!p.waitForStarted())
    return {-1, "", "cannot start process ["s + _path.toStdString() + "]"s};

  if (!p.waitForFinished())
    return {-1, p.readAllStandardOutput().toStdString(),
            " finish process with error"s + p.readAllStandardError().toStdString()};

  return {p.exitCode(), p.readAllStandardOutput().toStdString(),
          p.readAllStandardError().toStdString()};
}

std::string Process::which() {
  return _path.toStdString();
}
#include <QByteArray>
void Process::canReadStandardOutput() {
  QByteArray out = _process.readAllStandardOutput();
  _callback(out.toStdString(), Std::Out);
}
void Process::canReadStandardError() {
  QByteArray err = _process.readAllStandardError();
  _callback(err.toStdString(), Std::Err);
}

void Process::finished(int error_code, QProcess::ExitStatus exitStatus) {
  //
  _callback(std::to_string(error_code), Std::RetCode);
}

}  // namespace conanqt
