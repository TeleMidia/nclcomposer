#include "connectorparser.h"

namespace nclValidator {

ConnectorParser::ConnectorParser(Model *model, string connectorId)
{
    _model = model;
    _connectorId = connectorId;
    flag = false;
    completed = false;
}


void ConnectorParser::startElement(const XMLCh *const name, AttributeList &attr)
{
    if (completed) return;

    char* elementName = XMLString::transcode(name);
    string id = "";

    vector <Attribute> att;
    for (XMLSize_t i = 0; i < attr.getLength(); i++) {
        string name (XMLString::transcode(attr.getName(i)));
        string value(XMLString::transcode(attr.getValue(i)));

        if (name == "id")
            id = value;

        att.push_back(Attribute (name, value));
    }

    ModelElement element;
    element.setAttributes(att);
    element.setElementName(elementName);


    if (id == _connectorId && strcmp (elementName, "causalConnector") == 0){
        flag = true;
    }

    if (flag){
        _model->parseConnectorChild(&element, _roles);
    }
}

void ConnectorParser::endElement(const XMLCh *const name)
{
    string elementName = XMLString::transcode(name);
    if (elementName == "causalConnector" && flag){
        completed = true;

//        for (map<string, pair <int, int> >::iterator it = _rolesPointed->begin(); it != _rolesPointed->end(); it++ )
//            fprintf (stderr, "%s %d %d\n", it->first.c_str(), it->second.first, it->second.second);
    }
}


}
