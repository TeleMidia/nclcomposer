#include "templateparser.h"

#include <QDir>
#include <QFile>
#include <stdlib.h>
#include <QDebug>
#include <QMessageBox>
#include <QDomDocument>

#include "constants.h"

QVector <QString> TemplateParser::_toParse;

QVector<QString> TemplateParser::parse(QString path)
{
  QVector <QString> auxPaths;
  QDir dir;
  if (dir.mkdir(DEFAULT_DIR))
  {
    QMessageBox::information(0, "Directory created", "Directory " + DEFAULT_DIR + " created successfully. The output files will be stored in this folder!", QMessageBox::Ok);
  }

  QFile filePath (path);
  if (!filePath.exists())
  {
    QMessageBox::critical(0, "File not found", "The file '" + path + "'' doesn't exist!");
  }
  else
  {
    QDomDocument doc ("TAL");
    if (!filePath.open(QIODevice::ReadOnly))
    {
      QMessageBox::critical(0, "Error", "Unable to open the file '" + path + "'.");
      return auxPaths;
    }
    if (!doc.setContent(&filePath, true))
    {
      QMessageBox::critical(0, "Error", "Error while parsing the document.");
      return auxPaths;
    }
    filePath.close();

    QDomElement docElem = doc.documentElement();

    createTemplatePaths(docElem);

    foreach (QString templatePath, _toParse)
    {
      QString auxPath = generateAuxFile(templatePath);

      if (auxPath != "") auxPaths.append(auxPath);
    }

    QMessageBox::information(0, "", "Aux files generated successfully.", QMessageBox::Ok);
  }

  return auxPaths;
}

QString TemplateParser::generateAuxFile(QString path)
{
  QDomDocument doc ("template");
  QFile file (path);

  if (!file.open(QIODevice::ReadOnly))
  {
    QMessageBox::critical(0, "Error", "Unable to open the file '" + path + "'.");
    return "";
  }

  if (!doc.setContent(&file, true))
  {
    QMessageBox::critical(0, "Error", "Internal error while parsing '" + path + "' file.");
    return "";
  }
  file.close();

  QDomElement root = doc.documentElement();

  QDomDocument auxDoc ("aux");
  QDomElement auxRoot = auxDoc.createElement("wizardAux");
  auxDoc.appendChild(auxRoot);

  auxRoot.setAttribute("templatePath", path);

  QDomElement element = root.firstChildElement();

  if (element.tagName() == "template")
  {
    QString templateId = element.attribute("id");
    if (templateId != "")
      auxRoot.setAttribute("templateId", templateId);

    element = element.firstChildElement();
  }

  while (!element.isNull())
  {
    if (element.tagName() == "component" || element.tagName() == "interface")
    {
      auxRoot.appendChild(parseTALComponent(element, auxDoc));
    }
    else if (element.tagName() == "assert" || element.tagName() == "report")
    {
      QString tagname = element.tagName();
      QDomElement restrictionElement = auxDoc.createElement(tagname);
      auxRoot.appendChild(restrictionElement);

      restrictionElement.setAttribute("test", element.attribute("test"));

      QDomText textNode = auxDoc.createTextNode(element.text().trimmed());
      restrictionElement.appendChild(textNode);
    }

    element = element.nextSiblingElement();
  }

  int index = path.lastIndexOf("/");
  if (index != -1)
  {
    QString folder = path.left(index);
    QString templateId = path.mid(index);
    QString str;
    QTextStream stream(&str);
    auxDoc.save(stream, 4);

    QString filename = folder + templateId + ".aux";
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite))
    {
      QTextStream stream(&file);
      stream << str;
    }
    return filename;
  }

  return "";
}

QDomElement TemplateParser::parseTALComponent(QDomElement &component, QDomDocument &auxDoc)
{
  QDomElement auxGap = auxDoc.createElement("gap");
  auxGap.setAttribute("type", component.tagName());
  auxGap.setAttribute("selects", component.attribute("selects"));
  auxGap.setAttribute("id", component.attribute("id"));

  QDomElement comp = component.firstChildElement();
  while (!comp.isNull())
  {
    if (comp.tagName() == "component" || comp.tagName() == "interface")
      auxGap.appendChild(parseTALComponent(comp, auxDoc));

    comp = comp.nextSiblingElement();
  }

  return auxGap;
}

void TemplateParser::createTemplatePaths(QDomElement &node)
{
  QDir dir;
  QDomElement templateElement = node.firstChildElement();
  while(!templateElement.isNull())
  {
    QString templateId = templateElement.attribute("id");

    if (templateId != "")
    {
      dir.mkdir(DEFAULT_DIR + templateId);
      QDomDocument doc2 ("NewTAL");

      QDomElement root = doc2.createElementNS(TAL_NS, "tal:tal");
      doc2.appendChild(root);

      root.appendChild(templateElement.cloneNode(true));

      QString str;
      QTextStream stream(&str);
      doc2.save(stream, 4, QDomNode::EncodingFromTextStream);

      QString filename = DEFAULT_DIR + templateId + "/" + templateId + ".tal";
      QFile file(filename);
      if (file.open(QIODevice::ReadWrite))
      {
        QTextStream stream(&file);
        stream << str;

        _toParse.append(filename);
      }
    }
    templateElement = templateElement.nextSiblingElement();
  }
}
