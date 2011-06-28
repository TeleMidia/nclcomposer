#include "modules/ProjectControl.h"

namespace composer {
    namespace core {

INIT_SINGLETON (ProjectControl)

ProjectControl::ProjectControl()
{
}

ProjectControl::~ProjectControl()
{
    QMap<QString,Project*>::iterator it;
    PluginControl *pg = PluginControl::getInstance();
    for (it = openProjects.begin(); it != openProjects.end(); it++)
    {
        Project *project = it.value();
        if (pg->releasePlugins(project))
        {
            delete project;
            project = NULL;
        } else {
            qDebug() << "Error: Failed to releasePlugins ";
        }
    }
}

bool ProjectControl::closeProject(QString location)
{
    if (!openProjects.contains(location)) return false;

    Project *project = openProjects[location];
    if (PluginControl::getInstance()->releasePlugins(project))
    {
        delete project;
        project = NULL;
        openProjects.remove(location);
    } else {
        qDebug() << "Error: Failed to close the project :" << location;
        return false;
    }
    return true;
}

void ProjectControl::launchProject(QString location)
{
    if (openProjects.contains(location))
    {
        emit projectAlreadyOpenned(location);
        return;
    }

    QString ext = location;
    ext = ext.remove(0, ext.lastIndexOf(".") + 1);
    LanguageType type = Utilities::getLanguageTypeByExtension( ext );

    if(type == NONE)
    {
        //\todo TEST ON WINDOWS
        QProcess spaw;
        QStringList args;
        args.append(location);
#ifdef Q_WS_MAC
        spaw.startDetached("/usr/bin/open", args);
#else
        spaw.startDetached("/usr/bin/gnome-open", args);
#endif
        return;
    }

    /* Requests the LanguageProfile associated with this DocumentType */
    ILanguageProfile *profile = LanguageControl::getInstance()->
                                    getProfileFromType(type);

    if (!profile)
    {
        emit notifyError(tr("No Language Profile Extension "
                            "found for (%1)").
                         arg(ext.toUpper()));
        return;
    }

    emit startOpenProject(location);
    QMap<QString,QString> atts;
    QString documentId = location;
    documentId.remove(0, location.lastIndexOf(QDir::separator())+1);
    atts["id"] = documentId;

    /* create the NCLDocument */
    Project *project = new Project(atts);
    project->setLocation(location);
    project->setProjectType(type);
    // doc->setProjectId(projectId);

    PluginControl::getInstance()->launchProject(project);
    openProjects[location] = project;
    emit endOpenProject(location);
}

void ProjectControl::saveProject(QString location)
{
    Project *project = openProjects.value(location);
    QFile fout(location + ".cp");

    qDebug() << "Trying to save: " << location;
    if(!fout.exists())
    {
        qDebug() << "The file (" << location << ") doesn't exists. It will be\
                    created.";
    }

    if( !fout.open( QIODevice::WriteOnly ) )
    {
       // It could not open
       qDebug() << "Failed to open file (" <<  location << ")";
       return;
    }

    QTextStream stream( &fout );
    qDebug() << project->toString();
    stream << project->toString();
    fout.close();
}

} }//end namespace

