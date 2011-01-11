#ifndef PROJECTWIZARD_H
#define PROJECTWIZARD_H

#include <QWizard>
#include <QString>
#include <QVariant>
#include <QDebug>

#include "ProjectEntryPage.h"

class ProjectWizard : public QWizard
{
    Q_OBJECT
    public:
        ProjectWizard(QWidget* parent=0);
        ~ProjectWizard();
        void init();
        void accept();

    private:
        ProjectEntryPage *entryPage;
        QString name;
        QString location;
    signals:
        void infoReceived(QString,QString);



};

#endif // PROJECTWIZARD_H
