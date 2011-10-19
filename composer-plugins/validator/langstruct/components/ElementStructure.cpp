/*
 * ElementStructure.cpp
 *
 *  Created on: 08/08/2011
 *      Author: Mária Gonçalves
 */

#include "ElementStructure.h"


namespace nclValidator {

ElementStructure::ElementStructure() {}


/*
 * 
 */
ElementStructure::ElementStructure(string _name, bool _defineScope) {
    name = _name;
    defineScope = _defineScope;
}


ElementStructure::~ElementStructure() {}


/*
 * Add an attribute to element
 */
void ElementStructure::addAtt (string attName, bool required) {
    atts[attName] = required;
}


/*
 * Add a parent to element
 */
void ElementStructure::addParent (string _parent, char _cardinality) {
    parents[_parent] = _cardinality;
}


/*
 * Get element name
 */
string ElementStructure::getName() const {
    return name;
}


/*
 * Get element parents
 */
map<string, char> ElementStructure::getParents() const {
    return parents;
}


/*
 */
char ElementStructure::getCardinality(string _parent) {
    return parents[_parent];
}


/*
 *
 */
map<string, bool> ElementStructure::getAtts() const {
    return atts;
}


/*
 *
 */
bool ElementStructure::isScopeDefined() const {
    return defineScope;
}

}
