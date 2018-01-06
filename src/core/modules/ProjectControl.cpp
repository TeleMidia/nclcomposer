/* Copyright (C) 2011-2018 PUC-Rio/TeleMídia Lab.

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

#include "modules/ProjectControl.h"
#include "modules/ProjectReader.h"

CPR_CORE_BEGIN_NAMESPACE

INIT_SINGLETON (ProjectControl)

ProjectControl::~ProjectControl ()
{
  for (Project *proj : _openProjects.values ())
  {
    if (PluginControl::instance ()->releasePlugins (proj))
    {
      delete proj;
    }
    else
    {
      qCDebug (CPR_CORE) << "Error: Failed to releasePlugins ";
    }
  }
}

bool
ProjectControl::closeProject (const QString &path)
{
  if (!_openProjects.contains (path))
    return false;

  Project *project = _openProjects[path];
  if (PluginControl::instance ()->releasePlugins (project))
  {
    delete project;
    project = nullptr;
    _openProjects.remove (path);
  }
  else
  {
    qCDebug (CPR_CORE) << "Error: Failed to close the project :" << path;
    return false;
  }
  return true;
}

bool
ProjectControl::launchProject (const QString &path)
{
  if (_openProjects.contains (path))
  {
    emit projectAlreadyOpen (path);
    return false;
  }

  QString ext = path;
  ext = ext.remove (0, ext.lastIndexOf (".") + 1);
  LanguageType type = Utilities::languageTypeByExtension (ext);

  if (type == NONE)
  {
    return false;
  }

  /* Requests the LanguageProfile associated with this DocumentType */
  ILanguageProfile *profile
      = LanguageControl::instance ()->profileFromType (type);

  if (!profile)
  {
    emit notifyError (tr ("No Language Profile Extension found for (%1)")
                          .arg (ext.toUpper ()));
    return false;
  }

  emit startOpenProject (path);

  QMap<QString, QString> atts;
  QString documentId = path;
  documentId = documentId.remove (0, path.lastIndexOf ("/") + 1);
  atts["id"] = documentId;

  ProjectReader pr;
  Project *project = pr.readFile (path);

  if (project)
  {
    project->setAtrrs (atts);
    // The project was readed without a problem.
    project->setLocation (path);
    project->setProjectType (type);

    PluginControl::instance ()->launchProject (project);

    _openProjects[path] = project;
    connect (project, SIGNAL (dirtyProject (bool)), this,
             SLOT (projectIsDirty (bool)));
  }
  else
    qCDebug (CPR_CORE) << tr ("Project could not be open!");

  emit endOpenProject (path);

  return true;
}

void
ProjectControl::importFromDocument (const QString &docPath,
                                    const QString &projPath)
{
  if (_openProjects.contains (projPath))
  {
    emit projectAlreadyOpen (projPath);
    return;
  }

  QString ext = docPath;
  ext = ext.remove (0, ext.lastIndexOf (".") + 1);
  LanguageType type = Utilities::languageTypeByExtension (ext);

  if (type == NONE)
  {
    qCDebug (CPR_CORE) << "File format not recognized.";
    return;
  }

  /* Requests the LanguageProfile associated with this DocumentType */
  ILanguageProfile *profile
      = LanguageControl::instance ()->profileFromType (type);

  if (!profile)
  {
    emit notifyError (tr ("No Language Profile Extension found for (%1)")
                          .arg (ext.toUpper ()));
    return;
  }

  emit startOpenProject (projPath);

  QMap<QString, QString> atts;
  QString projectId = projPath;
  projectId.remove (0, projPath.lastIndexOf ("/") + 1);
  atts["id"] = projectId;

  ProjectReader pr;
  Project *project = pr.readFile (projPath);
  project->setAtrrs (atts);

  if (project != nullptr)
  {
    // The project was read without any problem.
    project->setLocation (projPath);
    project->setProjectType (type);

    PluginControl::instance ()->launchProject (project);
    project->setLocation (projPath);

    _openProjects[projPath] = project;
  }
  else
    qCDebug (CPR_CORE) << tr ("Project could not be open!");

  emit endOpenProject (projPath);

  QCoreApplication::processEvents ();

  if (project != nullptr)
  {
    IDocumentParser *parser;
    parser = profile->createParser (project);
    PluginControl::instance ()->connectParser (
        parser, PluginControl::instance ()->messageControl (project));

    QFile input (docPath);
    if (input.open (QIODevice::ReadOnly))
      parser->parseContent (input.readAll ());
    else
      qCDebug (CPR_CORE) << tr ("File could not be open!");

    input.close ();
  }

  project->setDirty (false);

  /* PluginControl::instance()->messageControl(project)
     ->setCurrentProjectAsDirty(); */
}

void
ProjectControl::saveProject (const QString &path)
{
  Project *project = _openProjects.value (path);
  QFile fout (path);

  if (!fout.exists ())
  {
    qCDebug (CPR_CORE) << "The file (" << path << ") doesn't exists. \
                           It will be created.";
  }

  if (!fout.open (QIODevice::WriteOnly))
  {
    // It could not open
    qCDebug (CPR_CORE) << "Failed to open file (" << path << ") for writing.";
    return;
  }

  QString content = project->toString ();
  fout.write (qCompress (content.toLatin1 (), content.size ()));
  //  fout.write (content.toLatin1 ());
  fout.close ();
  project->setDirty (false);
}

void
ProjectControl::moveProject (const QString &path, const QString &dest,
                             bool saveDest)
{
  QFileInfo fileInfo (dest);
  if (fileInfo.absoluteDir ().exists ())
  {
    Project *project = _openProjects.value (path);
    project->setLocation (dest);
    _openProjects.insert (dest, project);
    _openProjects.remove (path); // remove de old

    if (saveDest)
      saveProject (dest);

    QMap<QString, QString> atts;
    QString documentId = dest;
    documentId = documentId.remove (0, path.lastIndexOf ("/") + 1);
    atts["id"] = documentId;
    project->setAtrrs (atts);
  }
  else
    qCWarning (CPR_CORE) << "Could not copy the current project from  " << path
                         << " to " << dest;
}

void
ProjectControl::saveTempProject (const QString &path)
{
  Project *project = _openProjects.value (path);
  QFile fout (path + "~");

  qCDebug (CPR_CORE) << "Trying to autosave: " << path;
  if (!fout.exists ())
  {
    qCDebug (CPR_CORE) << "The file (" << path << ") doesn't exists.\
                            It will be created.";
  }

  if (!fout.open (QIODevice::WriteOnly))
  {
    // It could not open
    qCDebug (CPR_CORE) << "Failed to open file (" << path << ") for writing.";
    return;
  }

  QString content = project->toString ();
  fout.write (qCompress (content.toLatin1 (), content.size ()));
  //  fout.write (content.toLatin1 ());
  fout.close ();
  //  project->setDirty(false);
}

Project *
ProjectControl::project (const QString &location)
{
  if (_openProjects.contains (location))
    return _openProjects.value (location);

  return nullptr;
}

void
ProjectControl::projectIsDirty (bool isDirty)
{
  Project *project = qobject_cast<Project *> (QObject::sender ());
  if (project)
  {
    emit dirtyProject (project->location (), isDirty);
  }
  else
    qCDebug (CPR_CORE)
        << "Received a dirtyProject message for a nullptr project";
}

CPR_CORE_END_NAMESPACE
