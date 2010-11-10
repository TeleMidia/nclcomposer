#ifndef COREMANAGER_H
#define COREMANAGER_H

#include <QDebug>

#include <QObject>
#include <QDir>

#include <control/modules/ProjectControl.h>
using namespace composer::core::module;

class CoreManager : public QObject
{
Q_OBJECT
public:
    explicit CoreManager(QObject *parent = 0);
    ~CoreManager();
private:
    ProjectControl *projectControl;
    void addFilesInDirectory(QDir dir, QString projectId);

signals:
    void projectCreated(QString,QString);
    void documentAdded(QString,QString,QString);
    void onError(QString);

public slots:
    void createProject(QString,QString);
    void addProject(QString,QString);
    void addDocument(QString,QString,QString,bool);
    void createDocument(QString,QString);
    bool saveSettings();


};

#endif // COREMANAGER_H
