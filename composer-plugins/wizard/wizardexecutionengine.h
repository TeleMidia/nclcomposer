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
    explicit WizardExecutionEngine(const QString &wsPath = "", QObject *parent  = 0);

    void setWS (const QString&);
    void setInputFile (const QString&);

    void run ();

public slots:
    void createFinalApplication ();
};

#endif // WIZARDEXECUTIONENGINE_H
