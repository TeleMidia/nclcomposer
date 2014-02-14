#ifndef COMBOATTRINPUT_H
#define COMBOATTRINPUT_H

#include <QComboBox>

#include "AttrInput.h"

class ComboAttrInput : public AttrInput
{
  Q_OBJECT

public:
  explicit ComboAttrInput(const QString &question, const QString &name,
                          const QString &type = "combo",
                          const QString &value = "", QWidget * parent = 0);

  inline QString getAnswer() const { return _combo->currentText(); }
  inline void setAnswer(const QString &answer) { _combo->setEditText(answer); }
  inline AttrInput * clone () const
  {
    return new ComboAttrInput (_question, _name, _type, _value,
                               (QWidget *) parent());
  }

public slots:
  void changeSelection (int);

private:
  QComboBox * _combo;

};

#endif // COMBOATTRINPUT_H
