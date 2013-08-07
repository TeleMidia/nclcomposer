#ifndef WIZARDEXECUTIONENGINE_H
#define WIZARDEXECUTIONENGINE_H

#include "constants.h"
#include <QWizard>
#include <QDomDocument>

class WizardExecutionEngine : public QObject
{
    Q_OBJECT

private:
    QWizard _wizard;
    QString _wsPath;
    QString _inputFile;

    void removeUuid(QDomElement& rootElement);

public:
    WizardExecutionEngine(QString);

    void setWS (QString);

    void run ();

public slots:
    void createFinalApplication ();
};

#endif // WIZARDEXECUTIONENGINE_H
