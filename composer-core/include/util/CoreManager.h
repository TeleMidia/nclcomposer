#ifndef COREMANAGER_H
#define COREMANAGER_H

#include <QDebug>

#include <QObject>
#include <QDir>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>

#include <QMessageBox>

#include "../modules/MessageControl.h"
#include "../modules/ProjectControl.h"
using namespace composer::core::module;

namespace composer {
namespace core {
namespace util {

class CoreManager : public QObject
{
Q_OBJECT
public:
    explicit CoreManager(QObject *parent = 0);
    ~CoreManager();

private:
    void addFilesInDirectory(QDir dir, QString projectId);

signals:
    void projectCreated(QString,QString);
    void documentAdded(QString,QString);
    void onError(QString);

public slots:
    void createProject(QString,QString);
    void addProject(QString,QString);
    void addDocument(QString,QString,QString,bool);
    bool saveSettings();

};

}
}
}
#endif // COREMANAGER_H
