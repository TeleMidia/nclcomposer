/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef RUNREMOTEGINGAVM_H
#define RUNREMOTEGINGAVM_H

#include <QThread>
#include <QProgressBar>
#include <model/Project.h>

#include "SimpleSSHClient.h"

class RunRemoteGingaVMAction : public QObject
{
  Q_OBJECT

public:
  explicit RunRemoteGingaVMAction();
  void setCurrentProject(Project* project);

public slots:
  void copyCurrentProject();
  void stopExecution();
  void setAutoplay(bool autoplay);

signals:
  void finished();
  void copyFinished();

  void startTask();
  void taskDescription(QString);
  void taskMaximumValue(int);
  void taskValue(int);

private:
  Project *project;
  bool mustStop;
  bool _autoplay;

  /*!
   * \brief Return the list of files to be sent to the Ginga Set-top box
   *  Virtual.
   *
   * \param project The project that we want to send.
   */
  QStringList filesToSendToGingaVM(Project *project, QString nclLocalPath);

  bool sendFilesToGingaVM(SimpleSSHClient &sshclient, QString baseLocalPath,
                          QString baseRemotePath, QStringList filesToSend);

  bool fixSrcsFromNCLFile(const QString &nclLocalPath);
};

class StopRemoteGingaVMAction : public QObject
{
  Q_OBJECT

public slots:
  void stopRunningApplication();

};
#endif // RUNREMOTEGINGAVM_H
