#include "attrinput.h"

#include <QHBoxLayout>

AttrInput::AttrInput(QString question, QString name, QString type, QString value, QWidget *parent)
    : QWidget (parent)
{
  // setStyleSheet("AttrInput { border: 1px solid black; }");

  _question = question;
  _name = name;
  _type = type;
  _value = value;
}

