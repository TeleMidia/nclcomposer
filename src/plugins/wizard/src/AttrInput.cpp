#include "AttrInput.h"

#include <QHBoxLayout>

AttrInput::AttrInput(const QString &question, const QString &name,
                     const QString &type, const QString &value, QWidget *parent)
  : QWidget (parent)
{
  // setStyleSheet("AttrInput { border: 1px solid black; }");
  _question = question;
  _name = name;
  _type = type;
  _value = value;
}

