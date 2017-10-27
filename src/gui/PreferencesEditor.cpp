#include "PreferencesEditor.h"
#include "ui_PreferencesEditor.h"

#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <util/Preferences.h>
using namespace cpr::core;

PreferencesEditor::PreferencesEditor(QWidget *parent) :
  QDialog (parent),
  ui(new Ui::PreferencesEditor)
{
  ui->setupUi(this);
  connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveChanges()));
}

PreferencesEditor::~PreferencesEditor()
{
  delete ui;
}

void PreferencesEditor::show()
{
  loadPreferences();
  QWidget::show();
}

void PreferencesEditor::loadPreferences()
{
  int i = 0;
  QStringList categories = Preferences::instance()->categories();
  QString category,key;

  while (ui->tableParameters->rowCount())
    ui->tableParameters->removeRow(0);

  while(!categories.isEmpty())
  {
    category = categories.takeFirst();
    QList<Preference *> preferences = Preferences::instance()->preferences(category);

    while(!preferences.isEmpty())
    {
      key = preferences.takeFirst()->key();

      ui->tableParameters->insertRow(i);
      QTableWidgetItem *item = new QTableWidgetItem();
      item->setText(key);
      item->setFlags(item->flags() ^ Qt::ItemIsEditable); //First column non-editable
      ui->tableParameters->setItem(i, 0, item);
      item = new QTableWidgetItem();

      QVariant value = Preferences::instance ()->getValue(key)->value();

      if (value.type() == QVariant::StringList)
        item->setText(value.toStringList().join(", "));
      else
        item->setText(value.toString());

      ui->tableParameters->setItem(i, 1, item);
      ++i;
    }
  }
}

void PreferencesEditor::saveChanges()
{
  for(int i = 0; i < ui->tableParameters->rowCount(); i++)
  {
    QString key = ui->tableParameters->item(i, 0)->text();
    QVariant value = Preferences::instance ()->getValue(key)->value();
    QString new_value= ui->tableParameters->item(i, 1)->text();

    if (value.type () == QVariant::StringList)
      Preferences::instance ()->setValue(key, new_value.replace(" ", "").split(","));
    else
      Preferences::instance ()->setValue(key, new_value);
  }

  Preferences::instance ()->save();
}
