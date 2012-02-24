/*
 * nclmodel.c
 *
 *  Created on: Jul 27, 2011
 *      Author: Rodrigo Costa
 */

#include "nclmodel.h"
#include <QDebug>


namespace nclValidator {

/***********************ModelElement's definition*****************************/
ModelElement::ModelElement(virtualId parentId){
    _parentId = parentId;
}

ModelElement::~ModelElement (){
}


void ModelElement::addAttribute(string name, string value){
    Attribute attr (name, value);
    _attributes.push_back (attr);
}

void ModelElement::addAttribute(Attribute &attr){
    _attributes.push_back (attr);

}

void ModelElement::setAttributes(vector <Attribute> &attributes){
    _attributes = attributes;
}

void ModelElement::setElementName(string name){
    _elementName.assign(name);
}

void ModelElement::setParent(virtualId parent){
    _parentId = parent;
}

void ModelElement::addChild(virtualId childId){
    _children.push_back(childId);
}

bool ModelElement::removeReference(virtualId id)
{
    for (int i=0; i < _referencesToMyself.size(); i++)
        if (_referencesToMyself[i] == id){
            _referencesToMyself.erase(_referencesToMyself.begin() + i);
            return true;
        }
    return false;
}

void ModelElement::removeChild(virtualId childId){
    for (size_t i=0; i < _children.size(); i++)
        if (childId == _children[i]){
            _children.erase (_children.begin () + i);
            break;
        }
}

Attribute ModelElement::attribute(string attribute) const{
    Attribute attr ("", "");
    for (size_t i = 0; i < _attributes.size(); i++)
        if (_attributes[i].name() == attribute)
            attr = _attributes[i];
    return attr;
}

/***********************Model's definition*****************************/
Model::Model(){
    _seed = 1;
}

Model::~Model(){
}

virtualId Model::randomId () {
    string s("vId");
    stringstream oss;
    oss << s << _seed ++;
    return oss.str ();
}

void Model::setElementId (virtualId id, ModelElement & element){
    if (&element)
        element._id = id;
}


bool Model::editElement(virtualId &id, vector <Attribute> &newAttributes){
    ModelElement *elementEdited = this->element(id);
    if (elementEdited){
        vector<Attribute> oldAttributes = elementEdited->attributes();
        string oldId = "";
        string newId = "";

//        if (elementEdited->elementName() == "importBase" || elementEdited->elementName() == "importNCL"){
//            string alias = elementEdited->attribute("alias").value();
//            string uri = elementEdited->attribute("documentURI").value();
//            for (int i = 0; i < _importedDocuments.size(); i++){
//                if (_importedDocuments[i].first == alias){
//                    _importedDocuments[i].second = uri;
//                    break;
//                }
//            }
//        }

        for (int i = 0; i < elementEdited->references().size(); i++)
            _affectedEllements.insert(elementEdited->references().at(i));
        _markedElements.insert(id);

        for (int i=0; i < oldAttributes.size(); i++){
            Attribute oldAttribute = oldAttributes[i];

            if (oldAttribute.name() == "id" || oldAttribute.name() == "alias")
                oldId = oldAttribute.value();

            if (Langstruct::isAttributeReferenceDependent(elementEdited->elementName(), oldAttribute.name())){

                vector <ModelElement *> references = elementByIdentifier(oldAttribute.value());
                if (references.size() > 0)
                    for (int j = 0; j < references.size(); j++){
                        ModelElement *ref = references[j];
                        ref->removeReference(id);
                    }
                else{
                    multimap<string, virtualId>::iterator it = _elementsNotYetInserted.find(oldAttribute.value());
                    if (it != _elementsNotYetInserted.end()){
                        do{
                            if ((*it).second == id){
                                _elementsNotYetInserted.erase(it);
                                break;
                            }
                        it++;
                        }while (it != _elementsNotYetInserted.upper_bound(oldAttribute.value()));
                    }
                }
            }
        }


        for (int i=0; i < newAttributes.size(); i++){
            Attribute newAttribute = newAttributes[i];

            if ((newAttribute.name() == "id" || newAttribute.name() == "alias") && newAttribute.value() != oldId){

                elementEdited->_referencesToMyself.clear();
                newId = newAttribute.value();


                for (map<virtualId, ModelElement>::iterator it = _modelElements.begin(); it != _modelElements.end(); it++){
                    ModelElement *el = &(it->second);

                    if (el == elementEdited) continue;

                    if (el){
                        vector<Attribute> elAttr = el->attributes();

                        for (int j =0; j < elAttr.size(); j++){
                            if (Langstruct::isAttributeReferenceDependent(el->elementName(), elAttr[j].name())){
                                if (elAttr[j].value() == newId){
                                    elementEdited->addReference(el->id());
                                }
                            }
                        }
                    }
                }


            }
        }

        multimap <string, virtualId>::iterator it = _idToElement.find(oldId);
        if (it != _idToElement.end()){
            do{
                if ((*it).second == elementEdited->id()){
                    _idToElement.erase(it);
                    break;
                }
                it ++;
            }while (it != _idToElement.upper_bound(oldId));
        }


        it = _elementsNotYetInserted.find(newId);
        if (it != _elementsNotYetInserted.end()){
            do{
                elementEdited->addReference((*it).second);
                it ++;
            }while (it != _elementsNotYetInserted.upper_bound(newId));
        }

        _elementsNotYetInserted.erase(newId);


        elementEdited->setAttributes(newAttributes);

        string _ = "";

        adjustReference(elementEdited->elementName(), *elementEdited, _);


        if (elementEdited->elementName() == "simpleAction" || elementEdited->elementName() == "simpleCondition" ||
            elementEdited->elementName() == "compoundAction" || elementEdited->elementName() == "compoundCondition" ||
            elementEdited->elementName() == "assessmentStatement" || elementEdited->elementName() == "attributeAssessment" ||
            elementEdited->elementName() == "connectorParam" || elementEdited->elementName() == "compoundStatement" ||
            elementEdited->elementName() == "valueAssessment") {
            ModelElement *parent = this->element(elementEdited->parent());
            while (parent){
                if (parent->elementName() == "causalConnector"){

                    _markedElements.insert(parent->id());
                    for (int i = 0; i < parent->references().size(); i++)
                        _affectedEllements.insert(parent->references().at(i));

                    break;
                }
                parent = this->element(parent->parent());
            }
        }

        return true;
    }
    return false;
}

bool Model::removeElement(virtualId &id){
    if (_modelElements.count(id)){
        ModelElement * element = this->element(id);
        ModelElement * parent = this->element(element->parent());

//        if (element->elementName() == "importBase" || element->elementName() == "importNCL"){
//            string alias = element->attribute("alias").value();
//            for (int i = 0; i < _importedDocuments.size(); i++)
//                if (_importedDocuments[i].first == alias){
//                    _importedDocuments.erase(_importedDocuments.begin() + i);
//                    break;
//                }

//        }

        if (parent){
            parent->removeChild(id);

            if (element->elementName() == "simpleCondition" ||
                    element->elementName() == "simpleAction" || element->elementName() == "attributeAssessment")
            {
                while (parent){
                    if (parent->elementName() == "causalConnector"){
                        _markedElements.insert(parent->id());
                        break;
                    }
                    parent = this->element(parent->parent());
                }
            }
            else if (element->elementName() == "bind")
                _markedElements.insert(parent->id());


        }

        for (int i = 0; i < element->references().size(); i++)
            _affectedEllements.insert(element->references().at(i));

        vector<Attribute> attrs = element->attributes();
        for (int i = 0; i < attrs.size(); i++){
            Attribute attribute = attrs[i];

            if (attribute.name() == "id" || attribute.name() == "alias"){
                multimap<string, virtualId>::iterator it = _idToElement.find(attribute.value());
                if (it != _idToElement.end())
                    _idToElement.erase(it);
            }

            if (Langstruct::isAttributeReferenceDependent(element->elementName(), attribute.name())){
                vector <ModelElement *> els = elementByIdentifier(attribute.value());
                for (int j = 0; j<els.size(); j++){
                    ModelElement * e = els[j];
                    e->removeReference(id);
                }
            }
        }

        for (int i=0; i < element->children().size(); i++){
            ModelElement *child = this->element(element->children()[i]);
            child->setParent(element->parent());
        }

        _markedElements.erase(id);
        _modelElements.erase(id);

        _nonIncrementalMarkedElements.erase(id);


        return true;
    }
    return false;
}

set <virtualId> Model::elementsWithErrorInLastPass()
{
    for (set<virtualId>::iterator it = _elementsWithErrorInLastPass.begin(); it != _elementsWithErrorInLastPass.end(); it++)
        if (_markedElements.count(*it))
            _elementsWithErrorInLastPass.erase(it);

    return _elementsWithErrorInLastPass;
}

ModelElement * Model::element(const virtualId & id){
    if (_modelElements.count(id)){
        return &_modelElements[id];
    }
    else{
        return 0;
    }
}

void Model::print(){
//    map<virtualId, ModelElement>::const_iterator iterator = _modelElements.begin();

//    for ( ; iterator != _modelElements.end(); iterator ++){
//        cout << "Element: " << (*iterator).second.elementName() << endl;
//        //
//        cout << "VirtualID: " << (*iterator).second.id() << endl;
//        cout << "Perspective: " << (*iterator).second.scope() << endl;

//        ModelElement * parent = element((*iterator).second.parent());

//        if (parent){

//            cout << "Parent: " << parent->elementName() << endl;
//            cout << "Parent virtualId: " << parent->id() << endl;
//        }

//        cout << "Children: ";
//        vector <virtualId> children = (*iterator).second.children();
//        for (size_t i = 0; i < children.size(); i++)
//            cout << children[i] << " ";
//        cout << "\nReferences: ";
//        vector <virtualId> references = (*iterator).second.references();
//        for (size_t i = 0; i < references.size(); i++)
//            cout << references[i] << " ";


//        cout << "\n*********************************" << endl;
//    }
//    cout << "\nelementsNotYetInserted size: " << _elementsNotYetInserted.size() << endl;

//    for( map<string,virtualId>::iterator ii=_elementsNotYetInserted.begin(); ii!=_elementsNotYetInserted.end(); ++ii)
//    {
//        cout << (*ii).first << " ";
//    }
//    cout << endl;

}

//***************************Novos métodos********************************

virtualId Model::addElement(string elementName, vector <Attribute> attributes){
    if (elementName != ""){
        virtualId id = randomId();

        while (_modelElements.count(id))
            id.append ("" + 0);

        ModelElement newElement;
        setElementId(id, newElement); //insere o id do elemento

        newElement.setElementName(elementName);
        newElement.setAttributes(attributes);
        newElement.setParent("");
        newElement.setScope("");

        if (elementName == "ncl")
            newElement.setScope(newElement.id());

//        if (elementName == "importBase" || elementName == "importNCL"){
//            string alias = newElement.attribute("alias").value();
//            string uri = newElement.attribute("documentURI").value();

//            _importedDocuments.push_back(make_pair(alias, uri));
//        }

        string attributeIdentifier = "";
        adjustReference(elementName, newElement, attributeIdentifier);

        multimap <string, virtualId>::iterator it = _elementsNotYetInserted.find(attributeIdentifier);

        if (it != _elementsNotYetInserted.end()){ //se alguém tiver feito referência a esse elemento antes dele ser inserido
            //coloca-se tal elemento na lista dos elementos que referenciam
            do{
                newElement.addReference((*it).second);
                it ++;
            }while (it != _elementsNotYetInserted.upper_bound(attributeIdentifier));

            _elementsNotYetInserted.erase(attributeIdentifier);
        }

        _modelElements[newElement.id()] = newElement;
        _nonIncrementalMarkedElements.insert(newElement.id());


        _markedElements.insert(newElement.id());



        return newElement.id();
    }
    return "";

}

void Model::adjustReference(string elementName, ModelElement & newElement, string & attributeIdentifier)
{
    if (elementName == "bind" || elementName == "bindParam" || elementName == "linkParam"){

        if (elementName == "bind"){

            Attribute attrComponent = newElement.attribute("component");
            Attribute attrInterface = newElement.attribute("interface");

            if (attrComponent.name() != ""){
                vector <ModelElement *> el = elementByIdentifier(attrComponent.value()); //podem ter mais de um elemento com o mesmo id
                if (el.size() == 0){
                    _elementsNotYetInserted.insert(pair <string,virtualId> (attrComponent.value(), newElement.id()));
                    if (attrInterface.name() != "")
                        _elementsNotYetInserted.insert(pair <string,virtualId> (attrComponent.value() + "/" + attrInterface.value(),
                                                                                newElement.id()));
                }
                else{
                    for (size_t i = 0; i < el.size(); i++){
                        ModelElement * e = el[i];
                        e->addReference(newElement.id());

                        if (attrInterface.name() == "") continue;

                        vector <virtualId> children = e->children();

                        for (size_t j = 0; j < children.size(); j++){
                            ModelElement *child = this->element(children[j]);

                            if (!child) continue;
                            Attribute attribute ("", "");

                            if (e->elementName() == "media"){
                                if (child->elementName() == "property")
                                    attribute = child->attribute("name");
                                else if (child->elementName() == "area")
                                    attribute = child->attribute("id");

                            }
                            else if (e->elementName() == "context"){
                                if (child->elementName() == "port")
                                    attribute = child->attribute("id");
                                else if (child->elementName() == "property")
                                    attribute = child->attribute("name");
                            }
                            else if (e->elementName() == "switch" && child->elementName() == "switchPort")
                                attribute = child->attribute("id");


                            if (attribute.name() != "" && attribute.value() == attrInterface.value())
                                child->addReference(newElement.id());
                        }
                    }
                }
            }
        }
    }
    else{
        vector <Attribute> attributes = newElement.attributes();
        for (size_t i = 0; i < attributes.size(); i++){
            if (attributes[i].name() == "id" || attributes[i].name() == "alias"){
                _idToElement.insert(pair <string, virtualId> (attributes[i].value(), newElement.id())); //se tiver id, coloca no mapa de id's
                attributeIdentifier = attributes[i].value();
            }

            if (Langstruct::isAttributeReferenceDependent(elementName, attributes[i].name())){ //se o atributo faz referência a outro elemento

                vector <ModelElement *> elements;
                string attributeValue = attributes[i].value();
                string toInsert = attributeValue;

                int index = attributeValue.find("#");
                if ( index == string::npos){
                    elements = elementByIdentifier(attributeValue);
                }
                else{
                     string alias = attributeValue.substr(0, index);
                     toInsert = alias;
                     elements = elementByIdentifier(alias);
                }

                if (elements.size() == 0){
                    _elementsNotYetInserted.insert(pair <string, virtualId> (toInsert, newElement.id()));
                }
                else{
                    for (size_t i = 0; i < elements.size(); i++) {
//                        cout << "el name: " << elements[i]->elementName() << endl;
                        elements[i]->_referencesToMyself.push_back(newElement.id());
                    }
                }
            }
        }
    }

}

set <virtualId> Model::affectedElements(){
    set <virtualId>::iterator it = _markedElements.begin();

    for ( ; it != _markedElements.end(); it++){
        ModelElement *e = element (*it);
        if (e)
            for (size_t i = 0; i < e->references().size(); i++)
                _affectedEllements.insert(e->references()[i]);

    }

    for (set<virtualId>::iterator it = _markedElements.begin(); it != _markedElements.end(); it++)
        _affectedEllements.erase(*it);

//    std::cout <<"affectedElements: " << _affectedEllements.size() << endl;
    return _affectedEllements;
}

vector <ModelElement *>  Model::elementByIdentifier(string identifier){
    vector <ModelElement *> elements;

    int index = int (identifier.find("#"));
    if (index == string::npos){ //se n�o tiver alias
        multimap <string, virtualId>::iterator it = _idToElement.find(identifier);
        if (it != _idToElement.end()){
            do{
                ModelElement *e = this->element ((*it).second);
                if (e)
                    elements.push_back(e);
                it ++;
            }while (it != _idToElement.upper_bound(identifier));
        }
    }

    else{
        string alias = identifier.substr(0, index);
        string id =  identifier.substr(index + 1);
        ModelElement *baseImported = 0;

        multimap<string, virtualId>::iterator it;
        it = _idToElement.find(alias);

        if (it == _idToElement.end())
            return elements; // no elements associated with key, so return immediately
        baseImported = this->element(it->second);

        if (baseImported){
            string path = baseImported->attribute("documentURI").value();

            ModelElement elementFound;
            QXmlContentHandler* docHandler = new TextualParser(&elementFound, id);

            findElementInImportedFile(path, docHandler);

            if (elementFound.elementName() != "" /*&& elementFound.elementName() != "causalConnector"*/){
                _importedElements.push_back(elementFound);
//                fprintf  (stderr, "%s\n\n", _importedElements.back().elementName().c_str());
                elements.push_back(& _importedElements.back());
            }

        }

    }


    return elements;
}


void Model::findElementInImportedFile(string fileName, QXmlContentHandler *docHandler){
    QFile ifile (QString::fromStdString(fileName));
//    ifstream ifile (fileName.c_str());
    if (!ifile.exists()){
        fileName = _relativePath + fileName;

        QFile ifile2 (QString::fromStdString(fileName));
//        ifstream ifile2 (fileName.c_str());
        if (!ifile2.exists())
            return;

        ifile.setFileName(QString::fromStdString(fileName));

    }

//    fprintf(stderr, "\n\n%s %s\n\n", fileName.c_str(), idToFind.c_str());
//    fprintf (stderr, "\n%s\n\n", fileName.c_str());


//    ModelElement el;

    QXmlSimpleReader reader;
    reader.setContentHandler(docHandler);

    reader.parse(QXmlInputSource (&ifile));

//    try {
//        XMLPlatformUtils::Initialize();
//    }
//    catch (const XMLException& toCatch) {
//        char* message = XMLString::transcode(toCatch.getMessage());
//        cerr << "Error during initialization! :\n"
//             << message << "\n";
//        XMLString::release(&message);
//    }

//    xercesc::SAXParser* parser = new SAXParser();
////    TextualParser* docHandler = new TextualParser(&el, idToFind);
//    xercesc::ErrorHandler* errHandler = (ErrorHandler*) docHandler;
//    parser->setDocumentHandler(docHandler);
//    parser->setErrorHandler(errHandler);

//    try {
//        parser->parse(fileName.c_str());
//    }
//    catch (const XMLException& toCatch) {
//        char* message = XMLString::transcode(toCatch.getMessage());
//        fprintf(stderr, "\nXMLException message is: %s\n", message);
//        XMLString::release(&message);
//    }
//    catch (const SAXParseException& toCatch) {
//        char* message = XMLString::transcode(toCatch.getMessage());
//        fprintf(stderr, "\nSAXParseException message is: %s\n", message);
//        XMLString::release(&message);
//    }

//    fprintf(stderr, "\nElement: %s\n\n", el.elementName().c_str());

//    XMLPlatformUtils::Terminate();
}

vector <ModelElement *> Model::elementByPropertyName(string component, string propertyName){
    multimap <string, virtualId>::iterator it = _idToElement.find(component);
    vector <ModelElement *> elements;
    if (it != _idToElement.end()){
        do{
            ModelElement *e = this->element ((*it).second);
            if (e){
                vector <virtualId> children = e->children();
                for (int i = 0; i < children.size(); i++){
                    ModelElement *child =  this->element(children[i]);
                    if (!child || child->elementName() != "property") continue;

                    Attribute attribute = child->attribute("name");
                    if (attribute.value() == propertyName)
                        elements.push_back(child);
                }
            }
            it ++;
        }while (it != _idToElement.upper_bound(component));
    }




    return elements;
}

void Model::parseConnectorChild (ModelElement *child, map <string, pair <int, int> > &roles){
    if (!child)
        return;

    if (child->elementName() == "simpleAction" || child->elementName() == "simpleCondition"
            || child->elementName() == "attributeAssessment"){
        string role = child->attribute("role").value();

        if (role != ""){
            stringstream ss;
            Attribute minAttr = child->attribute("min");
            Attribute maxAttr = child->attribute("max");

            int min = 1;
            int max = 1;

            if (minAttr.value() != ""){
                ss << minAttr.value();
                ss >> min;

                if (!ss)
                    min = 1;
                ss.clear();
            }
            if (maxAttr.value() != ""){
                if (maxAttr.value() == "unbounded"){
                    max = (int) numeric_limits <int>::max ();
                }
                else{
                    ss << maxAttr.value();
                    ss >> max;

                    if (!ss)
                        max = 1;
                }
            }
            roles[role] = make_pair (min, max);
        }
    }
}


void Model::parseAllConnectorChildren (ModelElement *connector, ModelElement *element,
                                       map <string, pair <int, int> > &roles){
    vector<virtualId> children = element->children();
    for (int i = 0; i < children.size(); i++){
        ModelElement *child = this->element(children[i]);
        if (child){
            parseConnectorChild(child, roles);

            parseAllConnectorChildren(connector, child, roles);
        }
    }
}

map<string, pair<int, int> > Model::getConnectorSetRoles (string identifier){
    int index = identifier.find("#");
    map <string, pair <int, int> > roles;
    if (index == string::npos){

//        fprintf (stderr, "\n\n%s", identifier.c_str());
        vector <ModelElement *> els = elementByIdentifier(identifier);

        if (els.size() == 0){
            map<string, pair<int, int> > temp;
            return (temp);
        }

        ModelElement *connector = els[0];

        parseAllConnectorChildren(connector, connector, roles);

    }
    else{
        string alias = identifier.substr(0, index);
        string id =  identifier.substr(index + 1);
        ModelElement *baseImported = 0;

        multimap<string, virtualId>::iterator it;
        it = _idToElement.find(alias);

        if (it == _idToElement.end())
            return roles; // no elements associated with key, so return immediately

        baseImported = this->element(it->second);

        if (baseImported){
            string path = baseImported->attribute("documentURI").value();
            QXmlContentHandler* docHandler = new ConnectorParser (this, id);


            findElementInImportedFile(path, docHandler);
            ConnectorParser *connParser = dynamic_cast <ConnectorParser *> (docHandler);
            roles = connParser->getRolesMap ();

            for (map<string, pair <int, int> >::iterator it = roles.begin(); it != roles.end(); it++ )
                fprintf (stderr, "%s %d %d\n", it->first.c_str(), it->second.first, it->second.second);

        }
    }
    return roles;
}


virtualId Model::addChild(virtualId parentId, string elementName, vector <Attribute> attributes){
    virtualId childId = addElement(elementName, attributes);
    if (childId != ""){
        ModelElement *parentElement = element(parentId);
        ModelElement *childElement = element (childId);

        addChild (parentElement, childElement);
    }

    return childId;
}

void Model::addChild (virtualId parent, virtualId child){
    ModelElement *parentElement = element (parent);
    ModelElement *childElement = element (child);

    //	cout << parentElement->attribute("id").value() << endl;

    if (parentElement && childElement)
        addChild (parentElement, childElement);
}

void Model::addChild(ModelElement *parent, ModelElement *child){
    parent->addChild(child->id());
    child->setParent(parent->id());

    if (parent->elementName() == "causalConnector")
        _markedElements.insert (parent->id());
    else if (parent->elementName() == "simpleAction" || parent->elementName() == "simpleCondition" ||
             parent->elementName() == "compoundAction" || parent->elementName() == "compoundCondition" ||
             parent->elementName() == "assessmentStatement" || parent->elementName() == "attributeAssessment" ||
             parent->elementName() == "connectorParam" || parent->elementName() == "compoundStatement" ||
             parent->elementName() == "valueAssessment"){

        _markedElements.erase(parent->id());

        ModelElement *element = this->element(parent->parent());

        while (element && element->elementName() != "causalConnector"){
            _markedElements.erase (element->id());
            element = this->element(element->parent());
        }
        if (element && element->elementName() == "causalConnector")
            _markedElements.insert(element->id());

    }
    if (parent->elementName() == "link")
        _markedElements.insert(parent->id());
    else if (parent->elementName() == "bind"){
        ModelElement *element = this->element(parent->parent());

        while (element && element->elementName() != "link"){
            _markedElements.erase (element->id());
            element = this->element(element->parent());
        }
    }

    if (child->elementName() == "property" || child->elementName() == "area" ||
            child->elementName() == "switchPort" || child->elementName() == "port"){
        Attribute parentIdAttribute = parent->attribute("id");
        if (parentIdAttribute.name() != ""){
            string toFind = parentIdAttribute.value() + "/";

            Attribute childIdAttribute = child->attribute("id");
            if (childIdAttribute.name() == "")
                childIdAttribute = child->attribute("name");

            if (childIdAttribute.name() != "")
                toFind+= childIdAttribute.value();

            multimap <string, virtualId>::iterator it = _elementsNotYetInserted.find(toFind);

            if (it != _elementsNotYetInserted.end()){
                do{
                    child->addReference((*it).second);
                    it ++;
                }while (it != _elementsNotYetInserted.upper_bound(toFind));

                _elementsNotYetInserted.erase(toFind);
            }
        }
    }




    adjustScope(parent->id(), child->id());
}

void Model::adjustScope (virtualId parentId, virtualId childId){
    ModelElement *parentElement = element (parentId);
    ModelElement *childElement = element (childId);

    try {
        if (Langstruct::defineScope(parentElement->elementName()))
            childElement->setScope(parentId);
        else
            childElement->setScope(parentElement->scope());

        vector <virtualId> children = childElement->children();
        //		cout << children.size () << endl;
        for (size_t i = 0; i < children.size(); ++i){
            ModelElement * e = element(children[i]);
            if (e){
                if (e->scope() != "")
                    adjustScope(childId, e->id());
            }
        }
    }
    catch (...){
        cerr << "Element doesn't exist!";
    }
}

void Model::clear()
{
    _modelElements.clear();
    _markedElements.clear();
    _affectedEllements.clear();
    _elementsWithErrorInLastPass.clear();
    _idToElement.clear();
    _elementsNotYetInserted.clear();
}


}
