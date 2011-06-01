#ifndef DocumentWizard_H
#define DocumentWizard_H

#include <QWizard>
#include <QString>
#include <QVariant>
#include <QDebug>

#include "DocumentEntryPage.h"
namespace composer {
    namespace gui {

class DocumentWizard : public QWizard
{
    Q_OBJECT
    public:
        DocumentWizard(QWidget* parent=0);
        ~DocumentWizard();
        void init();
        void accept();
        inline void setProjectId(QString projectId) {
               this->projectId = projectId;
        }
    private:
        DocumentEntryPage *entryPage;
        QString projectId;
    signals:
        void infoReceived(QString, QString,QString,bool);



};

}} //end namespace
#endif // DocumentWizard_H
