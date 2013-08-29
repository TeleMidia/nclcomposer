#ifndef FILEATTRINPUT_H
#define FILEATTRINPUT_H

#include "attrinput.h"

#include <QLineEdit>

class FileAttrInput : public AttrInput
{
  Q_OBJECT

private:
  QLineEdit *_answer;

public:
  explicit FileAttrInput(QString question, QString name, QString type = "string", QString value = "", QWidget * parent = 0);

  QString answer() const { return _answer->text(); }
  void setAnswer(QString answer) {_answer->setText(answer); }
  inline AttrInput * clone () const{ return (new FileAttrInput (_question, _name, _type, _value, (QWidget *) parent())); }

public slots:
  void searchFile ();
};

#endif // FILEATTRINPUT_H
