#include "WSWriter.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>

#include "SelectsParser.h"

void WSWriter::writeWizard(const QString &path)
{
  QDomDocument auxDoc ("aux");

  QFile auxFile (path);
  if (!auxFile.open(QIODevice::ReadOnly))
  {
    QMessageBox::critical(0, "Error", "Error while opening '" +
                          path + "' file.");
    return;
  }
  if (!auxDoc.setContent(&auxFile))
  {
    QMessageBox::critical(0, "Error", "Error while parsing '" +
                          path + "' file.");
    return;
  }
  auxFile.close();

  QDomDocument wsDoc;
  wsDoc.createProcessingInstruction("xml",
                                    "version=\"1.0\" encoding=\"utf-8\"");

  QDomElement wsRoot = wsDoc.createElement("wizard");
  wsDoc.appendChild(wsRoot);

  QDomNodeList gapsList = auxDoc.elementsByTagName("gap");

  int stepCount = 0;
  int elemInputCount = 0;

  SelectsParser selectsParser;

  for (int i = 0; i < gapsList.size(); i++)
  {
    QDomElement gapElement = gapsList.at(i).toElement();

    QDomElement stepElement = wsDoc.createElement("step");
    wsRoot.appendChild(stepElement);

    stepElement.setAttribute("id", "step" + QString::number(stepCount++));

    QDomElement elemInputElement = wsDoc.createElement("elemInput");
    stepElement.appendChild(elemInputElement);

    elemInputElement.setAttribute("id", "input" +
                                  QString::number(elemInputCount++));

    QString componentId = gapElement.attribute("id");
    QDomText elemInputText = wsDoc.createTextNode("Do you want create a new \""
                                                  + componentId +
                                                  "\" component?");
    elemInputElement.appendChild(elemInputText);

    QString selectsAttr = gapElement.attribute("selects");
    QString tagname = "";
    QString attributeName = "";
    QString attributeValue = "";


    if (selectsAttr != "")
    {
      elemInputElement.setAttribute("selector", selectsAttr);

      selectsParser.setSeletsAttributeValue(selectsAttr);
      tagname = selectsParser.getElementTagname();

      if (tagname == "*") tagname = "media";

      attributeName = selectsParser.getElementAttributeName();
      attributeValue = selectsParser.getElementAttributeValue();
    }

    QDomElement attrInputElement = wsDoc.createElement("attrInput");

    if (tagname == "media")
    {
      attrInputElement.setAttribute("name", "src");
      attrInputElement.setAttribute("type", "file");

      QDomText attrInputText = wsDoc.createTextNode("Please enter with the "
                                                    "value for the \"src\" "
                                                    "attribute");
      attrInputElement.appendChild(attrInputText);

      elemInputElement.appendChild(attrInputElement);

      QDomElement propertyElemInput = wsDoc.createElement("elemInput");
      elemInputElement.appendChild(propertyElemInput);

      QDomText propertyAttrInputText =
          wsDoc.createTextNode("You are now defining the attributes of the \""
                               + componentId + "\"");

      propertyElemInput.appendChild(propertyAttrInputText);

      propertyElemInput.setAttribute("id", "input" +
                                     QString::number(elemInputCount++));
      propertyElemInput.setAttribute("selector", selectsAttr +
                                     ">property[name=bounds]");

      QDomElement valueAttrInputElement = wsDoc.createElement("attrInput");
      propertyElemInput.appendChild(valueAttrInputElement);

      valueAttrInputElement.setAttribute("type", "string");
      valueAttrInputElement.setAttribute("name", "value");

      QDomText valueAttrInputText =
          wsDoc.createTextNode("Please enter with the value for the "
                               "\"bounds\" attribute");
      valueAttrInputElement.appendChild(valueAttrInputText);
    }
  }

  QFileInfo fileInfo (auxFile);
  QString folder = fileInfo.absolutePath();
  QString wsName = fileInfo.baseName() + ".ws";

  QString str;
  QTextStream stream(&str);
  wsDoc.save(stream, 4);

  QString filename = folder + "/" + wsName;
  QFile file(filename);
  if (file.open(QIODevice::ReadWrite))
  {
    QTextStream stream(&file);
    stream << str;
  }

  auxFile.remove();
}
