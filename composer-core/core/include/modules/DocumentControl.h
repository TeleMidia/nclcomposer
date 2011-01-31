#ifndef DOCUMENTCONTROL_H
#define DOCUMENTCONTROL_H

#include <QObject>
#include <QProcess>

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
                    DocumentControl();
                    ~DocumentControl();
                    QMap<QString, Document*> openDocuments;

                signals:
                    void openDocumentTab(QString);
                    void notifyError(QString);
                public slots:
                    bool closeDocument(QString location);
                    void launchDocument(QString projectId, QString location);
                };

                }
        }
}


#endif // DOCUMENTCONTROL_H
