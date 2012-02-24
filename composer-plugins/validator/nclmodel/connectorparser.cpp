#include "connectorparser.h"

namespace nclValidator {

ConnectorParser::ConnectorParser(Model *model, string connectorId)
{
    _model = model;
    _connectorId = connectorId;
    flag = false;
    completed = false;
}


bool ConnectorParser::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &atts)
{
    if (completed) return true;


    string id = "";

    vector <Attribute> att;
    for (int i = 0; i < atts.count(); i++) {
        string name (atts.qName(i).toStdString());
        string value(atts.value(i).toStdString());

        if (name == "id")
            id = value;

        att.push_back(Attribute (name, value));
    }

    ModelElement element;
    element.setAttributes(att);
    element.setElementName(qName.toStdString());


    if (id == _connectorId && strcmp (qName.toStdString().c_str(), "causalConnector") == 0){
        flag = true;
    }

    if (flag){
        _model->parseConnectorChild(&element, _roles);
    }

    return true;
}

bool ConnectorParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    if (qName.toStdString() == "causalConnector" && flag){
        completed = true;

//        for (map<string, pair <int, int> >::iterator it = _rolesPointed->begin(); it != _rolesPointed->end(); it++ )
//            fprintf (stderr, "%s %d %d\n", it->first.c_str(), it->second.first, it->second.second);
    }

    return true;
}


}
