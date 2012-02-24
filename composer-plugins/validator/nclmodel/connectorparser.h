#ifndef CONNECTORPARSER_H
#define CONNECTORPARSER_H

#include <QXmlDefaultHandler>
#include "nclmodel.h"


namespace nclValidator {

class ModelElement;
class Model;

using namespace std;

class ConnectorParser : public QXmlDefaultHandler
{
public:
    ConnectorParser(Model *, string);
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);

    map <string, pair <int, int> > getRolesMap () const {return _roles; }

private:
    Model *_model;
    string _connectorId;
    map <string, pair <int, int> > _roles;
    bool flag;
    bool completed;
};

}
#endif // CONNECTORPARSER_H
