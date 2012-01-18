/* Copyright (c) 2011-2012 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "RunRemoteGingaVM.h"

#include <QtXml>
#include <QDomDocument>
#include <QStack>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QMessageBox>
#include <QSettings>

void RunRemoteGingaVMAction::setCurrentProject(Project *project)
{
  this->project = project;
}

QStringList RunRemoteGingaVMAction::filesToSendToGingaVM(Project *project,
                                                         QString nclLocalPath)
{
  // Search the files for all required files.
  QStack <Entity*> stack;
  Entity *current = project;
  stack.push(current);

  QStringList filesToSend;

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

    qDebug() << "path " << path;

    /* Fix the fullpath if it is necessary */
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
  }
  return filesToSend;
}

bool RunRemoteGingaVMAction::sendFilesToGingaVM(SimpleSSHClient &sshclient,
                                                QStringList filesToSend)
{
  int ret = 1;
  // \todo This search MUST BE RECURSIVE!!!
  // \todo This also can be a function.
  qDebug() << "Must send this files:" << filesToSend;
  for(int i = 0; i < filesToSend.size(); i++)
  {
    QString fullpath = filesToSend.at(i);
    qDebug() << " FULLPATH = " << fullpath;
    int resp = sshclient.scp_copy_file(fullpath.toStdString().c_str());
    ret = ret && !resp;
  }

  return ret;
}

void RunRemoteGingaVMAction::runCurrentProject()
{
  // Checks if there is a current NCL project.
  QString location = project->getLocation();

  // Getting the settings user data.
  QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                     "telemidia", "composer");
  settings.beginGroup("runginga");
  QString remoteIp = settings.value("remote_ip").toString();
  QString remoteUser = settings.value("remote_user").toString();
  QString remotePasswd = settings.value("remote_password").toString();
  QString remotePath = settings.value("remote_path").toString();
  QString remoteCmd = settings.value("remote_cmd").toString();
  settings.endGroup();

  /*\todo Put the code to run the remote NCL */
  SimpleSSHClient sshclient(remoteUser.toStdString().c_str(),
                            remotePasswd.toStdString().c_str(),
                            remoteIp.toStdString().c_str(),
                            remotePath.toStdString().c_str());

  QFileInfo fileNCLInfo (location);
  QString tmpNCLDir = fileNCLInfo.absoluteDir().absolutePath();
  QString nclLocalPath = tmpNCLDir;
  nclLocalPath += "/";
  nclLocalPath += "tmp.ncl";
  qDebug() << "Running NCL File (on a remote Machine): " << nclLocalPath;

  /* First, get the list of files to send */
  QStringList filesToSend = filesToSendToGingaVM(project, nclLocalPath);
  /* and send them to Ginga VM*/
  if(sendFilesToGingaVM(sshclient, filesToSend))
  {
    /* Now, fix all the paths from NCL, and send the NCL file*/
    QFile file(nclLocalPath);
    if(file.open(QFile::WriteOnly | QIODevice::Truncate))
    {
      /* Write FILE!! */
      if(project->getChildren().size())
        file.write(project->getChildren().at(0)->toString(0).toAscii());

      file.close();

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
          qDebug() << src;
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

      // Replace the old XML file with the with the source fixed!!!
      if (tmpXmlFile.open( QIODevice::WriteOnly ))
      {
        QTextStream textStream(&tmpXmlFile);
        textStream << document.toString() ;
        tmpXmlFile.close();
      }
      else
        qWarning() << "I couldn't fix the path in XML file, probably it will "
                   << "not run!!";

      /* RUNNING GINGA */
      sshclient.scp_copy_file(nclLocalPath.toStdString().c_str());
      QString cmd = remoteCmd;
      cmd += " ";
      cmd += remotePath + "/tmp.ncl";
      sshclient.exec_cmd(cmd.toStdString().c_str());
    }
    else
    {
      qWarning() << "Error trying to run NCL. Could not create : "
                 << nclLocalPath << " !";
    }
  }
  else
  {
    qDebug() << "Error copying the dependency files";
  }

  emit finished();
}

void StopRemoteGingaVMAction::stopRunningApplication()
{
  // Getting the settings user data.
  QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                     "telemidia", "composer");
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

  sshclient.exec_cmd(remoteStopCmd.toStdString().c_str());

}
