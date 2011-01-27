#ifndef DOCUMENTCONTROL_H
#define DOCUMENTCONTROL_H

#include <QObject>
#include <QtGui/QDesktopServices>
#include <QUrl>

#include "PluginControl.h"
#include "LanguageControl.h"
using namespace composer::core::module;

#include "../model/Document.h"
using namespace composer::core::model;

#include "../util/Singleton.h"
#include "../util/Utilities.h"
using namespace composer::core::util;

namespace composer{
    namespace core {
            namespace module {
                class DocumentControl : public QObject,
                                        public Singleton<DocumentControl>
                {
                    Q_OBJECT
                    friend class Singleton<DocumentControl>;
                private:
                    explicit DocumentControl(QObject *parent = 0);
                    ~DocumentControl();
                    QMap<QString, Document*> openDocuments;

                signals:
                    void openDocumentTab(QString);
                    void notifyError(QString);
                public slots:
                    void closeDocument(QString location);
                    void launchDocument(QString projectId, QString location);
                    //void closeDocument(Document *doc) {}

                };

                }
        }
}


#endif // DOCUMENTCONTROL_H
