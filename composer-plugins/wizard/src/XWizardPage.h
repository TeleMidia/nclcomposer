#ifndef XWIZARDPAGE_H
#define XWIZARDPAGE_H

#include <QWizardPage>
#include <QMap>
#include <QVBoxLayout>
#include <QDomElement>
#include <QSet>
#include <QUuid>

#include <QFormLayout>

#include "Constants.h"
#include "ElemInput.h"

class XWizardPage : public QWizardPage
{
  Q_OBJECT

public:
  inline static void createRandomIds(QDomElement& rootElement)
  {
    QString uuid = QUuid::createUuid().toString();
    rootElement.setAttribute(TEMP_ATTR, uuid);

    QDomElement childElement = rootElement.firstChildElement();
    while (!childElement.isNull())
    {
      createRandomIds(childElement);
      childElement = childElement.nextSiblingElement();
    }
  }

  explicit XWizardPage(const QString &title,
                       const QString &text = "",
                       QWidget *parent = 0);

  ElemInput * addElemInput (const QString &id,
                            const QString &elemInputSelector,
                            const QString &title );

  void addElemInput (ElemInput *);

  void createConnections (ElemInput *);

  void addAttrInput (ElemInput *elemInput,
                     const QString &question,
                     const QString&,
                     const QString &type = "string",
                     const QString &value = "" );

  bool computeAnswers (QDomElement& finalAppRootElement,
                       QDomElement& pdpRootElement,
                       QSet <QString>& selectorsUsed );

  void addLabel(const QString &text);
  void addHtmlPage(const QString &html);

  bool validatePage();

public slots:
  void cloneElemInput (ElemInput *elemInput);
  void removeElemInput (ElemInput *elemInput);

private:
  QVector<QDomElement> searchElement ( const QString &tagname,
                                       const QString &attributeName,
                                       const QString &attributeValue,
                                       QDomElement &rootElement );

  void resolve (ElemInput *elemInput, QDomElement& finalAppRootElement,
                QDomElement& pdpRootElement, QSet <QString>& elementsMarked);

  void recursiveElementSearch ( const QString &attributeName,
                                const QString &attributeValue,
                                QDomElement &rootElement,
                                QVector<QDomElement>&elementsToReturn );

  static QMap <QString, int> _elemPrefixIdCount;

  QVector <ElemInput*> _elemInputs;
  QFormLayout *_containerLayout;


};

#endif // XWIZARDPAGE_H
