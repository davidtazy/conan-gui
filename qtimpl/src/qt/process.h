#pragma once

#include <interfaces/iprocess.h>
#include <QObject>
#include <QProcess>
#include <QString>
namespace conanqt {

struct Process : public QObject, public IProcess {
  Q_OBJECT

 public:
  explicit Process(std::string path);

  void set_executable(std::string path) override;

  int long_call(const Args& args, OutputStream callback_p) override;

  OutProcess call(const Args& args) override;

  std::string which() override;

 private slots:
  void canReadStandardOutput();
  void canReadStandardError();
  void finished(int error_code, QProcess::ExitStatus exitStatus);

 private:
  QString _path;
  QProcess _process;
  OutputStream _callback;
};

}  // namespace conanqt
