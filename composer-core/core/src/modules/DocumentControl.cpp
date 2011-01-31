#include "modules/DocumentControl.h"

namespace composer {
    namespace core {
            namespace module {

DocumentControl::DocumentControl(QObject *parent) :
QObject(parent)
{
}

DocumentControl::~DocumentControl()
{
    QMap<QString,Document*>::iterator it;
    PluginControl *pg = PluginControl::getInstance();
    for (it = openDocuments.begin(); it != openDocuments.end(); it++)
    {
        Document *doc = it.value();
        if (pg->releasePlugins(doc))
        {
            delete doc;
            doc = NULL;
        } else {
            qDebug() << "Error: Failed to releasePlugins ";
        }
    }
}

bool DocumentControl::closeDocument(QString location)
{
    if (!openDocuments.contains(location)) return false;

    Document *doc = openDocuments[location];
    if (PluginControl::getInstance()->releasePlugins(doc))
    {
        delete doc;
        doc = NULL;
        openDocuments.remove(location);
    } else {
        qDebug() << "Error: Failed to close document (" << location << " )";
        return false;
    }
    return true;
}

void DocumentControl::launchDocument(QString projectId, QString location)
{

    if (openDocuments.contains(location))
    {
        emit openDocumentTab(location);
        return;
    }

    QString ext = location;
    ext = ext.remove(0,ext.lastIndexOf(".")+1);
    LanguageType type = Utilities::getLanguageTypeByExtension(ext);

    if(type == NONE) {
        //TEST: WINDOWS
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

    /*Requests the LanguageProfile associated with this DocumentType */
    ILanguageProfile *profile = LanguageControl::getInstance()->
                                getProfileFromType(type);
    if (!profile) {
        emit notifyError(tr("No Language Profile Extension "
                            "found for (%1)").
                         arg(ext.toUpper()));
        return;
    }

    QMap<QString,QString> atts;
    QString documentId = location;
    documentId.remove(0,location.lastIndexOf(QDir::separator())+1);
    atts["id"] = documentId;

    /* create the NCLDocument */
    Document *doc = new Document(atts);
    doc->setLocation(location);
    doc->setDocumentType(type);
    doc->setProjectId(projectId);

    PluginControl::getInstance()->launchDocument(doc);
    openDocuments[location] = doc;

}
                }
        }
}//end namespaec composer

