#include "ComboAttrInput.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QInputDialog>

ComboAttrInput::ComboAttrInput(QString question, QString name, QString type,
                               QString value, QWidget *parent)
  : AttrInput(question, name, type, value, parent)
{
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QLabel *label = new QLabel (question);

  _combo = new QComboBox(this);

  mainLayout->addWidget(label);
  mainLayout->addWidget(_combo);

  QStringList items = value.split(",", QString::SkipEmptyParts);

  foreach (QString item , items){
    _combo->addItem(item.trimmed());
  }

  _combo->insertSeparator(_combo->count());
  _combo->addItem("Other...");

  if (items.size())
    _combo->setEditText(items.at(0));

  connect (_combo, SIGNAL(currentIndexChanged(int)), this,
           SLOT(changeSelection(int)));

  setLayout(mainLayout);
}

void ComboAttrInput::changeSelection(int currentIndex)
{
  if (currentIndex == _combo->count() - 1){
    bool ok;
    QString text = QInputDialog::getText(this, tr("Other..."),
                                         tr("Type a new value:"), QLineEdit::Normal,
                                         "Other", &ok);
    if (ok && !text.isEmpty()){
      disconnect (_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSelection(int)));
      _combo->insertItem(0, text);
      _combo->setCurrentIndex(0);
      connect (_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSelection(int)));
    }
  }
}

