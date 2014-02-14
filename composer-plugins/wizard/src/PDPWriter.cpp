#include "PDPWriter.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

#include <QDebug>

int PDPWriter::_mediaCount = 0;
int PDPWriter::_contextCount = 0;
int PDPWriter::_switchCount = 0;

void PDPWriter::writePDP(QString auxPath)
{
  QDomDocument auxDoc ("auxDoc");

  QFile auxFile (auxPath);
  if (!auxFile.open(QIODevice::ReadOnly))
  {
    QMessageBox::critical(0, "Error", "Error while opening the file '"
                          + auxPath + "'.");
    return;
  }

  if (!auxDoc.setContent(&auxFile))
  {
    QMessageBox::critical(0, "Error", "Error while parsing the file '" +
                          auxPath + "'.");
    return;
  }
  auxFile.close();

  QDomDocument pdpDocument;
  pdpDocument.createProcessingInstruction("xml",
                                          "version=\"1.0\" encoding=\"utf-8\"");

  QDomElement pdpRoot = pdpDocument.createElementNS(NCL_NS, "ncl");
  pdpDocument.appendChild(pdpRoot);

  QDomElement bodyElement = pdpDocument.createElement ("body");
  pdpRoot.appendChild(bodyElement);

  QDomElement auxRoot = auxDoc.documentElement();
  QString talPath = auxRoot.attribute("templatePath");
  QString templateId = auxRoot.attribute("templateId");
  bodyElement.setAttribute("template", talPath + "#" + templateId);

  QDomNodeList gapElements = auxDoc.elementsByTagName("gap");

  for (int i = 0; i < gapElements.size(); i++)
  {
    QDomElement gapElement = gapElements.at(i).toElement();
    QDomElement pdpElement = parseGapElement(gapElement, pdpDocument);

    if (!pdpElement.isNull())
      bodyElement.appendChild(pdpElement);
  }

  QFileInfo fileInfo (auxFile);
  QString folder = fileInfo.absolutePath();
  QString pdpName = fileInfo.baseName() + ".ncl";

  QString str;
  QTextStream stream(&str);
  pdpDocument.save(stream, 4);

  QString filename = folder + "/" + pdpName;
  QFile file(filename);
  if (file.open(QIODevice::ReadWrite))
  {
    QTextStream stream(&file);
    stream << str;
  }

  PDPWriter::_mediaCount = 0;
  PDPWriter::_contextCount = 0;
  PDPWriter::_switchCount = 0;
}

QDomElement PDPWriter::parseGapElement(const QDomElement &gapElement,
                                       QDomDocument& pdpDoc)
{
  QDomElement pdpElement;
  QString selectsAttr = gapElement.attribute("selects");
  if (selectsAttr != "")
  {
    SelectsParser selectsParser (selectsAttr);

    QString tagname = selectsParser.getElementTagname();

    if (tagname == "*") tagname = "media";

    QString attributeName = selectsParser.getElementAttributeName();
    QString attributeValue = selectsParser.getElementAttributeValue();

    if (tagname != "")
    {
      pdpElement = pdpDoc.createElement(tagname);
      if (tagname == "media")
      {
        pdpElement.setAttribute("id", QString ("m_" +
                                               QString::number(_mediaCount++)));
        pdpElement.setAttribute("src", "");

        QDomElement propertyElement = pdpDoc.createElement("property");
        pdpElement.appendChild(propertyElement);

        propertyElement.setAttribute("name", "bounds");
        propertyElement.setAttribute("value", "");

      }
      else if (tagname == "context")
        pdpElement.setAttribute("id", QString ("c_" +
                                               QString::number(
                                                 _contextCount++)));
      else if (tagname == "switch")
        pdpElement.setAttribute("id", QString ("s_" +
                                               QString::number(
                                                 _switchCount++)));

      if (attributeName != "")
        pdpElement.setAttribute(attributeName, attributeValue);
    }
  }

  return pdpElement;
}
