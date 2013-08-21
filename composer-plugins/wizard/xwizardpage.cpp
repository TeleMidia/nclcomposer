#include "xwizardpage.h"

#include <QScrollArea>
#include <QScrollBar>

#include "selectsparser.h"

#include <QDebug>

XWizardPage::XWizardPage(QWidget *parent) :
    QWizardPage(parent)
{
    QScrollArea *widgetScrollable = new QScrollArea;
    widgetScrollable->setWidgetResizable(true);

    QScrollBar *scrollBar = widgetScrollable->verticalScrollBar();
    scrollBar->setSliderDown(true);
    scrollBar->setValue(0);

    widgetScrollable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    widgetScrollable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QWidget *containerWidget = new QWidget;
    widgetScrollable->setWidget(containerWidget);

    _containerLayout = new QVBoxLayout;
    containerWidget->setLayout(_containerLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(widgetScrollable);

    setLayout(mainLayout);
}

ElemInput *XWizardPage::addElemInput(QString id, QString elemInputSelector, QString title)
{
    ElemInput *elemInput = 0;
    if (id != ""){
        elemInput = new ElemInput (elemInputSelector, title, this);
        elemInput->setId(id);
        addElemInput(elemInput);
    }

    return elemInput;
}

void XWizardPage::addElemInput(ElemInput *elemInput)
{
    _elemInputs.append(elemInput);

    connect(elemInput, SIGNAL(cloned(ElemInput*)), this, SLOT(cloneElemInput(ElemInput *)));
    connect(elemInput, SIGNAL(removeRequested(ElemInput*)), this, SLOT(removeElemInput(ElemInput*)));

    _containerLayout->addWidget(elemInput);
}

void XWizardPage::addAttrInput(ElemInput *elemInput, QString question, QString name, QString type, QString value)
{
    if (elemInput){
        elemInput->addAttrInput(question, name, type, value);
    }
}

void XWizardPage::resolve(ElemInput *elemInput, QDomElement &finalAppRootElement,
                          QDomElement &pdpRootElement, QSet<QString>& elementsMarked)
{
    QString selector = elemInput->selector();
    SelectsParser selectorParser (selector);

    QDomElement elementTarget;

    QString elementToSearch = selectorParser.elementTagname();
    QString elementAttributeName = selectorParser.elementAttributeName();
    QString elementAttributeValue = selectorParser.elementAttributeValue();

    QString parentTagname = selectorParser.parentTagname();
    QString parentAttributeName = selectorParser.parentAttributeName();
    QString parentAttributeValue = selectorParser.parentAttributeValue();

    QVector <QDomElement> elements;
    if (selectorParser.hasParentDependency()){
        elements = searchElement(parentTagname, parentAttributeName, parentAttributeValue, finalAppRootElement);
        QDomElement parentElement;
        if (!elements.empty())
             parentElement = elements.front();
        if (!parentElement.isNull()){
            elements = searchElement(elementToSearch, elementAttributeName, elementAttributeValue, parentElement);

            if (!elements.empty())
                elementTarget = elements.front();
        }
    }
    else{
        elements = searchElement(elementToSearch, elementAttributeName, elementAttributeValue, finalAppRootElement);
        if (!elements.isEmpty())
            elementTarget = elements.front();
    }

    if (elementTarget.isNull()) return;

    QString elementTargetUuid = elementTarget.attribute(TEMP_ATTR);

    if (elementsMarked.contains(elementTargetUuid)){
        QDomElement toClone;

        if (selectorParser.hasParentDependency()){
            elements = searchElement(parentTagname, parentAttributeName, parentAttributeValue, pdpRootElement);
            QDomElement parentElement;
            if (!elements.isEmpty())
                parentElement = elements.front();

            if (!parentElement.isNull()){
                elements = searchElement(elementToSearch, elementAttributeName, elementAttributeValue, parentElement);

                if (!elements.isEmpty())
                    toClone = elements.front();
            }
        }
        else{
            elements = searchElement(elementToSearch, elementAttributeName, elementAttributeValue, pdpRootElement);
            if (!elements.isEmpty())
                toClone = elements.front();
        }

        if (!toClone.isNull()){
            QDomElement parentElement;

            parentElement = elementTarget.parentNode().toElement();
            elementTarget = toClone.cloneNode(true).toElement();
            createRandomIds (elementTarget);

            parentElement.appendChild(elementTarget);
        }
    }

    if (!elementTarget.isNull()){
        elementTargetUuid = elementTarget.attribute(TEMP_ATTR);
        elementsMarked.insert(elementTargetUuid);

        QVector <AttrInput*> attrInputs = elemInput->attrInputs();
        for (int i = 0; i < attrInputs.size(); i++){
            AttrInput * attrInput = attrInputs.at(i);
            QString attributeName = attrInput->name();
            QString attributeValue = attrInput->answer();

            elementTarget.setAttribute(attributeName, attributeValue);
        }

        QString prefixId = elementTarget.tagName().mid(0, 1);
        if(_elemPrefixIdCount.count(prefixId))
          _elemPrefixIdCount[prefixId] += 1;
        else
          _elemPrefixIdCount[prefixId] = 1;

        elementTarget.setAttribute("id",  prefixId + "_" + QString::number(_elemPrefixIdCount[prefixId]));
    }

    foreach (ElemInput *childElemInput, elemInput->elemInputs()){
        resolve (childElemInput, elementTarget, pdpRootElement, elementsMarked);
    }
}

bool XWizardPage::computeAnswers(QDomElement& finalAppRootElement, QDomElement& pdpRootElement,
                                 QSet<QString> &selectorsUsed)
{
    bool answer = true;

    for (int i = 0; i < _elemInputs.size(); i++) {
        ElemInput *elemInput = _elemInputs.at(i);

        resolve(elemInput, finalAppRootElement, pdpRootElement, selectorsUsed);
    }

    return answer;
}

QVector <QDomElement> XWizardPage::searchElement(QString tagname, QString attributeName,
                                       QString attributeValue, QDomElement &rootElement)
{
    QVector<QDomElement> elementsToReturn;

    QDomNodeList elements;
    if (tagname == "*"){
        //recursive search
    }
    else {
        QString rootTagname = rootElement.tagName();
        if (rootTagname == tagname){
            QString rootAttributeValue = rootElement.attribute(attributeName);
            if (rootAttributeValue == attributeValue){
                elementsToReturn.append(rootElement);
            }
        }

        elements = rootElement.elementsByTagName(tagname);
        for (int i = 0; i < elements.size(); i++){
            QDomElement element = elements.at(i).toElement();
            QString value = element.attribute(attributeName, "");

            if (value == attributeValue) {
                elementsToReturn.append(element);
            }
        }

    }

    return elementsToReturn;
}

QDomElement XWizardPage::searchParent(QString tagname, QString attributeName,
                                      QString attributeValue, QDomElement &rootElement)
{
    QDomElement elementToReturn;

    QDomNodeList elements;
    if (tagname == "*"){
        //recursive search
    }
    else {
        elements = rootElement.elementsByTagName(tagname);
        for (int i = 0; i < elements.size(); i++){
            QDomElement element = elements.at(i).toElement();
            QString value = element.attribute(attributeName, "");

            if (value == attributeValue) {
                elementToReturn = element;
                break;
            }
        }
    }

    return elementToReturn;
}

void XWizardPage::cloneElemInput(ElemInput *elemInput)
{
    if (elemInput){
        addElemInput(elemInput);
    }
}

void XWizardPage::removeElemInput(ElemInput *elemInput)
{
    if (elemInput){
        int count = _containerLayout->count();
        for (int i = 0; i < count; i++){
            QLayoutItem *item = _containerLayout->itemAt(i);
            if (item && item->widget() == elemInput){
                _containerLayout->removeItem(item);
                delete item->widget();
                delete item;
                break;
            }
        }
    }
}

bool XWizardPage::validatePage()
{
    return true;
}

