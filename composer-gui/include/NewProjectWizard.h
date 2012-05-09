#ifndef NEWPROJECTWIZARD_H
#define NEWPROJECTWIZARD_H

#include <QWizard>

namespace Ui {
class NewProjectWizard;
}

class NewProjectWizard : public QWizard
{
  Q_OBJECT
  
public:
  explicit NewProjectWizard(QWidget *parent = 0);
  ~NewProjectWizard();

  QString getProjectFullPath();
  bool shouldCopyDefaultConnBase();
  
private:
  Ui::NewProjectWizard *ui;

private slots:
  void browseDir();

};

#endif // NEWPROJECTWIZARD_H
