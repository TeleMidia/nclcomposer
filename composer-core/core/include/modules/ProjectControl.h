#ifndef DOCUMENTCONTROL_H
#define DOCUMENTCONTROL_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QTextStream>

#include <iostream>

#include "PluginControl.h"
#include "LanguageControl.h"
using namespace composer::core;

#include "../model/Project.h"
using namespace composer::core::model;

#include "../util/Singleton.h"
#include "../util/Utilities.h"
using namespace composer::core::util;

namespace composer {
    namespace core {

class ProjectControl : public QObject
{
    Q_OBJECT
    SINGLETON(ProjectControl)

private:
    ProjectControl();
    ~ProjectControl();
    QMap<QString, Project*> openProjects;

public:
    Project *getOpenProject(QString location);

 public slots:
    bool closeProject(QString location);
    void saveProject(QString location);
    void launchProject(QString location);

    void importFromDocument(QString docLocation, QString projLocation);

signals:
    void startOpenProject(QString document);
    void endOpenProject(QString document);
    void projectAlreadyOpen(QString);
    void notifyError(QString);
};

} } //end namespace


#endif // DOCUMENTCONTROL_H
