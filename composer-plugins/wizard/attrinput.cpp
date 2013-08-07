#include "attrinput.h"

AttrInput::AttrInput(QString question, QString name, QString type, QString value, QWidget *parent)
    : QWidget (parent)
{
    _question = question;
    _name = name;
    _type = type;
    _value = value;
}
