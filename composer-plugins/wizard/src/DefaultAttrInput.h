#ifndef DEAFULTATTRINPUT_H
#define DEAFULTATTRINPUT_H

#include "attrinput.h"

#include <QLineEdit>
#include <QVBoxLayout>

class DefaultAttrInput : public AttrInput
{
  Q_OBJECT

private:
  QLineEdit *_answer;

public:
  explicit DefaultAttrInput(QString question, QString name,
                            QString type = "string", QString value = "",
                            QWidget * parent = 0);

  inline QString getAnswer () const { return _answer->text(); }
  inline void setAnswer (QString answer) { _answer->setText(answer); }
  inline AttrInput * clone() const
  {
    return new DefaultAttrInput (_question, _name, _type, _value,
                                 (QWidget *) parent ());
  }
};

#endif // DEAFULTATTRINPUT_H
