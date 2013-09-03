#include "comboattrinput.h"

#include <QVBoxLayout>
#include <QLabel>

ComboAttrInput::ComboAttrInput(QString question, QString name, QString type, QString value, QWidget *parent)
  : AttrInput(question, name, type, value, parent)
{
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QLabel *label = new QLabel (question);

  _combo = new QComboBox(this);

  mainLayout->addWidget(label);
  mainLayout->addWidget(_combo);

  QStringList items = value.split(",", QString::SkipEmptyParts);

  _combo->addItems(items);

  if (items.size())
    _combo->setCurrentText(items.at(0));

  setLayout(mainLayout);
}

