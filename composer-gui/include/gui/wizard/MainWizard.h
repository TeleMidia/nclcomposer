#ifndef MAINWIZARD_H
#define MAINWIZARD_H

#include <QtDebug>

#include <QWizard>
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <QCoreApplication>
#include <QWidget>
#include <QVariant>
#include <QDir>

#include "IntroPage.h"
#include "EntryPage.h"

class MainWizard : public QWizard {
    Q_OBJECT
    private:
        QString name;
        QString location;
        QString projectId;
        IntroPage *intro;
        EntryPage *entry;
    public:
        MainWizard(QWidget* parent=0);
        ~MainWizard();
        void init();
        void accept();
        void inline setIsFile(bool isFile) {
            entry->isFile = isFile;
        }
        void inline setDefaultLocation(QString defaultLocation) {
            entry->defaultLocation = defaultLocation;
        }
        void inline setProjectId(QString projectId) {
            this->projectId = projectId;
        }

    signals:
        void infoReceived(QString,QString);
        void infoReceived(QString, QString,QString,bool);

};


#endif // MAINWIZARD_H
