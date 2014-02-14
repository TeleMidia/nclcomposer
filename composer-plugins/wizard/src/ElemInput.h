#ifndef ELEMINPUT_H
#define ELEMINPUT_H

#include <QGroupBox>

#include "AttrInput.h"
#include <QVector>
#include <QVBoxLayout>
#include <QDomElement>
#include <QPushButton>

class ElemInput : public QFrame
{
  Q_OBJECT

public:
  explicit ElemInput(const QString &getSelector, const QString &getQuestion,
                     QWidget *parent = 0);

  void addAttrInput (const QString &getQuestion, const QString &name,
                     const QString &type = "string", const QString &value = "");
  void addElemInput (const QDomElement & elemInputElement);

  inline void addAttrInput (AttrInput *attrInput )
  {
    if(attrInput)
    {
      _attrInputs.append(attrInput);
      _elemInputLayout->addWidget(attrInput);

      attrInput->setStyleSheet("QGroupBox { border: 2px solid gray; }");
    }
  }

  inline void setId (const QString &id) { _id = id; }
  inline void setSelector (const QString &selector) { _selector = selector; }

  inline QVector <ElemInput *> getElemInputs () const { return _elemInputs; }
  inline QVector <AttrInput*> getAttrInputs () const { return _attrInputs; }
  inline QString getQuestion () const { return _question; }
  inline QString getSelector () const { return _selector; }
  inline QString getId () const { return _id; }

  ~ElemInput ();

signals:
  void cloned (ElemInput *);
  void removeRequested (ElemInput *);

public slots:
  void menuSelected (QAction *);
  void removeElemInput (ElemInput *);
  ElemInput * clone ();

  inline void notifyRemove ()
  {
    emit removeRequested(this);
  }

  inline void addElemInput (ElemInput *elemInput)
  {
    if (elemInput)
    {
      connect (elemInput, SIGNAL(cloned(ElemInput*)),
               this, SLOT (addElemInput(ElemInput*)));
      connect (elemInput, SIGNAL(removeRequested(ElemInput*)),
               this, SLOT (removeElemInput(ElemInput*)));
      _elemInputs.append(elemInput);
      _elemInputLayout->addWidget(elemInput);
    }
  }

private:
  QVector <AttrInput *> _attrInputs;
  QVector <ElemInput *> _elemInputs;
  QVBoxLayout * _elemInputLayout;
  QString _question;
  QString _selector;
  QString _id;
  QPushButton *_optionsButton;
};

#endif // ELEMINPUT_H
