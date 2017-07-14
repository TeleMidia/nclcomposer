#ifndef PREFERENCESEDITOR_H
#define PREFERENCESEDITOR_H

#include <QWidget>

namespace Ui {
  class PreferencesEditor;
}

class PreferencesEditor : public QWidget
{
  Q_OBJECT

public:
  explicit PreferencesEditor(QWidget *parent = 0);
  ~PreferencesEditor();

public slots:
  void show();
private slots:
  void saveChanges();

private:
  Ui::PreferencesEditor *ui;
  void tableFillFromIni();
};

#endif // PREFERENCESEDITOR_H
