#ifndef COREMANAGER_H
#define COREMANAGER_H

#include <QDebug>

#include <QObject>
#include <QDir>

#include <control/manager/ProjectControl.h>
using namespace manager;

class CoreManager : public QObject
{
Q_OBJECT
public:
    explicit CoreManager(QObject *parent = 0);
    ~CoreManager();
private:
    ProjectControl *projectControl;

signals:
    void projectCreated(QString,QString);
    void onError(QString);

public slots:
    void createProject(QString,QString);
    void addDocument(QString,QString,QString);
    void createDocument(QString,QString);

};

#endif // COREMANAGER_H
