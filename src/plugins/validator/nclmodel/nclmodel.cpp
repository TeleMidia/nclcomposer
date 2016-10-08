/*
 * Copyright 2011-2013 Laws/UFMA.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "nclmodel.h"
#include <QDebug>

/***********************ModelElement's definition*****************************/
ModelElement::ModelElement(virtualId parentId)
{
  _parentId = parentId;
}

ModelElement::~ModelElement (){}

void ModelElement::addAttribute(string name, string value)
{
  Attribute attr (name, value);
  _attributes.push_back (attr);
}

void ModelElement::addAttribute(Attribute &attr)
{
  _attributes.push_back (attr);

}

void ModelElement::setAttributes(vector <Attribute> &attributes)
{
  _attributes = attributes;
}

void ModelElement::setElementName(string name)
{
  _elementName.assign(name);
}

void ModelElement::setParent(virtualId parent)
{
  _parentId = parent;
}

void ModelElement::addChild(virtualId childId)
{
  _children.push_back(childId);
}

bool ModelElement::removeReference(virtualId id)
{
  for (size_t i=0; i < _referencesToMyself.size(); i++)
    if (_referencesToMyself[i] == id)
    {
      _referencesToMyself.erase(_referencesToMyself.begin() + i);
      return true;
    }
  return false;
}

void ModelElement::removeChild(virtualId childId)
{
  for (size_t i=0; i < _children.size(); i++)
    if (childId == _children[i])
    {
      _children.erase (_children.begin () + i);
      break;
    }
}

Attribute ModelElement::attribute(string attribute) const
{
  Attribute attr ("", "");
  for (size_t i = 0; i < _attributes.size(); i++)
    if (_attributes[i].name() == attribute)
      attr = _attributes[i];
  return attr;
}

/***********************Model's definition*****************************/
Model::Model()
{
  _seed = 1;
}

Model::~Model(){}

virtualId Model::randomId ()
{
  string s("vId");
  stringstream oss;
  oss << s.c_str() << _seed ++;
  return oss.str ();
}

void Model::setElementId (virtualId id, ModelElement & element)
{
  if (&element)
    element._id = id;
}


bool Model::editElement(virtualId &id, vector <Attribute> &newAttributes)
{
  ModelElement *elementEdited = this->element(id);
  if (elementEdited)
  {
    vector<Attribute> oldAttributes = elementEdited->attributes();
    string oldId = "";
    string newId = "";

    for (size_t i = 0; i < elementEdited->references().size(); i++)
      _affectedEllements.insert(elementEdited->references().at(i));

    _markedElements.insert(id);

    for (size_t i=0; i < oldAttributes.size(); i++)
    {
      Attribute oldAttribute = oldAttributes[i];

      if (oldAttribute.name() == "id" || oldAttribute.name() == "alias")
        oldId = oldAttribute.value();

      if(Langstruct::isAttributeReferenceDependent(elementEdited->elementName(),
                                                   oldAttribute.name()))
      {
        vector <ModelElement *> references = elementsByIdentifier(
              oldAttribute.value());

        if (references.size() > 0)
          for (size_t j = 0; j < references.size(); j++)
          {
            ModelElement *ref = references[j];
            ref->removeReference(id);
          }
        else
        {
          multimap<string, virtualId>::iterator it =
              _elementsNotYetInserted.find(oldAttribute.value());

          if (it != _elementsNotYetInserted.end())
          {
            do
            {
              if ((*it).second == id)
              {
                _elementsNotYetInserted.erase(it);
                break;
              }
              it++;
            }
            while (it != _elementsNotYetInserted.upper_bound(
                     oldAttribute.value()));
          }
        }
      }
    }


    for (size_t i=0; i < newAttributes.size(); i++)
    {
      Attribute newAttribute = newAttributes[i];

      if ((newAttribute.name() == "id" || newAttribute.name() == "alias")
          && newAttribute.value() != oldId)
      {
        elementEdited->_referencesToMyself.clear();
        newId = newAttribute.value();

        for (map<virtualId, ModelElement>::iterator it = _modelElements.begin();
             it != _modelElements.end(); it++)
        {
          ModelElement *el = &(it->second);

          if (el == elementEdited) continue;

          if (el)
          {
            vector<Attribute> elAttr = el->attributes();

            for (size_t j =0; j < elAttr.size(); j++)
            {
              if (Langstruct::isAttributeReferenceDependent(el->elementName(),
                                                            elAttr[j].name()))
              {
                if (elAttr[j].value() == newId)
                {
                  elementEdited->addReference(el->id());
                }
              }
            }
          }
        }
      }
    }

    multimap <string, virtualId>::iterator it = _idToElement.find(oldId);
    if (it != _idToElement.end())
    {
      do
      {
        if ((*it).second == elementEdited->id())
        {
          _idToElement.erase(it);
          break;
        }
        it ++;
      }
      while (it != _idToElement.upper_bound(oldId));
    }


    it = _elementsNotYetInserted.find(newId);
    if (it != _elementsNotYetInserted.end())
    {
      do
      {
        elementEdited->addReference((*it).second);
        it ++;
      }
      while (it != _elementsNotYetInserted.upper_bound(newId));
    }

    _elementsNotYetInserted.erase(newId);

    elementEdited->setAttributes(newAttributes);
    string dumb = "";
    adjustReference(elementEdited->elementName(), *elementEdited, dumb);

    if (elementEdited->elementName() == "simpleAction" ||
        elementEdited->elementName() == "simpleCondition" ||
        elementEdited->elementName() == "compoundAction" ||
        elementEdited->elementName() == "compoundCondition" ||
        elementEdited->elementName() == "assessmentStatement" ||
        elementEdited->elementName() == "attributeAssessment" ||
        elementEdited->elementName() == "connectorParam" ||
        elementEdited->elementName() == "compoundStatement" ||
        elementEdited->elementName() == "valueAssessment")
    {
      ModelElement *parent = this->element(elementEdited->parent());
      while (parent)
      {
        if (parent->elementName() == "causalConnector")
        {
          _markedElements.insert(parent->id());
          for (size_t i = 0; i < parent->references().size(); i++)
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

bool Model::removeElement(virtualId &id)
{
  if (_modelElements.count(id))
  {
    ModelElement * element = this->element(id);
    ModelElement * parent = this->element(element->parent());

    if (parent)
    {
      parent->removeChild(id);

      if (element->elementName() == "simpleCondition" ||
          element->elementName() == "simpleAction" ||
          element->elementName() == "attributeAssessment")
      {
        while (parent)
        {
          if (parent->elementName() == "causalConnector")
          {
            _markedElements.insert(parent->id());
            break;
          }
          parent = this->element(parent->parent());
        }
      }
      else if (element->elementName() == "bind")
        _markedElements.insert(parent->id());


      _markedElements.insert(parent->id());
    }

    for (size_t i = 0; i < element->references().size(); i++)
      _affectedEllements.insert(element->references().at(i));

    vector<Attribute> attrs = element->attributes();
    for (size_t i = 0; i < attrs.size(); i++)
    {
      Attribute attribute = attrs[i];

      if (attribute.name() == "id" || attribute.name() == "alias")
      {
        multimap<string, virtualId>::iterator it =
            _idToElement.find(attribute.value());

        if (it != _idToElement.end())
          _idToElement.erase(it);
      }

      if (Langstruct::isAttributeReferenceDependent(element->elementName(),
                                                    attribute.name()))
      {
        vector <ModelElement *> els = elementsByIdentifier(attribute.value());
        for (size_t j = 0; j<els.size(); j++)
        {
          ModelElement * e = els[j];
          e->removeReference(id);
        }
      }
    }

    for (size_t i = 0; i < element->children().size(); i++)
    {
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
  for (set<virtualId>::iterator it = _elementsWithErrorInLastPass.begin();
       it != _elementsWithErrorInLastPass.end(); )
  {
    if (_markedElements.count(*it))
    {
      _elementsWithErrorInLastPass.erase(it++);
    }
    else
    {
      ++it;
    }
  }

  return _elementsWithErrorInLastPass;
}

ModelElement * Model::element(const virtualId & id)
{
  if (_modelElements.count(id))
  {
    return &_modelElements[id];
  }
  else
  {
    return 0;
  }
}

void Model::print()
{
}

//***************************Novos métodos********************************

virtualId Model::addElement(string elementName, vector <Attribute> attributes)
{
  if (elementName != "")
  {
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

    string attributeIdentifier = "";
    adjustReference(elementName, newElement, attributeIdentifier);

    multimap <string, virtualId>::iterator it =
        _elementsNotYetInserted.find(attributeIdentifier);

    if (it != _elementsNotYetInserted.end())
    { //se alguÃ©m tiver feito referÃªncia a esse elemento antes dele ser
      //inserido coloca-se tal elemento na lista dos elementos que referenciam
      do
      {
        newElement.addReference((*it).second);
        it ++;
      }
      while (it != _elementsNotYetInserted.upper_bound(attributeIdentifier));

      _elementsNotYetInserted.erase(attributeIdentifier);
    }

    _modelElements[newElement.id()] = newElement;
    _nonIncrementalMarkedElements.insert(newElement.id());

    _markedElements.insert(newElement.id());

    return newElement.id();
  }
  return "";

}

void Model::adjustReference(string elementName, ModelElement & newElement,
                            string & attributeIdentifier)
{
  if (elementName == "bind" || elementName == "bindParam" ||
      elementName == "linkParam")
  {

    if (elementName == "bind")
    {
      Attribute attrComponent = newElement.attribute("component");
      Attribute attrInterface = newElement.attribute("interface");

      if (attrComponent.name() != "")
      {
        //podem ter mais de um elemento com o mesmo id
        vector <ModelElement *> el =elementsByIdentifier(attrComponent.value());
        if (el.size() == 0)
        {
          _elementsNotYetInserted.insert(pair <string,virtualId>
                                         (attrComponent.value(),
                                          newElement.id()));
          if (attrInterface.name() != "")
            _elementsNotYetInserted.insert(pair <string,virtualId> (attrComponent.value() + "/" + attrInterface.value(),
                                                                    newElement.id()));
        }
        else
        {
          for (size_t i = 0; i < el.size(); i++)
          {
            ModelElement * e = el[i];
            e->addReference(newElement.id());

            if (attrInterface.name() == "") continue;

            vector <virtualId> children = e->children();

            for (size_t j = 0; j < children.size(); j++)
            {
              ModelElement *child = this->element(children[j]);

              if (!child) continue;
              Attribute attribute ("", "");

              if (e->elementName() == "media")
              {
                if (child->elementName() == "property")
                  attribute = child->attribute("name");
                else if (child->elementName() == "area")
                  attribute = child->attribute("id");

              }
              else if (e->elementName() == "context")
              {
                if (child->elementName() == "port")
                  attribute = child->attribute("id");
                else if (child->elementName() == "property")
                  attribute = child->attribute("name");
              }
              else if (e->elementName() == "switch" &&
                       child->elementName() == "switchPort")
                attribute = child->attribute("id");


              if (attribute.name() != "" &&
                  attribute.value() == attrInterface.value())
                child->addReference(newElement.id());
            }
          }
        }
      }
    }
  }
  else
  {
    vector <Attribute> attributes = newElement.attributes();
    for (size_t i = 0; i < attributes.size(); i++)
    {
      if (attributes[i].name() == "id" || attributes[i].name() == "alias")
      {
        //se tiver id, coloca no mapa de id's
        _idToElement.insert(pair <string, virtualId> (attributes[i].value(),
                                                      newElement.id()));
        attributeIdentifier = attributes[i].value();
      }

      //se o atributo faz referÃªncia a outro elemento
      if (Langstruct::isAttributeReferenceDependent(elementName,
                                                    attributes[i].name()))
      {
        vector <ModelElement *> elements;
        string attributeValue = attributes[i].value();
        string toInsert = attributeValue;

        size_t index = attributeValue.find("#");
        if ( index == string::npos)
        {
          elements = elementsByIdentifier(attributeValue);
        }
        else
        {
          string alias = attributeValue.substr(0, index);
          toInsert = alias;
          elements = elementsByIdentifier(alias);
        }

        if (elements.size() == 0)
        {
          _elementsNotYetInserted.insert(pair <string, virtualId> (
                                           toInsert, newElement.id()));
        }
        else
        {
          for (size_t i = 0; i < elements.size(); i++)
          {
            elements[i]->_referencesToMyself.push_back(newElement.id());
          }
        }
      }
    }
  }

}

set <virtualId> Model::affectedElements()
{
  set <virtualId>::iterator it = _markedElements.begin();

  for ( ; it != _markedElements.end(); it++)
  {
    ModelElement *e = element (*it);
    if (e)
      for (size_t i = 0; i < e->references().size(); i++)
        _affectedEllements.insert(e->references()[i]);
  }

  for (set<virtualId>::iterator it = _markedElements.begin();
       it != _markedElements.end(); it++)
    _affectedEllements.erase(*it);

  //    std::cout <<"affectedElements: " << _affectedEllements.size() << endl;
  return _affectedEllements;
}

vector <ModelElement *>  Model::elementsByIdentifier(string identifier)
{
  vector <ModelElement *> elements;

  size_t index = int (identifier.find("#"));

  //se não tiver alias
  if (index == string::npos)
  {
    multimap <string, virtualId>::iterator it = _idToElement.find(identifier);
    if (it != _idToElement.end())
    {
      do
      {
        ModelElement *e = this->element ((*it).second);
        if (e)
          elements.push_back(e);
        it ++;
      }
      while (it != _idToElement.upper_bound(identifier));
    }
  }
  else
  {
    string alias = identifier.substr(0, index);
    string id =  identifier.substr(index + 1);
    ModelElement *baseImported = 0;

    multimap<string, virtualId>::iterator it;
    it = _idToElement.find(alias);

    if (it == _idToElement.end())
      return elements; // no elements associated with key, so return immediately
    baseImported = this->element(it->second);

    if (baseImported)
    {
      string path = baseImported->attribute("documentURI").value();

      ModelElement elementFound;
      QXmlContentHandler* docHandler = new TextualParser(&elementFound, id);

      findElementInImportedFile(path, docHandler);

      if (elementFound.elementName() != "" )
      {
        _importedElements.push_back(elementFound);
        elements.push_back(& _importedElements.back());
      }

    }

  }
  return elements;
}

vector <ModelElement *> Model::elementsByName(string elementName)
{
  vector <ModelElement *> elements;

  map <virtualId, ModelElement>::iterator it = _modelElements.begin();
  for ( ; it != _modelElements.end(); ++it)
  {
    ModelElement *el = &((*it).second);
    if (el && el->elementName() == elementName)
      elements.push_back(el);
  }

  return elements;
}


void Model::findElementInImportedFile(string fileName,
                                      QXmlContentHandler *docHandler)
{
  QFile ifile (QString::fromStdString(fileName));
  //    ifstream ifile (fileName.c_str());
  if (!ifile.exists())
  {
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

vector <ModelElement *> Model::elementsByPropertyName(string component,
                                                      string propertyName)
{
  //    fprintf (stderr, "\n\n%s %s\n\n", component.c_str(), propertyName.c_str());
  multimap <string, virtualId>::iterator it = _idToElement.find(component);
  vector <ModelElement *> elements;
  if (it != _idToElement.end())
  {
    do
    {
      ModelElement *e = this->element ((*it).second);
      if (e)
      {
        vector <virtualId> children = e->children();
        for (size_t i = 0; i < children.size(); i++)
        {
          ModelElement *child =  this->element(children[i]);
          if (!child || child->elementName() != "property") continue;

          Attribute attribute = child->attribute("name");
          if (attribute.value() == propertyName)
            elements.push_back(child);
        }

        Attribute refer = e->attribute("refer");

        if (refer.value() != "")
        {
          multimap <string, virtualId>::iterator it2 =
              _idToElement.find(refer.value());

          if(it2 != _idToElement.end())
          {
            do
            {
              ModelElement *eRefer = this->element((*it2).second);
              printf("%p\n", eRefer);

              if (eRefer)
              {
                children = eRefer->children();
                for (size_t i = 0; i < children.size(); i++)
                {
                  ModelElement *child =  this->element(children[i]);
                  if (!child || child->elementName() != "property") continue;

                  Attribute attribute = child->attribute("name");
                  if (attribute.value() == propertyName)
                    elements.push_back(child);
                }
              }
              it2++;
            }
            while (it2 != _idToElement.upper_bound(refer.value()));
          }
        }
      }
      it ++;
    }
    while (it != _idToElement.upper_bound(component));
  }

  return elements;
}

void Model::parseConnectorChild (ModelElement *child, map <string,
                                 pair <int, int> > &roles)
{
  if (!child)
    return;

  if (child->elementName() == "simpleAction" ||
      child->elementName() == "simpleCondition" ||
      child->elementName() == "attributeAssessment")
  {
    string role = child->attribute("role").value();

    if (role != "")
    {
      stringstream ss;

      Attribute minAttr = child->attribute("min");
      Attribute maxAttr = child->attribute("max");

      int min = 1;
      int max = 1;

      if (minAttr.value() != "")
      {
        ss << minAttr.value().c_str();
        ss >> min;

        if (!ss)
          min = 1;
        ss.clear();
      }
      if (maxAttr.value() != "")
      {
        if (maxAttr.value() == "unbounded")
        {
          max = (int) numeric_limits <int>::max ();
        }
        else
        {
          ss << maxAttr.value().c_str();
          ss >> max;

          if (!ss)
            max = 1;
        }
      }
      roles[role] = make_pair (min, max);
    }
  }
}


void Model::parseAllConnectorChildren (ModelElement *connector,
                                       ModelElement *element,
                                       map <string, pair <int, int> > &roles)
{
  vector<virtualId> children = element->children();
  for (size_t i = 0; i < children.size(); i++)
  {
    ModelElement *child = this->element(children[i]);
    if (child)
    {
      parseConnectorChild(child, roles);

      parseAllConnectorChildren(connector, child, roles);
    }
  }
}

map<string, pair<int, int> > Model::getConnectorSetRoles (string identifier)
{
  map <string, pair <int, int> > roles;
  size_t index = identifier.find("#");

  if (index == string::npos)
  {
    vector <ModelElement *> els = elementsByIdentifier(identifier);

    if (els.size() == 0)
    {
      map<string, pair<int, int> > temp;
      return (temp);
    }

    ModelElement *connector = els[0];

    parseAllConnectorChildren(connector, connector, roles);
  }
  else
  {
    string alias = identifier.substr(0, index);
    string id =  identifier.substr(index + 1);
    ModelElement *baseImported = 0;

    multimap<string, virtualId>::iterator it;
    it = _idToElement.find(alias);

    if (it == _idToElement.end())
      return roles; // no elements associated with key, so return immediately

    baseImported = this->element(it->second);

    if (baseImported)
    {
      string path = baseImported->attribute("documentURI").value();
      QXmlContentHandler* docHandler = new ConnectorParser (this, id);


      findElementInImportedFile(path, docHandler);
      ConnectorParser *connParser = dynamic_cast<ConnectorParser *>(docHandler);
      roles = connParser->getRolesMap ();
    }
  }
  return roles;
}


virtualId Model::addChild(virtualId parentId, string elementName,
                          vector <Attribute> attributes)
{
  virtualId childId = addElement(elementName, attributes);
  if (childId != "")
  {
    ModelElement *parentElement = element(parentId);
    ModelElement *childElement = element (childId);

    addChild (parentElement, childElement);
  }

  return childId;
}

void Model::addChild (virtualId parent, virtualId child)
{
  ModelElement *parentElement = element (parent);
  ModelElement *childElement = element (child);

  if (parentElement && childElement)
    addChild (parentElement, childElement);
}

void Model::addChild(ModelElement *parent, ModelElement *child)
{
  parent->addChild(child->id());
  child->setParent(parent->id());

  if (parent->elementName() == "causalConnector")
    _markedElements.insert (parent->id());
  else if (parent->elementName() == "simpleAction" ||
           parent->elementName() == "simpleCondition" ||
           parent->elementName() == "compoundAction" ||
           parent->elementName() == "compoundCondition" ||
           parent->elementName() == "assessmentStatement" ||
           parent->elementName() == "attributeAssessment" ||
           parent->elementName() == "connectorParam" ||
           parent->elementName() == "compoundStatement" ||
           parent->elementName() == "valueAssessment")
  {
    _markedElements.erase(parent->id());

    ModelElement *element = this->element(parent->parent());

    while (element && element->elementName() != "causalConnector")
    {
      _markedElements.erase (element->id());
      element = this->element(element->parent());
    }
    if (element && element->elementName() == "causalConnector")
      _markedElements.insert(element->id());

  }
  if (parent->elementName() == "link")
    _markedElements.insert(parent->id());
  else if (parent->elementName() == "bind")
  {
    ModelElement *element = this->element(parent->parent());

    while (element && element->elementName() != "link")
    {
      _markedElements.erase (element->id());
      element = this->element(element->parent());
    }
  }

  if (child->elementName() == "property" || child->elementName() == "area" ||
      child->elementName() == "switchPort" || child->elementName() == "port")
  {
    Attribute parentIdAttribute = parent->attribute("id");
    if (parentIdAttribute.name() != "")
    {
      string toFind = parentIdAttribute.value() + "/";

      Attribute childIdAttribute = child->attribute("id");
      if (childIdAttribute.name() == "")
        childIdAttribute = child->attribute("name");

      if (childIdAttribute.name() != "")
        toFind+= childIdAttribute.value();

      multimap <string, virtualId>::iterator it =
          _elementsNotYetInserted.find(toFind);

      if (it != _elementsNotYetInserted.end())
      {
        do
        {
          child->addReference((*it).second);
          it ++;
        }
        while (it != _elementsNotYetInserted.upper_bound(toFind));

        _elementsNotYetInserted.erase(toFind);
      }
    }
  }

  adjustScope(parent->id(), child->id());
}

void Model::adjustScope (virtualId parentId, virtualId childId)
{
  ModelElement *parentElement = element (parentId);
  ModelElement *childElement = element (childId);

  try
  {
    if (Langstruct::defineScope(parentElement->elementName()))
      childElement->setScope(parentId);
    else
      childElement->setScope(parentElement->scope());

    vector <virtualId> children = childElement->children();
    //		cout << children.size () << endl;
    for (size_t i = 0; i < children.size(); ++i)
    {
      ModelElement * e = element(children[i]);
      if (e)
      {
        if (e->scope() != "")
          adjustScope(childId, e->id());
      }
    }
  }
  catch (...)
  {
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
