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

#include "../model/Document.h"
using namespace composer::core::model;

#include "../util/Singleton.h"
#include "../util/Utilities.h"
using namespace composer::core::util;

namespace composer {
    namespace core {

        class DocumentControl : public QObject
        {
            Q_OBJECT
            SINGLETON(DocumentControl)
        private:
                    DocumentControl();
            ~DocumentControl();
            QMap<QString, Document*> openDocuments;

        signals:
            void openDocumentTab(QString);
            void notifyError(QString);
            void beginOpenDocument();
            void endOpenDocument();

         public slots:
            bool closeDocument(QString location);
            void saveDocument(QString location);
            void launchDocument(QString projectId, QString location);
        };

} } //end namespace


#endif // DOCUMENTCONTROL_H
