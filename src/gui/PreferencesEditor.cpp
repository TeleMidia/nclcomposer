#include "PreferencesEditor.h"
#include "ui_PreferencesEditor.h"

#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <util/Preferences.h>
using namespace cpr::core;

PreferencesEditor::PreferencesEditor(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PreferencesEditor)
{
  ui->setupUi(this);
  connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(saveChanges()));
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
  QStringList categories = Preferences::getInstance()->categories();
  QString category,key;

  while (ui->tableParameters->rowCount())
    ui->tableParameters->removeRow(0);

  while(!categories.isEmpty())
  {
    category = categories.takeFirst();
    QList<Preference *> preferences = Preferences::getInstance()->preferences(category);

    while(!preferences.isEmpty())
    {
      // qDebug() << preferences.takeFirst()->key() << "KEY";
      key = preferences.takeFirst()->key();

      ui->tableParameters->insertRow(i);
      QTableWidgetItem *item = new QTableWidgetItem();
      item->setText(key);
      item->setFlags(item->flags() ^ Qt::ItemIsEditable); //First column non-editable
      ui->tableParameters->setItem(i, 0, item);
      item = new QTableWidgetItem();

      if(Preferences::getInstance()->getValue(key)->value().toBool())
      {
        item->setText(Preferences::getInstance()->getValue(key)->value().toString());
      }
      ui->tableParameters->setItem(i, 1, item);
      i++;

    }
  }
  /*
  key = "cpr.textual.bgCaretLine";

  for (i=0 ; i < size; i++)
  {

    ui->tableParameters->insertRow(i);
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(key);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable); //First column non-editable
    ui->tableParameters->setItem(i, 0, item);
    item = new QTableWidgetItem();
    if(Preferences::getInstance()->getValue(key)->value().toBool())
    {
    item->setText(Preferences::getInstance()->getValue(key)->value().toString());
    }
    ui->tableParameters->setItem(i, 1, item);

  }*/
}

void PreferencesEditor::saveChanges()
{
  int rowcount = ui->tableParameters->rowCount();
  for(int i=0; i < rowcount; i++)
  {
    Preferences::getInstance()->setValue(ui->tableParameters->item(i,0)->text(),
                                         ui->tableParameters->item(i,1)->text());
  }
  Preferences::getInstance()->save();
}
