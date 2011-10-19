/*
 * langstruct.c
 *
 *  Created on: Aug 08, 2011
 *      Author: José Rios
 */

#include "langstruct.h"
#include "components/ElementStructure.h"
#include "components/AttributeStructure.h"
#include "components/ReferenceStructure.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <re2/re2.h>
#include <QDebug>
#include <QFile>

#include <assert.h>

namespace nclValidator {

map<string, ElementStructure> elements;
map<string, AttributeStructure> attributes;
vector<ReferenceStructure> references;
map<string, string> datatypes;


void showMaps () {
//
//    cout << "Elements map" << endl;
//
//    map<string, ElementStructure>::iterator it = elements.begin ();
//    for (; it != elements.end (); ++it) {
//        cout << "key: " << (*it).first << endl;
//        cout << "element: " << endl;
//        cout << "   name: " << (*it).second.getName() << endl;
//        cout << "   scope: " << (*it).second.isScopeDefined() << endl;
//        cout << "   atts:" << endl;
//
//        map<string, bool> atts = (*it).second.getAtts();
//        map<string, bool>::iterator itAtts = atts.begin();
//        for (; itAtts != atts.end (); ++itAtts) {
//            cout << "      name: " << (*itAtts).first << "         required: " << (*itAtts).second << endl;
//        }
//
//        cout << "   parents:" << endl;
//        map<string, string> parents = (*it).second.getParents();
//        map<string, string>::iterator itParent = parents.begin();
//        while (itParent != parents.end ()) {
//            cout << "      name: " << (*itParent).first << "         card: " << (*itParent).second << endl;
//            ++itParent;
//        }
//    }
//
//    cout << "Attributes map" << endl << endl;
//
//    map<string, AttributeStructure>::iterator it2 = attributes.begin();
//    for (; it2 != attributes.end (); ++it2) {
//        cout << "key: " << (*it2).first << endl;
//        cout << "attribute: " << endl;
//        cout << "   name: " << (*it2).second.getName() << endl;
//        cout << "   elements:" << endl;
//
//        map<string, bool> els = (*it2).second.getElements();
//        map<string, bool>::iterator itEls = els.begin();
//        for (; itEls != els.end (); ++itEls) {
//            cout << "      name: " << (*itEls).first << "         required: " << (*itEls).second
//            	 << "      regex: " << (*it2).second.getDatatype((*itEls).first) << endl;;
//        }
//    }

//    cout << "References map" << endl << endl;
//
//    vector<ReferenceStructure>::iterator it3 = references.begin();
//    for (; it3 != references.end (); ++it3) {
//        cout << "reference: " << endl;
//        cout << "   from: " << (*it3).getFrom() << endl;
//        cout << "   fromAtt: " << (*it3).getFromAtt() << endl;
//        cout << "   to: " << (*it3).getTo() << endl;
//        cout << "   toAtt: " << (*it3).getToAtt() << endl;
//        cout << "   perspective: " << (*it3).getPerspective() << endl;
//        cout << "   perspectiveAtt: " << (*it3).getPerspectiveAtt() << endl << endl;
//    }
//
//    fprintf (stdout, "DATATYPE map\n");
//    map<string, string>::iterator it4 = datatypes.begin();
//    for (; it4 != datatypes.end (); ++it4) {
//        fprintf (stdout, "datatype:\n");
//        fprintf (stdout, "name: %s\n",(*it4).first.c_str ());
//        fprintf (stdout, "regex: %s\n\n", (*it4).second.c_str());
//    }
//
//    fprintf (stdout, "end show maps\n");
}



/*
 */
void Langstruct::init ()
{
    QFile ifs (":/config/langstruct.in");
//    ifstream ifs (":/config/langstruct.in", ifstream::in);

    if (!ifs.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    qDebug() << "Langstruct passou primeiro if";
    
    istringstream strTok;
    
    string line, 
           component;

    qDebug() << "Begin readFile";
    while (!ifs.atEnd()) {
        line = QString (ifs.readLine()).toStdString();
//        qDebug() << "Begin readFile";
        std::cout << line;
//        qDebug() << QString (line[0]);
//        ifs >> line;
//        qDebug() << "End readFile";
        strTok.clear ();
        strTok.str(line);
        
        getline(strTok, component, '(');
        
        assert (component == "ELEMENT" || component == "ATTRIBUTE" 
                || component == "REFERENCE" || component == "DATATYPE");
        
        if (component == "ELEMENT") {
            string name,
                   parent,
                   cardinality,
                   scope;

            getline(strTok, name, ',');
            getline(strTok, parent, ',');
            getline(strTok, cardinality, ',');
            getline(strTok, scope, ')');            

            // Include an ElementStructure
            if (!elements.count (name)) {
                ElementStructure el (name, scope == "true");
                elements[name] = el;
            }
            
            elements[name].addParent(parent, cardinality[0]);
        }
        
        else if (component == "ATTRIBUTE") {
            string element,
                   name,
                   required,
                   datatype;
            
            getline(strTok, element, ',');
            getline(strTok, name, ',');
            getline(strTok, required, ',');
            getline(strTok, datatype, ')');

            // Include an AttributeStructure. All Elements must be already included
            assert (elements.count (element));
            if (!attributes.count (name)) {
                AttributeStructure att (name);
                attributes[name] = att;
            }

            elements[element].addAtt(name, required == "true");
            attributes[name].addElement(element, required == "true");
            attributes[name].addRegex(datatype, element);
        }
        
        else if (component == "REFERENCE") {
            string from,
                   fromAtt,
                   to,
                   toAtt,
                   perspective,
                   perspectiveAtt;

            getline(strTok, from, ',');
            getline(strTok, fromAtt, ',');
            getline(strTok, to, ',');
            getline(strTok, toAtt, ',');
            getline(strTok, perspective, ')');
            
            int persAttPos = perspective.find('.');
            if (persAttPos != perspective.npos) {                
                perspectiveAtt = perspective.substr(persAttPos + 1, perspective.size() - 1);
                perspective.erase (persAttPos, perspective.size () - 1);
            }                
            
            assert (elements.count (from));
            assert (attributes.count (fromAtt));
            assert (elements.count (to));
            assert (attributes.count (toAtt));

            ReferenceStructure ref = ReferenceStructure (from, fromAtt,
                    to, toAtt, perspective, perspectiveAtt);

            references.push_back (ref);              
        }
        
        // DATATYPE
        else {
            string datatype,
                   regex;

            getline(strTok, datatype, ',');
            getline(strTok, regex);
            regex.erase(regex.size () - 1);
//
//            RE2 re(regex);
//            assert(re.ok());  // compiled; if not, see re.error();

            datatypes[datatype] = regex;
        }
    }
    
    qDebug() << "End readFile";
    ifs.close();
    
    showMaps();
}


/*
 */
bool Langstruct::existElement(const string el)
{
    return elements.count (el);
}


/*
 */
bool Langstruct::existAttribute(const string element, const string att)
{
    if (!elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
        return false;
    }
    
    return elements[element].getAtts ().count (att) != 0;
}


/*
 */
bool Langstruct::existParent(const string child, const string parent)
{
    if (!elements.count(child)) {
//        throw string ("'" + child + "' element don't exist!"); // TODO: return false?
        return false;
    }

    return elements[child].getParents().count (parent);
}


/*
 */
set<string> Langstruct::getRequiredAttributes(const string element)
{
    set<string> requiredAtts;

    if (!elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
        return requiredAtts;
    }
    
    map<string, bool> atts = elements[element].getAtts ();



    // Search required atts
    for (map<string, bool>::iterator it = atts.begin (); it != atts.end (); ++it)
        if (it -> second == true)
                requiredAtts.insert (it -> first);

    return requiredAtts;
}


/*
 */
vector<string> Langstruct::getChildrenNames(const string element)
{
    vector<string> childrenNames;
    if (!elements.count(element)){
//        throw string ("'" + element + "' element don't exist!"); //TODO: return empty vector?
        return childrenNames;
    }
    
    for (map<string, ElementStructure>::iterator it = elements.begin (); it != elements.end (); ++it) 
        if ((it -> second).getParents().count(element)) 
            childrenNames.push_back (it -> first);        
    
    return childrenNames;
}


/*
 */
bool Langstruct::defineScope(const string element)
{
    if (!elements.count(element))
        return false;//throw string ("'" + element + "' element don't exist!");

    return elements[element].isScopeDefined();
}


/*
 */
map<string, bool> Langstruct::getAttributes(const string element)
{
    if (!elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
        return *(new map<string,bool> ());
    }
    
    return elements[element].getAtts();
}


/*
 */
char Langstruct::getCardinality(const string &element, const string &parent)
{
    if (!elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
        return '0';
    }

    //TODO: colocar retorno como '0' pra esse caso?
    if (!elements[element].getParents().count (parent)) {
//        throw string ("'" + element + "' is not child of '" + parent + "'!");
        return '0';
    }
    
    return elements[element].getParents().at (parent);
}


/*
 * TODO: Implementar regex match. Utilizar api da google.
 */
bool Langstruct::isValidAttribute(const string attName, const string attValue, 
        const string element)
{
    if (!elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
        return false;
    }
    
    if (!elements[element].getAtts().count (attName)) {
//        throw string ("'" + attName + "' is not an attribute of '" + element + "'!");
        return false;
    }

    return RE2::FullMatch (attValue, datatypes[attributes[attName].getDatatype(element)]);
}


/*
 */
bool Langstruct::isElementReferenceDependent(const string element)
{
    bool dependent = false;
    
    for (vector<ReferenceStructure>::iterator it = references.begin(); it != references.end (); ++it) {
        if ((*it).getFrom() == element) {
            dependent = true;        
            break;
        }
    }
    
    return dependent;
}


/*
 */
bool Langstruct::isElementReferenced(const string element)
{
    bool referenced = false;
    
    for (vector<ReferenceStructure>::iterator it = references.begin(); it != references.end (); ++it) 
        if ((*it).getTo() == element) {
            referenced = true;        
            break;
        }    
    
    return referenced;
}


/*
 */
bool Langstruct::isAttributeReferenceDependent(const string &element, const string &att)
{    
    if (!elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
        return false;
    }

    if (!elements[element].getAtts().count (att)) {
//        throw string ("'" + att + "' is not an attribute of '" + element + "'!");
        return false;
    }
    
    
    bool dependent = false;
    
    for (vector<ReferenceStructure>::iterator it = references.begin(); it != references.end (); ++it) 
        if ((*it).getFrom() == element) 
            if ((*it).getFromAtt() == att){
                dependent = true;        
                break;
            }
    
    return dependent;
}

}
