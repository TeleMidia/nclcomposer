#ifndef WIZARDCHOOSER_H
#define WIZARDCHOOSER_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui
{
  class wizardchooser;
}

class WizardChooser : public QDialog
{
  Q_OBJECT
    
public:
  explicit WizardChooser(QWidget *parent = 0, bool modal = true);
  ~WizardChooser();

  QString getSelectedPath();

public Q_SLOTS:
  virtual int exec(const QString &_path);
    
private slots:
  void changeDescView(const QString&);

private:
  Ui::wizardchooser *_ui;
  QString _path;
};

#endif // WIZARDCHOOSER_H
