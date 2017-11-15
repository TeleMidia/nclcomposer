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

#include "modules/ProjectControl.h"
#include "modules/ProjectReader.h"

CPR_CORE_BEGIN_NAMESPACE

INIT_SINGLETON (ProjectControl)

ProjectControl::ProjectControl () {}

ProjectControl::~ProjectControl ()
{
  QMap<QString, Project *>::iterator it;
  PluginControl *pg = PluginControl::instance ();
  for (it = _openProjects.begin (); it != _openProjects.end (); it++)
  {
    Project *project = it.value ();
    if (pg->releasePlugins (project))
    {
      delete project;
      project = nullptr;
    }
    else
    {
      qCDebug (CPR_CORE) << "Error: Failed to releasePlugins ";
    }
  }
}

bool
ProjectControl::closeProject (const QString &location)
{
  if (!_openProjects.contains (location))
    return false;

  Project *project = _openProjects[location];
  if (PluginControl::instance ()->releasePlugins (project))
  {
    delete project;
    project = nullptr;
    _openProjects.remove (location);
  }
  else
  {
    qCDebug (CPR_CORE) << "Error: Failed to close the project :" << location;
    return false;
  }
  return true;
}

bool
ProjectControl::launchProject (const QString &location)
{
  if (_openProjects.contains (location))
  {
    emit projectAlreadyOpen (location);
    return false;
  }

  QString ext = location;
  ext = ext.remove (0, ext.lastIndexOf (".") + 1);
  LanguageType type = Utilities::getLanguageTypeByExtension (ext);

  if (type == NONE)
  {
    return false;
  }

  /* Requests the LanguageProfile associated with this DocumentType */
  ILanguageProfile *profile
      = LanguageControl::instance ()->getProfileFromType (type);

  if (!profile)
  {
    emit notifyError (tr ("No Language Profile Extension found for (%1)")
                          .arg (ext.toUpper ()));
    return false;
  }

  emit startOpenProject (location);

  QMap<QString, QString> atts;
  QString documentId = location;
  documentId = documentId.remove (0, location.lastIndexOf ("/") + 1);
  atts["id"] = documentId;

  ProjectReader pr;
  Project *project = pr.readFile (location);

  if (project != nullptr)
  {
    project->setAtrrs (atts);
    // The project was readed without a problem.
    project->setLocation (location);
    project->setProjectType (type);

    PluginControl::instance ()->launchProject (project);

    _openProjects[location] = project;
    connect (project, SIGNAL (dirtyProject (bool)), this,
             SLOT (projectIsDirty (bool)));
  }
  else
    qCDebug (CPR_CORE) << tr ("Project could not be open!");

  emit endOpenProject (location);

  return true;
}

void
ProjectControl::importFromDocument (const QString &docLocation,
                                    const QString &projLocation)
{
  if (_openProjects.contains (projLocation))
  {
    emit projectAlreadyOpen (projLocation);
    return;
  }

  QString ext = docLocation;
  ext = ext.remove (0, ext.lastIndexOf (".") + 1);
  LanguageType type = Utilities::getLanguageTypeByExtension (ext);

  if (type == NONE)
  {
    qCDebug (CPR_CORE) << "File format not recognized.";
    return;
  }

  /* Requests the LanguageProfile associated with this DocumentType */
  ILanguageProfile *profile
      = LanguageControl::instance ()->getProfileFromType (type);

  if (!profile)
  {
    emit notifyError (tr ("No Language Profile Extension found for (%1)")
                          .arg (ext.toUpper ()));
    return;
  }

  emit startOpenProject (projLocation);

  QMap<QString, QString> atts;
  QString projectId = projLocation;
  projectId.remove (0, projLocation.lastIndexOf ("/") + 1);
  atts["id"] = projectId;

  ProjectReader pr;
  Project *project = pr.readFile (projLocation);
  project->setAtrrs (atts);

  if (project != nullptr)
  {
    // The project was read without any problem.
    project->setLocation (projLocation);
    project->setProjectType (type);

    PluginControl::instance ()->launchProject (project);
    project->setLocation (projLocation);

    _openProjects[projLocation] = project;
  }
  else
    qCDebug (CPR_CORE) << tr ("Project could not be open!");

  emit endOpenProject (projLocation);

  QCoreApplication::processEvents ();

  if (project != nullptr)
  {
    IDocumentParser *parser;
    parser = profile->createParser (project);
    PluginControl::instance ()->connectParser (
        parser, PluginControl::instance ()->getMessageControl (project));

    QFile input (docLocation);
    if (input.open (QIODevice::ReadOnly))
      parser->parseContent (input.readAll ());
    else
      qCDebug (CPR_CORE) << tr ("File could not be open!");

    input.close ();
  }

  project->setDirty (false);

  /* PluginControl::instance()->getMessageControl(project)
     ->setCurrentProjectAsDirty(); */
}

void
ProjectControl::saveProject (const QString &location)
{
  Project *project = _openProjects.value (location);
  QFile fout (location);

  if (!fout.exists ())
  {
    qCDebug (CPR_CORE) << "The file (" << location << ") doesn't exists. \
                           It will be created.";
  }

  if (!fout.open (QIODevice::WriteOnly))
  {
    // It could not open
    qCDebug (CPR_CORE) << "Failed to open file (" << location
                       << ") for writing.";
    return;
  }

  QString content = project->toString ();
  fout.write (qCompress (content.toLatin1 (), content.size ()));
  //  fout.write (content.toLatin1 ());
  fout.close ();
  project->setDirty (false);
}

void
ProjectControl::moveProject (const QString &location, const QString &dest,
                             bool saveDest)
{
  QFileInfo fileInfo (dest);
  if (fileInfo.absoluteDir ().exists ())
  {
    Project *project = _openProjects.value (location);
    project->setLocation (dest);
    _openProjects.insert (dest, project);
    _openProjects.remove (location); // remove de old

    if (saveDest)
      saveProject (dest);

    QMap<QString, QString> atts;
    QString documentId = dest;
    documentId = documentId.remove (0, location.lastIndexOf ("/") + 1);
    atts["id"] = documentId;
    project->setAtrrs (atts);
  }
  else
    qCWarning (CPR_CORE) << "Could not copy the current project from  "
                         << location << " to " << dest;
}

void
ProjectControl::saveTemporaryProject (const QString &location)
{
  Project *project = _openProjects.value (location);
  QFile fout (location + "~");

  qCDebug (CPR_CORE) << "Trying to autosave: " << location;
  if (!fout.exists ())
  {
    qCDebug (CPR_CORE) << "The file (" << location << ") doesn't exists.\
                            It will be created.";
  }

  if (!fout.open (QIODevice::WriteOnly))
  {
    // It could not open
    qCDebug (CPR_CORE) << "Failed to open file (" << location
                       << ") for writing.";
    return;
  }

  QString content = project->toString ();
  fout.write (qCompress (content.toLatin1 (), content.size ()));
  //  fout.write (content.toLatin1 ());
  fout.close ();
  //  project->setDirty(false);
}

Project *
ProjectControl::getOpenProject (const QString &location)
{
  if (_openProjects.contains (location))
    return _openProjects.value (location);

  return nullptr;
}

void
ProjectControl::projectIsDirty (bool isDirty)
{
  Project *project = qobject_cast<Project *> (QObject::sender ());
  if (project != nullptr)
  {
    emit dirtyProject (project->location (), isDirty);
  }
  else
    qCDebug (CPR_CORE)
        << "Received a dirtyProject message for a nullptr project";
}

CPR_CORE_END_NAMESPACE
