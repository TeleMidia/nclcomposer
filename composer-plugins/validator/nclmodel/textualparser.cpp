#include "textualparser.h"

namespace nclValidator{


bool TextualParser::startElement(const QString &, const QString &, const QString & qName, const QXmlAttributes & atts )
{

//    const char* elementName = qName.toStdString().c_str();

    bool flag = false;

    vector <Attribute> att;
    for (int i = 0; i < atts.count(); i++) {


        string name (atts.qName(i).toStdString());
        string value(atts.value(i).toStdString());

        att.push_back(Attribute (name, value));

        if (name == "id" && value == _idToFind){

            flag = true;
        }

    }



    if (flag){
        _element->setAttributes(att);
        _element->setElementName(qName.toStdString());
    }
    return true;
}


}
