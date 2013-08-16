#include "eleminput.h"

#include <QPushButton>

#include "defaultattrinput.h"
#include "fileattrinput.h"

ElemInput::ElemInput (QString selector, QString title, QWidget *parent)
    : QGroupBox(title, parent)
{
    _selector = selector;
    _title = title;
    _elemInputLayout = new QVBoxLayout;

    QPushButton *plusButton = new QPushButton;
    QIcon icon (":/images/plus.png");
    plusButton->setIcon(icon);

    connect (plusButton, SIGNAL(pressed()), this, SLOT(clone()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(_elemInputLayout);
    mainLayout->addWidget(plusButton);

    setLayout(mainLayout);
}

void ElemInput::addAttrInput(QString question, QString name, QString type, QString value)
{
    AttrInput *attrInput = 0;

    if (type == "file")
        attrInput = new FileAttrInput (question, name, type, value);
    else
        attrInput = new DefaultAttrInput(question, name, type, value);

    addAttrInput(attrInput);
}

ElemInput *ElemInput::clone()
{
    ElemInput *clone = new ElemInput (_selector, _title, (QWidget *) parent());

    foreach (AttrInput *attrInput, _attrInputs)
        clone->addAttrInput(attrInput->clone());

    foreach (ElemInput *elemInput, _elemInputs){
        disconnect (elemInput, SIGNAL(cloned(ElemInput*)), this, SLOT (addElemInput(ElemInput*)));
        clone->addElemInput(elemInput->clone());
        connect (elemInput, SIGNAL(cloned(ElemInput*)), this, SLOT (addElemInput(ElemInput*)));
    }

    emit cloned(clone);

    return (clone);
}

void ElemInput::addElemInput(QDomElement &elemInputElement)
{
    QString elemInputText = "";
    QDomNodeList childNodes = elemInputElement.childNodes();
    for (int i = 0; i < childNodes.size(); i++){
        QDomNode node = childNodes.at(i);
        if (node.isText()){
            elemInputText += node.toText().data();
        }
    }

    QString elemInputSelector = elemInputElement.attribute("selector");

    ElemInput *elemInput = new ElemInput (elemInputSelector, elemInputText);


    QDomElement attrInput = elemInputElement.firstChildElement("attrInput");
    while (!attrInput.isNull()){
        QString attrInputQuestion = attrInput.text();
        QString attrInputName = attrInput.attribute("name");
        QString attrInputType = attrInput.attribute("type");
        QString attrInputValue = attrInput.attribute("value");

        elemInput->addAttrInput(attrInputQuestion, attrInputName, attrInputType, attrInputValue);

        attrInput = attrInput.nextSiblingElement("attrInput");
    }

    addElemInput(elemInput);

    QDomElement elemInputRecursiveElement = elemInputElement.firstChildElement("elemInput");
    while (!elemInputRecursiveElement.isNull()){
        elemInput->addElemInput(elemInputRecursiveElement);

        elemInputRecursiveElement = elemInputRecursiveElement.nextSiblingElement("elemInput");
    }
}
