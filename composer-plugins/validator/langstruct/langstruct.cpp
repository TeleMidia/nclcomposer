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
#include <boost/regex.hpp>
#include <QDebug>
#include <QFile>

#include <assert.h>

namespace nclValidator {

// Defining static variables
map<string, ElementStructure> Langstruct::elements;
map<string, AttributeStructure> Langstruct::attributes;
map<string, ReferenceStructure> Langstruct::references;
map<string, string> Langstruct::datatypes;

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
    
    istringstream strTok;
    
    string line, 
           component;

    while (!ifs.atEnd()) {
        line = QString (ifs.readLine()).toStdString();

//        std::cout << line;

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
            if (!Langstruct::elements.count (name)) {
                ElementStructure el (name, scope == "true");
                Langstruct::elements[name] = el;
            }

            Langstruct::elements[name].addParent(parent, cardinality[0]);
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
            assert (Langstruct::elements.count (element));
            if (!Langstruct::attributes.count (name)) {
                AttributeStructure att (name);
                Langstruct::attributes[name] = att;
            }

            Langstruct::elements[element].addAtt(name, required == "true");
            Langstruct::attributes[name].addElement(element, required == "true");
            Langstruct::attributes[name].addRegex(datatype, element);
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

            assert (Langstruct::elements.count (from));
            assert (Langstruct::attributes.count (fromAtt));
            assert (Langstruct::elements.count (to));
            assert (Langstruct::attributes.count (toAtt));

            ReferenceStructure ref = ReferenceStructure (from, fromAtt,
                                                         to, toAtt, perspective, perspectiveAtt);

            Langstruct::references.insert (pair<string, ReferenceStructure> (from + fromAtt + to, ref));
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

            Langstruct::datatypes[datatype] = regex;
        }
    }
    
    ifs.close();
    
    showMaps();
}


/*
 */
bool Langstruct::existElement(const string el)
{
    return Langstruct::elements.count (el);
}


/*
 */
bool Langstruct::existAttribute(const string element, const string att)
{
 if (!Langstruct::elements.count(element)){
//        throw string ("'" + element + "' element don't exist!");
        return false;
    }
    
    return Langstruct::elements[element].getAtts ().count (att) != 0;
}


/*
 */
bool Langstruct::existParent(const string child, const string parent)
{
    if (!Langstruct::elements.count(child)) {
//        throw string ("'" + child + "' element don't exist!"); // TODO: return false?
        return false;
    }

    return Langstruct::elements[child].getParents().count (parent);
}


/*
 */
set<string> Langstruct::getRequiredAttributes(const string element)
{
    set<string> requiredAtts;

    if (!Langstruct::elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
        return requiredAtts;
    }
    
    map<string, bool> atts = Langstruct::elements[element].getAtts ();



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
    if (!Langstruct::elements.count(element)){
//        throw string ("'" + element + "' element don't exist!"); //TODO: return empty vector?
        return childrenNames;
    }
    
    for (map<string, ElementStructure>::iterator it = Langstruct::elements.begin (); it != Langstruct::elements.end (); ++it)
        if ((it -> second).getParents().count(element)) 
            childrenNames.push_back (it -> first);        
    
    return childrenNames;
}


/*
 */
bool Langstruct::defineScope(const string element)
{
    if (!Langstruct::elements.count(element))
        return false;//throw string ("'" + element + "' element don't exist!");

    return Langstruct::elements[element].isScopeDefined();
}


/*
 */
map<string, bool> Langstruct::getAttributes(const string element)
{
    if (!Langstruct::elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
        return *(new map<string,bool> ());
    }
    
    return Langstruct::elements[element].getAtts();
}


/*
 */
char Langstruct::getCardinality(const string &element, const string &parent)
{
    if (!Langstruct::elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
//        qDebug() << "primeiro";
        return '0';
    }

    //TODO: colocar retorno como '0' pra esse caso?
    if (!Langstruct::elements[element].getParents().count (parent)) {
//        throw string ("'" + element + "' is not child of '" + parent + "'!");
//        qDebug() << "segundo";
        return '0';
    }
    
    return  Langstruct::elements[element].getParents().at (parent);
}


const ReferenceStructure *Langstruct::getReference (const string &elFrom, const string &elFromAtt, const string &elTo)
{
   if (!Langstruct::references.count (elFrom + elFromAtt + elTo))
        return NULL;

    return &Langstruct::references.at (elFrom + elFromAtt + elTo);
}


/*
 * TODO: Implementar regex match. Utilizar api da google.
 */
bool Langstruct::isValidAttribute(const string attName, const string attValue, 
        const string element)
{
     if (!Langstruct::elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
        return false;
    }
    
    if (!Langstruct::elements[element].getAtts().count (attName)) {
//        throw string ("'" + attName + "' is not an attribute of '" + element + "'!");
        return false;

    }

    string pattern = Langstruct::datatypes[Langstruct::attributes[attName].getDatatype(element)];
    const boost::regex reg (pattern);

//    fprintf (stderr, "\n\n%s\n%s\n%d\n",attValue.c_str(), reg.expression(), boost::regex_match (attValue, reg));

    return boost::regex_match (attValue, reg);
//    return RE2::FullMatch (attValue, Langstruct::datatypes[Langstruct::attributes[attName].getDatatype(element)]);
}


/*
 */
bool Langstruct::isElementReferenceDependent(const string element)
{
    bool dependent = false;

    map<string, ReferenceStructure>::iterator it = Langstruct::references.begin();
    for (; it != Langstruct::references.end (); ++it) {
        if ((it -> second).getFrom() == element) {
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

    map<string, ReferenceStructure>::iterator it = Langstruct::references.begin();
    for (; it != Langstruct::references.end (); ++it) {
        if ((it -> second).getTo() == element) {
            referenced = true;
            break;
        }
    }

    return referenced;
}


/*
 */
bool Langstruct::isAttributeReferenceDependent(const string &element, const string &att)
{    
    if (!Langstruct::elements.count(element)) {
//        throw string ("'" + element + "' element don't exist!");
        return false;
    }

    if (!Langstruct::elements[element].getAtts().count (att)) {
//        throw string ("'" + att + "' is not an attribute of '" + element + "'!");
        return false;
    }
    
    
    bool dependent = false;

    map<string, ReferenceStructure>::iterator it = Langstruct::references.begin();
    for (; it != Langstruct::references.end (); ++it) {
        if ((it -> second).getFrom() == element)
            if ((it -> second).getFromAtt() == att){
                dependent = true;
                break;
            }
    }

    return dependent;
}

}
