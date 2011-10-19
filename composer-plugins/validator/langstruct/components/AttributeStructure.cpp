/*
 * AttributeStructure.cpp
 *
 *  Created on: 09/08/2011
 *      Author: Mária Gonçalves
 */

#include "AttributeStructure.h"

namespace nclValidator {

AttributeStructure::AttributeStructure() {}


AttributeStructure::AttributeStructure(const string &_name) {
	name = _name;
}


AttributeStructure::~AttributeStructure() {}


void AttributeStructure::addElement(const string &_parent, bool _required) {
	elements[_parent] = _required;
}


void AttributeStructure::addRegex (const string &regex, const string &element) {
	datatypes[element] = regex;
}


string AttributeStructure::getName() const
{
    return name;
}

map<string,bool> AttributeStructure::getElements() const
{
    return elements;
}

string AttributeStructure::getDatatype(const string &element) const
{
    return datatypes.at (element);
}

}
