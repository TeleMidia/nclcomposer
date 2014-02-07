#include "DefaultAttrInput.h"

#include <QLabel>

DefaultAttrInput::DefaultAttrInput (QString question, QString name, QString type, QString value, QWidget * parent)
    : AttrInput (question, name, type, value, parent)
{
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QLabel *label = new QLabel (question);
  _answer = new QLineEdit;

  mainLayout->addWidget(label);
  mainLayout->addWidget(_answer);

  setLayout(mainLayout);
}
