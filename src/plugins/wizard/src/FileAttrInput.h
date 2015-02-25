#ifndef FILEATTRINPUT_H
#define FILEATTRINPUT_H

#include "AttrInput.h"
#include <QLineEdit>

class FileAttrInput : public AttrInput
{
  Q_OBJECT

public:
  explicit FileAttrInput(const QString &question, const QString &name,
                         const QString &type = "string",
                         const QString &value="", QWidget * parent = 0);

  QString getAnswer() const { return _answer->text(); }
  void setAnswer(const QString &answer) {_answer->setText(answer); }
  inline AttrInput * clone () const
  {
    return (new FileAttrInput (_question, _name, _type,
                               _value, (QWidget *) parent()));
  }

public slots:
  void searchFile ();

private:
  QLineEdit *_answer;
  static QString _lastPath;

};

#endif // FILEATTRINPUT_H
