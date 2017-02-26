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

#include "RunRemoteGingaVM.h"

#include <QtXml>
#include <QDomDocument>
#include <QStack>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QMessageBox>
#include <QApplication>
#include <iostream>

#include <util/ComposerSettings.h>
using namespace composer::core::util;

RunRemoteGingaVMAction::RunRemoteGingaVMAction()
{
  _autoplay = true;
}

void RunRemoteGingaVMAction::setCurrentProject(Project *project)
{
  this->project = project;
}

QStringList RunRemoteGingaVMAction::filesToSendToGingaVM(Project *project,
                                                         QString nclLocalPath)
{
  Q_UNUSED(nclLocalPath)

  QStringList filesToSend;
  QFileInfo fileInfo(project->getLocation());
  QString absoluteFilePath = fileInfo.path();

//  qCDebug(CPR_GUI)() << "############ ABSOLUTE PATH: " << absoluteFilePath << endl;
  QDirIterator it(absoluteFilePath, QDirIterator::Subdirectories);
  while (it.hasNext()) {
    QString file = it.next();
    if(!file.endsWith("."))
    {
      filesToSend << file;
//      qCDebug(CPR_GUI)() << file;
    }
  }

  // Search the files for all required files.
  /* QStack <Entity*> stack;
  Entity *current = project;
  stack.push(current);
  while(stack.size())
  {
    current = stack.top();
    stack.pop();

    QString path = "", fullpath = "";
    if(current->hasAttribute("src"))
      path = current->getAttribute("src");
    if(current->hasAttribute("documentURI"))
      path = current->getAttribute("documentURI");
    if(current->hasAttribute("focusSrc"))
      path = current->getAttribute("focusSrc");
    if(current->hasAttribute("focusSelSrc"))
      path = current->getAttribute("focusSelSrc");

    qCDebug(CPR_GUI)() << "path " << path;

    // Fix the fullpath if it is necessary
    if(path != "")
    {
      fullpath = path;
      QFileInfo fileInfo (fullpath);
      if(fileInfo.exists())
      {
        fullpath = fileInfo.absoluteFilePath();
      }
      else
      {
        fileInfo.setFile(nclLocalPath);
        QString tmp = fileInfo.absoluteDir().absolutePath();
        tmp += QDir::separator();
        tmp += fullpath;
        fileInfo.setFile(tmp);

        if(fileInfo.exists())
          fullpath = fileInfo.absoluteFilePath();
      }

      if(fullpath != "")
        filesToSend << fullpath;
    }

    QVector <Entity*> children = current->getChildren();
    for(int i = 0; i < children.size(); i++)
    {
      stack.push(children[i]);
    }
  }*/
  return filesToSend;
}

bool RunRemoteGingaVMAction::sendFilesToGingaVM(SimpleSSHClient &sshclient,
                                                QString baseLocalPath,
                                                QString baseRemotePath,
                                                QStringList filesToSend)
{
  int ret = 1;

  // \todo This search MUST BE RECURSIVE!!!
  // \todo This also can be a function.

  if(!baseRemotePath.endsWith("/"))
    baseRemotePath.append("/");

  // This will force the creation of the baseRemotePath if it does not exists
  // filesToSend.prepend(baseRemotePath);
  qCDebug(CPR_GUI)() << "Must send these files:" << filesToSend;

  emit taskDescription(tr("Sending files to remote machine..."));
  emit taskMaximumValue(filesToSend.size());

  // Force the creation of remotePath (in case it does not exist)
  QString mkdir = "mkdir -p ";
  mkdir += baseRemotePath;

  qCDebug(CPR_GUI)() << "Running command = " << mkdir;
  ret = !sshclient.exec_cmd(mkdir.toStdString().c_str());

  for(int i = 0; i < filesToSend.size(); i++)
  {
    if(mustStop) break;

    emit taskValue(i);

    int resp = 0;
    QString fullpath = filesToSend.at(i);
    if(fullpath.contains(baseLocalPath))
    {
      QString relativePath = fullpath.mid(baseLocalPath.size()+1);
      QString relativePathDir = relativePath.mid(0, relativePath.lastIndexOf("/")+1);
      QFileInfo fileInfo(fullpath);

      if(fileInfo.isFile())
      {
        qCWarning(CPR_GUI) << "Sending file = " << fullpath <<
                    " to " << baseRemotePath + relativePathDir;

        resp = sshclient.sftp_copy_file( fullpath.toStdString().c_str(),
                         (baseRemotePath+relativePathDir).toStdString().c_str());
      }
      else
      {
        QString mkdir = "mkdir -p ";
        mkdir += baseRemotePath + relativePath;

        qCWarning(CPR_GUI) << "Running command = " << mkdir;
        resp = sshclient.exec_cmd(mkdir.toStdString().c_str());
      }
    }

    ret = ret && !resp;
  }

  return ret;
}

bool RunRemoteGingaVMAction::fixSrcsFromNCLFile(const QString &nclLocalPath)
{
  // Fix all the paths in the nclFile.
  QFile tmpXmlFile(nclLocalPath);
  QDomDocument document("doc");
  document.setContent(&tmpXmlFile);
  QStack <QDomElement> elements;
  elements.push(document.documentElement());
  while(elements.size())
  {
    QDomElement current = elements.top();
    elements.pop();

    if(current.hasAttribute("uniqueEntityId"))
      current.removeAttribute("uniqueEntityId");

    if(current.hasAttribute("src"))
    {
      QString src = current.attribute("src");
      int start = src.lastIndexOf(QDir::separator());
      src = src.mid(start+1);
      current.setAttribute("src", src);
      qCDebug(CPR_GUI)() << src;
    }
    if(current.hasAttribute("documentURI"))
    {
      QString src = current.attribute("documentURI");
      int start = src.lastIndexOf(QDir::separator());
      src = src.mid(start+1);
      current.setAttribute("documentURI", src);
    }
    if(current.hasAttribute("focusSrc"))
    {
      QString src = current.attribute("focusSrc");
      int start = src.lastIndexOf(QDir::separator());
      src = src.mid(start+1);
      current.setAttribute("focusSrc", src);
    }
    if(current.hasAttribute("focusSelSrc"))
    {
      QString src = current.attribute("focusSelSrc");
      int start = src.lastIndexOf(QDir::separator());
      src = src.mid(start+1);
      current.setAttribute("focusSelSrc", src);
    }

    QDomElement child = current.firstChildElement();
    while(!child.isNull())
    {
      elements.push_back(child);
      child = child.nextSiblingElement();
    }
  }
  tmpXmlFile.close();

  // Replace the old XML file with the with the new source fixed!!!
  if (tmpXmlFile.open( QIODevice::WriteOnly ))
  {
    QTextStream textStream(&tmpXmlFile);
    textStream << document.toString() ;
    tmpXmlFile.close();
  }
  else
    qCWarning(CPR_GUI) << "I couldn't fix the path in XML file, probably it will "
               << "not run!!";

  return true;
}

void RunRemoteGingaVMAction::copyCurrentProject()
{
  mustStop = false;

  emit startTask();
//  emit taskMaximumValue(3);

  // Checks if there is a current NCL project.
  QString location = project->getLocation();

  emit taskDescription(tr("Getting remote connection configuration"));
  // Getting the settings user data.
  GlobalSettings settings;
  settings.beginGroup("runginga");
  QString remoteIp = settings.value("remote_ip").toString();
  QString remoteUser = settings.value("remote_user").toString();
  QString remotePasswd = settings.value("remote_password").toString();
  QString remotePath = settings.value("remote_path").toString();
  QString remoteCmd = settings.value("remote_start_cmd").toString();
  settings.endGroup();

  /*\todo Put the code to run the remote NCL */
  SimpleSSHClient sshclient(remoteUser.toStdString().c_str(),
                            remotePasswd.toStdString().c_str(),
                            remoteIp.toStdString().c_str(),
                            remotePath.toStdString().c_str());

  // emit taskValue(1);
  emit taskDescription(tr("Trying to connect to machine from IP: %1").
                       arg(remoteIp));


  qCWarning(CPR_GUI) << "Trying to connect to remote machine...";

  int connRet = sshclient.doConnect();

  if( connRet != 0 && !mustStop)
  {
    qCWarning(CPR_GUI) << "Could not connect to remote machine...";
    emit finished();
    return;
  }

  QFileInfo fileNCLInfo (location);
  QString tmpNCLDir = fileNCLInfo.absoluteDir().absolutePath();
  QString nclLocalPath = tmpNCLDir;
  nclLocalPath += "/";
  nclLocalPath += "tmp.ncl";

  emit taskDescription(tr("Computing files to transmit..."));

  /* First, get the list of files to send */
  QStringList filesToSend = filesToSendToGingaVM(project, nclLocalPath);
  /* and send them to Ginga VM*/

  if(!mustStop &&
     sendFilesToGingaVM(sshclient, tmpNCLDir, remotePath, filesToSend))
  {
    emit copyFinished();
    /* Now, fix all the paths from NCL, and send the NCL file*/
    QFile file(nclLocalPath);
    if(file.open(QFile::WriteOnly | QIODevice::Truncate))
    {
      /* Write FILE!! */
      if(project->getChildren().size())
        file.write(project->getChildren().at(0)->toString(0, false).toLatin1());

      file.close();

//      if(fixSrcsFromNCLFile(nclLocalPath))
//      {
        /* RUNNING GINGA */

      if (_autoplay)
      {
        sshclient.sftp_copy_file(nclLocalPath.toStdString().c_str(),
                                remotePath.toStdString().c_str());
        QString cmd = remoteCmd;
        cmd += " ";
        cmd += remotePath + "/tmp.ncl";
        if(!mustStop)
          sshclient.exec_cmd(cmd.toStdString().c_str());
      }
//      }
    }
    else
    {
      qCWarning(CPR_GUI) << "Error trying to run NCL. Could not create : "
                 << nclLocalPath << " !";
    }
  }
  else
  {
    qCWarning(CPR_GUI) << "Error copying the dependency files";
  }

  sshclient.doDisconnect();

  emit finished();
}

void RunRemoteGingaVMAction::setAutoplay(bool autoplay)
{
  _autoplay = autoplay;
}

void RunRemoteGingaVMAction::stopExecution()
{
  qCDebug(CPR_GUI)() << "RunRemoteGingaVMAction::stopExecution";
  mustStop = true;
  emit taskDescription(tr("Cancelling..."));

  emit finished();
}

void StopRemoteGingaVMAction::stopRunningApplication()
{
  // Getting the settings user data.
  GlobalSettings settings;
  settings.beginGroup("runginga");
  QString remoteIp = settings.value("remote_ip").toString();
  QString remoteUser = settings.value("remote_user").toString();
  QString remotePasswd = settings.value("remote_password").toString();
  QString remotePath = settings.value("remote_path").toString();
  QString remoteStopCmd = settings.value("remote_stop_cmd").toString();
  settings.endGroup();

  /*\todo Put the code to run the remote NCL */
  SimpleSSHClient sshclient(remoteUser.toStdString().c_str(),
                            remotePasswd.toStdString().c_str(),
                            remoteIp.toStdString().c_str(),
                            remotePath.toStdString().c_str());

  int connRet = sshclient.doConnect();
  if( connRet != 0)
  {
    qCWarning(CPR_GUI) << "Could not connect to remote machine...";
    return;
  }

  sshclient.exec_cmd(remoteStopCmd.toStdString().c_str());
  sshclient.doDisconnect();
}
