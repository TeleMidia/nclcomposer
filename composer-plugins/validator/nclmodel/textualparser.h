#ifndef TEXTUALPARSER_H
#define TEXTUALPARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <QXmlDefaultHandler>
#include <QString>
#include "nclmodel.h"

namespace nclValidator {

class ModelElement;

using namespace std;

class TextualParser : public QXmlDefaultHandler{
public:
    TextualParser (ModelElement *element, string toFind) { _element = element; _idToFind = toFind; }

    bool startElement(const QString &, const QString &, const QString &, const QXmlAttributes &);


private:
    ModelElement *_element;
    string _idToFind;
};


}
#endif // TEXTUALPARSER_H
