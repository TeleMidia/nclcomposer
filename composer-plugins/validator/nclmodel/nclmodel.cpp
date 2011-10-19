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

void ModelElement::addAttribute(vector <Attribute> &attributes){
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
//	cout << _id << ": " << _children.size() << endl;
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

//virtualId Model::addElement(ModelElement &element){
//	if (&element){
//		virtualId id = randomId();

//		while (_modelElements.count(id))
//			id.append ("" + 0);

//		setElementId(id, element);

//		ModelElement * parent = this->element(element.parent());
//		if (parent){
//			parent->addChild(element.id());
//			string scope = "";
//			if (Langstruct::defineScope(parent->elementName()))
//				scope = parent->scope() + "/" + parent->id();
//			else
//				scope = parent->scope();

//			element.setScope(scope);


//			for (size_t i = 0; i < element.children().size(); i++){
//				ModelElement *e = this->element(element.children()[i]);
//				if (e){
//					e->setScope(parent->scope() + "/" + e->id());
//				}
//			}
//		}
//		else{

//			element.setScope(element.id());
//		}

//		_modelElements[element.id()] = element;
//		_markedElements.insert(element.id());

//		return element.id();
//	}
//	return "";
//}

bool Model::editElement(virtualId &id, ModelElement &element){
	if (_modelElements.count(id)){
		removeElement(id);
//		addElement(element);

		return true;
	}
	return false;
}

bool Model::removeElement(virtualId &id){
	if (_modelElements.count(id)){
                ModelElement * element = this->element(id);
                ModelElement * parent = this->element(_modelElements[id].parent());
		if (parent)
                    parent->removeChild(id);


                map<virtualId, ModelElement>::iterator it = _modelElements.begin();

                for ( ; it != _modelElements.end(); it++){
                    ModelElement * el = &(it->second);
                    if (el->removeReference(id)){
                        std::cout << "Faz referencia: " << el->elementName() << endl;
                        _markedElements.insert(el->id());
                    }
                }

                for (int i=0; i < element->children().size(); i++){
                    ModelElement *child = this->element(element->children()[i]);
                    child->setParent(element->parent());
                }

		_markedElements.erase(id);
		_modelElements.erase(id);
		return true;
	}
	return false;
}

ModelElement * Model::element(const virtualId & id){
	if (_modelElements.count(id))
		return &_modelElements[id];
	else{
		return 0;
	}
}

void Model::print(){
	map<virtualId, ModelElement>::const_iterator iterator = _modelElements.begin();

	for ( ; iterator != _modelElements.end(); iterator ++){
		cout << "Element: " << (*iterator).second.elementName() << endl;
//
		cout << "VirtualID: " << (*iterator).second.id() << endl;
		cout << "Perspective: " << (*iterator).second.scope() << endl;

		ModelElement * parent = element((*iterator).second.parent());

		if (parent){

			cout << "Parent: " << parent->elementName() << endl;
			cout << "Parent virtualId: " << parent->id() << endl;
		}

		cout << "Children: ";
		vector <virtualId> children = (*iterator).second.children();
		for (size_t i = 0; i < children.size(); i++)
				cout << children[i] << " ";
		cout << "\nReferences: ";
		vector <virtualId> references = (*iterator).second.references();
		for (size_t i = 0; i < references.size(); i++)
				cout << references[i] << " ";


		cout << "\n*********************************" << endl;
	}
	cout << "\nelementsNotYetInserted size: " << _elementsNotYetInserted.size() << endl;

	for( map<string,virtualId>::iterator ii=_elementsNotYetInserted.begin(); ii!=_elementsNotYetInserted.end(); ++ii)
	{
		cout << (*ii).first << " ";
	}
	cout << endl;

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
		newElement.addAttribute(attributes);
		newElement.setParent("");
		newElement.setScope("");

		if (elementName == "ncl")
			newElement.setScope(newElement.id());

		string attributeIdentifier = "";

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
		else
			for (size_t i = 0; i < attributes.size(); i++){
				if (attributes[i].name() == "id" || attributes[i].name() == "alias"){
//					cerr << "ta inserindo!" << endl;
					_idToElement.insert(pair <string, virtualId> (attributes[i].value(), newElement.id())); //se tiver id, coloca no mapa de id's
					attributeIdentifier = attributes[i].value();
				}

				if (Langstruct::isAttributeReferenceDependent(elementName, attributes[i].name())){ //se o atributo faz referência a outro elemento
					vector <ModelElement *> elements = elementByIdentifier(attributes[i].value());

					if (elements.size() == 0){
						_elementsNotYetInserted.insert(pair <string, virtualId> (attributes[i].value(), newElement.id()));
					}
					else{
						cout << "not empy" << endl;
						for (size_t i = 0; i < elements.size(); i++) {
							cout << "el name: " << elements[i]->elementName() << endl;
							elements[i]->_referencesToMyself.push_back(newElement.id());
						}
					}
				}
			}



		multimap <string, virtualId>::iterator it = _elementsNotYetInserted.find(attributeIdentifier);

		if (it != _elementsNotYetInserted.end()){ //se alguém tiver feito referência a esse elemento antes dele ser inserido
												  //coloca-se tal elemento na lista dos elementos que referenciam
			do{
				newElement.addReference((*it).second);
				it ++;
			}while (it != _elementsNotYetInserted.upper_bound(attributeIdentifier));

			_elementsNotYetInserted.erase(attributeIdentifier);
		}



//			newElement.addReference(_elementsNotYetInserted[attributeIdentifier]);


		_modelElements[newElement.id()] = newElement;
		_markedElements.insert(newElement.id());
		return newElement.id();
	}
	return "";

}

set <virtualId> Model::affectedElements(){
	set <virtualId> affected;

	set <virtualId>::iterator it = _markedElements.begin();

	for ( ; it != _markedElements.end(); it++){
		ModelElement *e = element (*it);
		if (e)
			for (size_t i = 0; i < e->references().size(); i++)
				affected.insert(e->references()[i]);

	}
        std::cout <<"affectedElements: " << affected.size() << endl;
	return affected;
}

vector <ModelElement *>  Model::elementByIdentifier(string identifier){
	multimap <string, virtualId>::iterator it = _idToElement.find(identifier);

	vector <ModelElement *> elements;

	if (it != _idToElement.end()){
		do{
			ModelElement *e = this->element ((*it).second);
			if (e)
				elements.push_back(e);
			it ++;
		}while (it != _idToElement.upper_bound(identifier));
	}

	return elements;
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


//	if (Langstruct::defineScope(parent->elementName()))
//		child->setScope(parent->id());
//	else
//		child->setScope(parent->scope());
//
//	vector <virtualId> children = child->children();
//	for (size_t i = 0; i < children.size(); ++i){
//		ModelElement * e = element(children[i]);
//		if (e)
//			adjustScope(child->id(), e->id());
//	}
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

}
