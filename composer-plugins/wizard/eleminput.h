#ifndef ELEMINPUT_H
#define ELEMINPUT_H

#include <QGroupBox>

#include "attrinput.h"
#include <QVector>
#include <QVBoxLayout>
#include <QDomElement>
#include <QPushButton>

class ElemInput : public QFrame
{
    Q_OBJECT

private:
    QVector <AttrInput *> _attrInputs;
    QVector <ElemInput *> _elemInputs;
    QVBoxLayout * _elemInputLayout;
    QString _question;
    QString _selector;
    QString _id;

    QPushButton *_optionsButton;

public:
    explicit ElemInput(QString selector, QString question, QWidget *parent = 0);

    void addAttrInput (QString question, QString name, QString type = "string", QString value = "");
    void addElemInput (QDomElement & elemInputElement);

    inline void addAttrInput (AttrInput *attrInput ) {
        if(attrInput){
            _attrInputs.append(attrInput);
            _elemInputLayout->addWidget(attrInput);

            attrInput->setStyleSheet("QGroupBox { border: 2px solid gray; }");
        }
    }
    inline void setId (QString id) { _id = id; }
    inline void setSelector (QString selector) { _selector = selector; }
    inline QVector <ElemInput *> elemInputs () const { return _elemInputs; }
    inline QVector <AttrInput*> attrInputs () const { return _attrInputs; }
    inline QString question () const { return _question; }
    inline QString selector () const { return _selector; }
    inline QString id () const { return _id; }

    ~ElemInput ();

signals:
    void cloned (ElemInput *);
    void removeRequested (ElemInput *);

public slots:
    void menuSelected (QAction *);
    ElemInput * clone ();

    inline void notifyRemove () {
        emit removeRequested(this);
    }

    inline void addElemInput (ElemInput *elemInput){
        if (elemInput){
            connect (elemInput, SIGNAL(cloned(ElemInput*)), this, SLOT (addElemInput(ElemInput*)));
            _elemInputs.append(elemInput);
            _elemInputLayout->addWidget(elemInput);
        }
    }

};

#endif // ELEMINPUT_H
