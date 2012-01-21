#include "textualparser.h"

namespace nclValidator{


void TextualParser::startElement(const XMLCh * name, AttributeList & attr)
{
    char* elementName = XMLString::transcode(name);
    bool flag = false;

    vector <Attribute> att;
    for (XMLSize_t i = 0; i < attr.getLength(); i++) {
      string name (XMLString::transcode(attr.getName(i)));
      string value(XMLString::transcode(attr.getValue(i)));

      att.push_back(Attribute (name, value));

      if (name == "id" && value == _idToFind)
          flag = true;

     }

    if (flag){
        _element->setAttributes(att);
        _element->setElementName(elementName);
    }

}


}
