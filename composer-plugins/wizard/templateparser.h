#ifndef TEMPLATEPARSER_H
#define TEMPLATEPARSER_H

#include <QString>
#include <QDomElement>
#include <QVector>

class TemplateParser
{
private:
  static QVector <QString> _toParse;

  static void createTemplatePaths (QDomElement &);
  static QString generateAuxFile(QString);
  static QDomElement parseTALComponent (QDomElement&, QDomDocument&);


public:
  static QVector <QString> parse (QString path);
};

#endif // TEMPLATEPARSER_H
