#ifndef PREFERENCESEDITOR_H
#define PREFERENCESEDITOR_H

#include <QDialog>
#include <QWidget>

namespace Ui
{
class PreferencesEditor;
}

class PreferencesEditor : public QDialog
{
  Q_OBJECT

public:
  explicit PreferencesEditor (QWidget *parent = 0);
  ~PreferencesEditor ();

public slots:
  void show ();
private slots:
  void saveChanges ();

private:
  Ui::PreferencesEditor *ui;
  void loadPreferences ();
};

#endif // PREFERENCESEDITOR_H
