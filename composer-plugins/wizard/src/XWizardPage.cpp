#include "xwizardpage.h"

#include <QScrollArea>
#include <QScrollBar>
#include <QWebView>
#include <QMessageBox>
#include <QLabel>

#include "SelectsElement.h"

#include <QDebug>

QMap <QString, int> XWizardPage::_elemPrefixIdCount;

XWizardPage::XWizardPage(const QString &title,
                         const QString &text,
                         QWidget *parent)
  : QWizardPage(parent)
{
  QScrollArea *widgetScrollable = new QScrollArea;
  widgetScrollable->setWidgetResizable(true);

  QScrollBar *scrollBar = widgetScrollable->verticalScrollBar();
  scrollBar->setSliderDown(true);
  scrollBar->setValue(0);

  widgetScrollable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  widgetScrollable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  QWidget *containerWidget = new QWidget;
  widgetScrollable->setWidget(containerWidget);

  _containerLayout = new QFormLayout;
  containerWidget->setLayout(_containerLayout);

  _containerLayout->addWidget(new QLabel(text));

  setTitle(title);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(widgetScrollable);

  setLayout(mainLayout);
}

ElemInput *XWizardPage::addElemInput( const QString &id,
                                      const QString &elemInputSelector,
                                      const QString &title )
{
  ElemInput *elemInput = 0;
  if (id != "")
  {
    elemInput = new ElemInput (elemInputSelector, title, this);
    elemInput->setId(id);

    addElemInput(elemInput);
  }

  return elemInput;
}

void XWizardPage::createConnections (ElemInput *elemInput)
{
  connect( elemInput,
           SIGNAL(cloned(ElemInput*)),
           this,
           SLOT(cloneElemInput(ElemInput *)) );

  connect( elemInput,
           SIGNAL(removeRequested(ElemInput*)),
           this, SLOT(removeElemInput(ElemInput*)) );
}

void XWizardPage::addElemInput(ElemInput *elemInput)
{
  _elemInputs.append(elemInput);

  createConnections(elemInput);

  _containerLayout->addWidget(elemInput);
}


void XWizardPage::addAttrInput( ElemInput *elemInput,
                                const QString &question,
                                const QString &name,
                                const QString &type,
                                const QString &value)
{
  if (elemInput)
    elemInput->addAttrInput(question, name, type, value);
}

void XWizardPage::resolve( ElemInput *elemInput,
                           QDomElement &finalAppRootElement,
                           QDomElement &pdpRootElement,
                           QSet<QString>& elementsMarked )
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
  if (selectorParser.hasParentDependency())
  {
    elements = searchElement( parentTagname,
                              parentAttributeName,
                              parentAttributeValue,
                              finalAppRootElement );
    QDomElement parentElement;
    if (!elements.empty())
      parentElement = elements.front();

    if (!parentElement.isNull())
    {
      elements = searchElement( elementToSearch,
                                elementAttributeName,
                                elementAttributeValue,
                                parentElement );

      if (!elements.empty())
        elementTarget = elements.front();
    }
  }
  else
  {
    elements = searchElement( elementToSearch,
                              elementAttributeName,
                              elementAttributeValue,
                              finalAppRootElement );

    if (!elements.isEmpty())
      elementTarget = elements.front();
  }

  if (elementTarget.isNull()) return;

  QString elementTargetUuid = elementTarget.attribute(TEMP_ATTR);

  if (elementsMarked.contains(elementTargetUuid))
  {
    QDomElement toClone;

    if (selectorParser.hasParentDependency())
    {
      elements = searchElement( parentTagname,
                                parentAttributeName,
                                parentAttributeValue,
                                pdpRootElement );

      QDomElement parentElement;
      if (!elements.isEmpty())
        parentElement = elements.front();

      if (!parentElement.isNull())
      {
        elements = searchElement( elementToSearch,
                                  elementAttributeName,
                                  elementAttributeValue,
                                  parentElement );

        if (!elements.isEmpty())
          toClone = elements.front();
      }
    }
    else
    {
      elements = searchElement( elementToSearch,
                                elementAttributeName,
                                elementAttributeValue,
                                pdpRootElement );

      if (!elements.isEmpty())
        toClone = elements.front();
    }

    if (!toClone.isNull())
    {
      QDomElement parentElement;

      parentElement = elementTarget.parentNode().toElement();
      elementTarget = toClone.cloneNode(true).toElement();
      createRandomIds (elementTarget);

      parentElement.appendChild(elementTarget);
    }
  }

  if (!elementTarget.isNull())
  {
    elementTargetUuid = elementTarget.attribute(TEMP_ATTR);
    elementsMarked.insert(elementTargetUuid);

    QVector <AttrInput*> attrInputs = elemInput->attrInputs();
    for (int i = 0; i < attrInputs.size(); i++)
    {
      AttrInput * attrInput = attrInputs.at(i);
      QString attributeName = attrInput->name();
      QString attributeValue = attrInput->getAnswer();

      elementTarget.setAttribute(attributeName, attributeValue);
    }

    // If the target element has an id attribute update it
    if( elementTarget.hasAttribute("id") )
    {
      QString prefixId = elementTarget.tagName().mid(0, 1);

      if(_elemPrefixIdCount.count(prefixId) >= 0)
        _elemPrefixIdCount[prefixId] += 1;
      else
        _elemPrefixIdCount[prefixId] = 1;

      elementTarget.setAttribute("id",  prefixId + "_" + QString::number(_elemPrefixIdCount[prefixId]));
    }
  }

  foreach (ElemInput *childElemInput, elemInput->elemInputs())
  {
    resolve (childElemInput, elementTarget, pdpRootElement, elementsMarked);
  }
}

bool XWizardPage::computeAnswers(QDomElement& finalAppRootElement, QDomElement& pdpRootElement,
                                 QSet<QString> &selectorsUsed)
{
  bool answer = true;

  for (int i = 0; i < _elemInputs.size(); i++)
  {
    ElemInput *elemInput = _elemInputs.at(i);
    resolve(elemInput, finalAppRootElement, pdpRootElement, selectorsUsed);
  }

  return answer;
}

QVector <QDomElement> XWizardPage::searchElement( const QString &tagname,
                                                  const QString &attributeName,
                                                  const QString &attributeValue,
                                                  QDomElement &rootElement )
{
  QVector<QDomElement> elementsToReturn;

  QDomNodeList elements;
  if (tagname == "*")
  {
    recursiveElementSearch(attributeName, attributeValue, rootElement, elementsToReturn);
  }
  else
  {
    QString rootTagname = rootElement.tagName();
    if (rootTagname == tagname)
    {
      QString rootAttributeValue = rootElement.attribute(attributeName);
      if (rootAttributeValue == attributeValue)
      {
        elementsToReturn.append(rootElement);
      }
    }

    elements = rootElement.elementsByTagName(tagname);

    for (int i = 0; i < elements.size(); i++)
    {
      QDomElement element = elements.at(i).toElement();
      QString value = element.attribute(attributeName, "");

      if (value == attributeValue)
      {
        elementsToReturn.append(element);
      }
    }
  }

  return elementsToReturn;
}

void XWizardPage::recursiveElementSearch( const QString &attributeName,
                                          const QString &attributeValue,
                                          QDomElement &rootElement,
                                          QVector<QDomElement> &elementsToReturn)
{
  if (!rootElement.isNull())
  {
    QString rootAttributeValue = rootElement.attribute(attributeName);
    if (rootAttributeValue == attributeValue){
      elementsToReturn.append(rootElement);
    }

    QDomNodeList elements = rootElement.childNodes();
    for (int i = 0; i < elements.size(); i++){
      QDomElement element = elements.at(i).toElement();

      recursiveElementSearch(attributeName, attributeValue, element, elementsToReturn);
    }
  }
}

void XWizardPage::cloneElemInput(ElemInput *elemInput)
{
  if (elemInput)
  {
    addElemInput(elemInput);
  }
}

void XWizardPage::removeElemInput(ElemInput *elemInput)
{
  if (_elemInputs.size() == 1){
    QMessageBox::warning(this, "Warning", "Each page must have at least one <elemInput>.", QMessageBox::Ok);
    return;
  }

  if (elemInput)
  {
    for (int i = 0; i < _elemInputs.size(); i++)
      if (_elemInputs.at(i) == elemInput){
        _elemInputs.remove(i);
        break;
      }

    int count = _containerLayout->count();
    for (int i = 0; i < count; i++)
    {
      QLayoutItem *item = _containerLayout->itemAt(i);
      if (item && item->widget() == elemInput)
      {
        _containerLayout->removeItem(item);
        delete item;
        elemInput->deleteLater();
        break;
      }
    }
  }
}

bool XWizardPage::validatePage()
{
  return true;
}

void XWizardPage::addLabel(const QString &text)
{
  _containerLayout->addWidget(new QLabel(text, this));
}

void XWizardPage::addHtmlPage(const QString &html)
{
  QWebView *view = new QWebView(this);
  _containerLayout->addWidget(view);
  view->load(html);
}

