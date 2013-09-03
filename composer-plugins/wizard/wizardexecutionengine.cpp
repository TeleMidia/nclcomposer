#include "wizardexecutionengine.h"

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include "xwizardpage.h"

#include <QSet>
#include <QDebug>

WizardExecutionEngine::WizardExecutionEngine(const QString &wsPath,
                                             QObject *parent):
    QObject(parent)
{
  if(wsPath != "")
    setWS(wsPath);

  _treeView = new QTreeWidget;
  _treeView->setHeaderLabel("Wizard Steps:");
  _treeView->setMinimumWidth(200);

  _progressBar = new QProgressBar;
  _progressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

  QVBoxLayout *sideWidgetLayout = new QVBoxLayout;
  sideWidgetLayout->addWidget(_treeView);
  sideWidgetLayout->addWidget(_progressBar);

  QWidget *sideWidget = new QWidget;
  sideWidget->setLayout(sideWidgetLayout);

  _wizard.setWizardStyle(QWizard::ClassicStyle);

  _wizard.setSideWidget(sideWidget);

  _wizard.setFixedSize(800, 600);

  connect(&_wizard,
          SIGNAL(accepted()),
          this,
          SLOT(createFinalApplication()));

  connect (&_wizard,
           SIGNAL(currentIdChanged(int)),
           this,
           SLOT(updateSelection(int)));

  connect (_treeView,
           SIGNAL(itemSelectionChanged()),
           this,
           SLOT(updateCurrentPage()));
}

void WizardExecutionEngine::updateSelection(int currentIndex)
{
    _treeView->setCurrentIndex(_treeView->model()->index(currentIndex,0));
    _progressBar->setValue(currentIndex);
}

void WizardExecutionEngine::updateCurrentPage()
{
    int selectedRow = _treeView->currentIndex().row();
    int currentPage = _wizard.currentId();

    if (selectedRow > currentPage)
        for (int i = 0; i < selectedRow - currentPage; i++)
            _wizard.next();
    else
        for (int i = 0; i < currentPage - selectedRow; i++)
            _wizard.back();

    _progressBar->setValue(selectedRow);
}

void WizardExecutionEngine::setWS(const QString &wsPath)
{
  _wsPath = wsPath;
  _inputFile = "";

  QFile wsFile (wsPath);
  if (!wsFile.open(QIODevice::ReadOnly)){
      QMessageBox::critical(0, "Error", "Error while opening the file " + wsPath + " for reading!");
      return;
    }
  QDomDocument wsDoc ("wsDoc");
  if (!wsDoc.setContent(&wsFile))
  {
    QMessageBox::critical(0, "Error", "Error while parsing the file " + wsPath + ".");
    return;
  }
  wsFile.close();

  QDomElement rootElement = wsDoc.documentElement();
  QDomNodeList stepList = rootElement.elementsByTagName("step");

  _progressBar->setMaximum(stepList.size());
  _progressBar->setValue(0);

  for (int i = 0; i < stepList.size(); i++)
  {
    QDomElement stepElement = stepList.at(i).toElement();

    QString stepId = stepElement.attribute("id");
    _treeView->addTopLevelItem(new QTreeWidgetItem(QStringList(stepId)));

    QString stepText = "";

    XWizardPage *page = new XWizardPage ( stepElement.attribute("title"),
                                          stepText.trimmed() );
    page->setSubTitle(stepElement.attribute("subtitle"));

    QDomElement stepChildElement = stepElement.firstChildElement();

    while (!stepChildElement.isNull())
    {
      if(stepChildElement.tagName() == "elemInput")
      {
        QString elemInputText = "";
        QDomNodeList childNodes = stepChildElement.childNodes();
        for (int i = 0; i < childNodes.size(); i++)
        {
          QDomNode node = childNodes.at(i);
          if (node.isText())
            elemInputText += node.toText().data();
        }

        QString elemInputId = stepChildElement.attribute("id");
        QString elemInputSelector = stepChildElement.attribute("selector");

        ElemInput *elemInput = page->addElemInput(elemInputId, elemInputSelector, elemInputText);

        QDomElement attrInput = stepChildElement.firstChildElement("attrInput");
        while (!attrInput.isNull())
        {
          QString attrInputQuestion = attrInput.text();
          QString attrInputName = attrInput.attribute("name");
          QString attrInputType = attrInput.attribute("type");
          QString attrInputValue = attrInput.attribute("value");

          page->addAttrInput(elemInput, attrInputQuestion, attrInputName, attrInputType, attrInputValue);

          attrInput = attrInput.nextSiblingElement("attrInput");
        }

        QDomElement elemInputRecursiveElement = stepChildElement.firstChildElement("elemInput");
        while (!elemInputRecursiveElement.isNull())
        {
          elemInput->addElemInput(elemInputRecursiveElement);

          elemInputRecursiveElement = elemInputRecursiveElement.nextSiblingElement("elemInput");
        }
      }
      else if(stepChildElement.tagName() == "label")
      {
        page->addLabel(stepChildElement.text());
      }

      stepChildElement = stepChildElement.nextSiblingElement();
    }

    _wizard.addPage(page);
  }
}

void WizardExecutionEngine::setInputFile(const QString &inputFile)
{
  this->_inputFile = inputFile;
}

void WizardExecutionEngine::run()
{
  if(_inputFile == "")
    _inputFile = QFileDialog::getOpenFileName(0, "Select the file input", QDir::homePath());

  if (_inputFile != "")
    _wizard.exec();
}

void WizardExecutionEngine::createFinalApplication()
{
  QFile inputPDP (_inputFile);
  if (!inputPDP.open(QIODevice::ReadOnly)) {
    QMessageBox::critical(0, "Error", "Error while opening the file " + _inputFile + " for reading!");
    return;
  }

  QDomDocument pdpDoc ("pdpDoc");
  QDomDocument finalAppDoc ("finalApp");

  if (!(pdpDoc.setContent(&inputPDP)))
  {
    QMessageBox::critical(0, "Error", "Error while parsing the file " + _inputFile + ".");
    return;
  }

  inputPDP.seek(0);
  if (!(finalAppDoc.setContent(&inputPDP)))
  {
    QMessageBox::critical(0, "Error", "Error while parsing the file " + _inputFile + ".");
    return;
  }

  inputPDP.close();

  QList <int> pageIds = _wizard.pageIds();
  QDomElement finalAppRootElement = finalAppDoc.documentElement();
  QDomElement pdpRootElement = pdpDoc.documentElement();

  XWizardPage::createRandomIds(finalAppRootElement);

  QSet <QString> elementsMarked;

  for (int i = 0; i < pageIds.size(); i++)
  {
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

  if (file.open(QIODevice::ReadWrite))
  {
    QTextStream stream(&file);
    stream << str;
  }
}

void WizardExecutionEngine::removeUuid(QDomElement &rootElement)
{
  rootElement.removeAttribute(TEMP_ATTR);
  QDomElement childElement = rootElement.firstChildElement();
  while (!childElement.isNull())
  {
    removeUuid(childElement);
    childElement = childElement.nextSiblingElement();
  }
}

