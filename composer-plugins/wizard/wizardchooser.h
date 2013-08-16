#ifndef WIZARDCHOOSER_H
#define WIZARDCHOOSER_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class wizardchooser;
}

class wizardchooser : public QDialog
{
  Q_OBJECT
    
public:
  explicit wizardchooser(QWidget *parent = 0, bool modal = true);
  ~wizardchooser();

  QString getSelectedPath();

public Q_SLOTS:
  virtual int exec(const QString &path);
    
private:
  Ui::wizardchooser *ui;
  QString path;

private slots:
  void changeDescView(QString);
};

#endif // WIZARDCHOOSER_H
