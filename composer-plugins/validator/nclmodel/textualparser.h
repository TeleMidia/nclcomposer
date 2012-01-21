#ifndef TEXTUALPARSER_H
#define TEXTUALPARSER_H

#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "nclmodel.h"

namespace nclValidator {

class ModelElement;

using namespace xercesc;
using namespace std;

class TextualParser : public HandlerBase{
public:
    TextualParser (ModelElement *element, string toFind) { _element = element; _idToFind = toFind; }

    void startDocument () {}
    void startElement(const XMLCh* const, AttributeList&);
    void endElement(const XMLCh* const) {}
    void endDocument() {}

private:
    ModelElement *_element;
    string _idToFind;
};


}
#endif // TEXTUALPARSER_H
