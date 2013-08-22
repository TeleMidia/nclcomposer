#include "eleminput.h"

#include <QPushButton>
#include <QMenu>
#include <QAction>

#include "defaultattrinput.h"
#include "fileattrinput.h"
#include "constants.h"

ElemInput::ElemInput (QString selector, QString title, QWidget *parent)
    : QFrame(parent)
{
    setFrameShape( QFrame::Box );

    _selector = selector;
    _question = title;
    _elemInputLayout = new QVBoxLayout;

    _optionsButton = new QPushButton(this);

    QMenu *menu = new QMenu(this);
    menu->addAction(ADD_LABEL);
    menu->addAction(REMOVE_LABEL);

    _optionsButton->setMenu(menu);
    QIcon optionsIcon (":/images/down.png");
    _optionsButton->setIcon(optionsIcon);

    _optionsButton->setFixedWidth(20);
    _optionsButton->setFixedHeight(20);

    connect (menu, SIGNAL(triggered(QAction*)), this, SLOT(menuSelected(QAction*)));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(_elemInputLayout);

    setLayout(mainLayout);
}

void ElemInput::menuSelected(QAction *action)
{
    if (action->text() == ADD_LABEL)
        clone ();
    else if (action->text() == REMOVE_LABEL)
        notifyRemove();
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
    ElemInput *clone = new ElemInput (_selector, _question, (QWidget *) parent());

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

ElemInput::~ElemInput()
{

}
