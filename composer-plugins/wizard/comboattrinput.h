#ifndef COMBOATTRINPUT_H
#define COMBOATTRINPUT_H

#include <QComboBox>

#include "attrinput.h"

class ComboAttrInput : public AttrInput
{
  Q_OBJECT

private:
  QComboBox * _combo;

public:
  explicit ComboAttrInput(QString question, QString name, QString type = "combo", QString value = "", QWidget * parent = 0);

  inline QString answer() const { return _combo->currentText(); }
  inline void setAnswer(QString answer) { _combo->setCurrentText(answer); }
  inline AttrInput * clone () const { return new ComboAttrInput (_question, _name, _type, _value, (QWidget *) parent()); }
};

#endif // COMBOATTRINPUT_H
