#ifndef WIZARDEXECUTIONENGINE_H
#define WIZARDEXECUTIONENGINE_H

#include "Constants.h"
#include <QWizard>
#include <QDomDocument>
#include <QTreeWidget>
#include <QProgressBar>

class WizardExecutionEngine : public QObject
{
  Q_OBJECT
public:
  explicit WizardExecutionEngine(const QString &wsPath = "",
                                 QObject *parent  = 0);

  void setWS (const QString&);
  void setInputFile (const QString&);

  void run ();

public slots:
  void createFinalApplication ();
  void updateSelection (int);
  void updateCurrentPage();

private:
  void removeUuid(QDomElement& rootElement);
  QWizard _wizard;

  QTreeWidget *_treeView;

  QString _wsPath;
  QString _inputFile;
  QProgressBar *_progressBar;
};

#endif // WIZARDEXECUTIONENGINE_H
