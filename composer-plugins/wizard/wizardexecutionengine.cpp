#include "wizardexecutionengine.h"

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include "xwizardpage.h"

#include <QSet>
#include <QDebug>

WizardExecutionEngine::WizardExecutionEngine(QString wsPath)
{
    setWS(wsPath);

    connect(&_wizard, SIGNAL(accepted()), this, SLOT(createFinalApplication()));
}

void WizardExecutionEngine::setWS(QString wsPath)
{
    _wsPath = wsPath;
    _inputFile = "";

    QFile wsFile (wsPath);
    if (!wsFile.open(QIODevice::ReadOnly)){
        QMessageBox::critical(0, "Error", "Error while opening the file " + wsPath + " for reading!");
        return;
    }
    QDomDocument wsDoc ("wsDoc");
    if (!wsDoc.setContent(&wsFile)){
        QMessageBox::critical(0, "Error", "Error while parsing the file " + wsPath + ".");
        return;
    }
    wsFile.close();

    QDomElement rootElement = wsDoc.documentElement();
    QDomNodeList stepList = rootElement.elementsByTagName("step");
    for (int i = 0; i < stepList.size(); i++){
        QDomElement stepElement = stepList.at(i).toElement();

        XWizardPage *page = new XWizardPage;
        QDomElement elemInputElement = stepElement.firstChildElement("elemInput");
        while (!elemInputElement.isNull()){
            QString elemInputText = "";
            QDomNodeList childNodes = elemInputElement.childNodes();
            for (int i = 0; i < childNodes.size(); i++){
                QDomNode node = childNodes.at(i);
                if (node.isText()){
                    elemInputText += node.toText().data();
                }
            }

            QString elemInputId = elemInputElement.attribute("id");
            QString elemInputSelector = elemInputElement.attribute("selector");

            ElemInput *elemInput = page->addElemInput(elemInputId, elemInputSelector, elemInputText);

            QDomElement attrInput = elemInputElement.firstChildElement("attrInput");
            while (!attrInput.isNull()){
                QString attrInputQuestion = attrInput.text();
                QString attrInputName = attrInput.attribute("name");
                QString attrInputType = attrInput.attribute("type");
                QString attrInputValue = attrInput.attribute("value");

                page->addAttrInput(elemInput, attrInputQuestion, attrInputName, attrInputType, attrInputValue);

                attrInput = attrInput.nextSiblingElement("attrInput");
            }

            QDomElement elemInputRecursiveElement = elemInputElement.firstChildElement("elemInput");
            while (!elemInputRecursiveElement.isNull()){
                elemInput->addElemInput(elemInputRecursiveElement);

                elemInputRecursiveElement = elemInputRecursiveElement.nextSiblingElement("elemInput");
            }

            elemInputElement = elemInputElement.nextSiblingElement("elemInput");
        }

        _wizard.addPage(page);
    }

}

void WizardExecutionEngine::run()
{
    _inputFile = QFileDialog::getOpenFileName(0, "Select the file input", getenv("HOME"));
    if (_inputFile != "")
        _wizard.exec();
}

void WizardExecutionEngine::createFinalApplication()
{
    QFile inputPDP (_inputFile);
    if (!inputPDP.open(QIODevice::ReadOnly)){
        QMessageBox::critical(0, "Error", "Error while opening the file " + _inputFile + " for reading!");
        return;
    }
    QDomDocument pdpDoc ("pdpDoc");
    QDomDocument finalAppDoc ("finalApp");

    if (!(pdpDoc.setContent(&inputPDP))){
        QMessageBox::critical(0, "Error", "Error while parsing the file " + _inputFile + ".");
        return;
    }
    inputPDP.seek(0);
    if (!(finalAppDoc.setContent(&inputPDP))){
        QMessageBox::critical(0, "Error", "Error while parsing the file " + _inputFile + ".");
        return;
    }

    inputPDP.close();

    QList <int> pageIds = _wizard.pageIds();
    QDomElement finalAppRootElement = finalAppDoc.documentElement();
    QDomElement pdpRootElement = pdpDoc.documentElement();


    XWizardPage::createRandomIds(finalAppRootElement);

    QSet <QString> elementsMarked;

    for (int i = 0; i < pageIds.size(); i++){
        int pageId = pageIds.at(i);

        XWizardPage *xWizardPage = (XWizardPage *) _wizard.page(pageId);
        xWizardPage->computeAnswers(finalAppRootElement, pdpRootElement, elementsMarked);
    }

    removeUuid(finalAppRootElement);

    QFileInfo fileInfo (_inputFile);
    QString path = fileInfo.absolutePath();
    QString fileName = fileInfo.baseName();
    QString suffix = fileInfo.suffix();

    QString str;
    QTextStream stream(&str);
    finalAppDoc.save(stream, 4);

    QString filename = path + "/" + fileName + "Final." + suffix;
    QFile file(filename);

    if (file.exists())
        file.remove();

    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << str;
    }
}

void WizardExecutionEngine::removeUuid(QDomElement &rootElement)
{
    rootElement.removeAttribute(TEMP_ATTR);
    QDomElement childElement = rootElement.firstChildElement();
    while (!childElement.isNull()){
        removeUuid(childElement);
        childElement = childElement.nextSiblingElement();
    }
}

