#ifndef TEMPLATEPARSER_H
#define TEMPLATEPARSER_H

#include <QString>
#include <QDomElement>
#include <QVector>

class TemplateParser
{
public:
  static QVector <QString> parse (const QString &path);

private:
  static QVector <QString> _toParse;
  static void createTemplatePaths (QDomElement &);
  static QString generateAuxFile(const QString&);
  static QDomElement parseTALComponent (const QDomElement&,
                                        QDomDocument &);

};

#endif // TEMPLATEPARSER_H
