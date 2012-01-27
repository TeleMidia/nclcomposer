#ifndef CONNECTORPARSER_H
#define CONNECTORPARSER_H

#include <xercesc/sax/HandlerBase.hpp>
#include "nclmodel.h"


namespace nclValidator {

class ModelElement;
class Model;

using namespace xercesc;
using namespace std;

class ConnectorParser : public HandlerBase
{
public:
    ConnectorParser(Model *, string);
    void startDocument () {}
    void startElement(const XMLCh* const, AttributeList&);
    void endElement(const XMLCh* const);
    void endDocument() {}
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
