#ifndef XWIZARDPAGE_H
#define XWIZARDPAGE_H

#include <QWizardPage>
#include <QMap>
#include <QVBoxLayout>
#include <QDomElement>
#include <QSet>
#include <QUuid>

#include <QFormLayout>

#include "constants.h"
#include "eleminput.h"

class XWizardPage : public QWizardPage
{
  Q_OBJECT

private:
  QMap <QString, int> _elemPrefixIdCount;

  QVector <ElemInput*> _elemInputs;
  QFormLayout *_containerLayout;

  QVector<QDomElement> searchElement (QString tagname, QString attributeName, QString attributeValue,
                             QDomElement &rootElement);

  void resolve (ElemInput *elemInput, QDomElement& finalAppRootElement,
                QDomElement& pdpRootElement, QSet <QString>& elementsMarked);

  void recursiveElementSearch (QString attributeName, QString attributeValue,
                               QDomElement &rootElement, QVector<QDomElement>&elementsToReturn);
public:
  static void createRandomIds(QDomElement& rootElement)
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

  explicit XWizardPage( const QString &title,
                        const QString &text = "",
                        QWidget *parent = 0);

  ElemInput * addElemInput ( const QString &id,
                             const QString &elemInputSelector,
                             const QString &title );

  void addElemInput (ElemInput *);

  void createConnections (ElemInput *);

  void addAttrInput ( ElemInput *elemInput,
                      const QString &question,
                      const QString&,
                      const QString &type = "string",
                      const QString &value = "" );

  bool computeAnswers ( QDomElement& finalAppRootElement,
                        QDomElement& pdpRootElement,
                        QSet <QString>& selectorsUsed );

  void addLabel(QString text);

  bool validatePage();
    
signals:
    
public slots:
  void cloneElemInput (ElemInput *elemInput);
  void removeElemInput (ElemInput *elemInput);
    
};

#endif // XWIZARDPAGE_H
