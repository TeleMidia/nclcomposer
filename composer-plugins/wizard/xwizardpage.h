#ifndef XWIZARDPAGE_H
#define XWIZARDPAGE_H

#include <QWizardPage>
#include <QMap>
#include <QVBoxLayout>
#include <QDomElement>
#include <QSet>
#include <QUuid>

#include "constants.h"
#include "eleminput.h"

class XWizardPage : public QWizardPage
{
    Q_OBJECT

private:
    QMap <QString, int> _elemPrefixIdCount;

    QVector <ElemInput*> _elemInputs;
    QVBoxLayout *_containerLayout;

    QVector<QDomElement> searchElement (QString tagname, QString attributeName, QString attributeValue,
                               QDomElement &rootElement);
    QDomElement searchParent (QString tagname, QString attributeName, QString attributeValue,
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
        while (!childElement.isNull()){
            createRandomIds(childElement);
            childElement = childElement.nextSiblingElement();
        }
    }

    explicit XWizardPage(QWidget *parent = 0);

    ElemInput * addElemInput (QString id, QString elemInputSelector, QString title);
    void addElemInput (ElemInput *);

    void addAttrInput (ElemInput *elemInput, QString question, QString, QString type = "string", QString value = "");

    bool computeAnswers (QDomElement& finalAppRootElement, QDomElement& pdpRootElement,
                         QSet <QString>& selectorsUsed);

    bool validatePage();
    
signals:
    
public slots:
    void cloneElemInput (ElemInput *elemInput);
    void removeElemInput (ElemInput *elemInput);
    
};

#endif // XWIZARDPAGE_H
