#ifndef PREFERENCESEDITOR_H
#define PREFERENCESEDITOR_H

#include <QDialog>

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

private:
  Ui::PreferencesEditor *ui;
};

#endif // PREFERENCESEDITOR_H
