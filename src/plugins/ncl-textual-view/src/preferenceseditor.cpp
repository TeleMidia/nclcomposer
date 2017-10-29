#include "preferenceseditor.h"
#include "ui_preferenceseditor.h"

PreferencesEditor::PreferencesEditor (QWidget *parent)
    : QDialog (parent), ui (new Ui::PreferencesEditor)
{
  ui->setupUi (this);
}

PreferencesEditor::~PreferencesEditor () { delete ui; }
