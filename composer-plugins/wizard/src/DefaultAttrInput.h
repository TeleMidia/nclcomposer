#ifndef DEAFULTATTRINPUT_H
#define DEAFULTATTRINPUT_H

#include "AttrInput.h"

#include <QLineEdit>
#include <QVBoxLayout>

class DefaultAttrInput : public AttrInput
{
  Q_OBJECT

public:
  explicit DefaultAttrInput(const QString &question, const QString &name,
                            const QString &type = "string",
                            const QString &value = "", QWidget * parent = 0);

  inline QString getAnswer () const { return _answer->text(); }
  inline void setAnswer (const QString &answer) { _answer->setText(answer); }
  inline AttrInput * clone() const
  {
    return new DefaultAttrInput (_question, _name, _type, _value,
                                 (QWidget *) parent ());
  }

private:
  QLineEdit *_answer;

};

#endif // DEAFULTATTRINPUT_H
