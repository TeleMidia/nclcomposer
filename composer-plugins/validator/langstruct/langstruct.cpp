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

#include "langstruct.h"
#include "components/ElementStructure.h"
#include "components/AttributeStructure.h"
#include "components/ReferenceStructure.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <QDebug>
#include <QFile>

#ifdef WITH_LIBBOOST
#include <boost/regex.hpp>
#else
#include <QRegExp>
#endif

#include <assert.h>


// Defining static variables
map<string, ElementStructure> Langstruct::_elements;
map<string, AttributeStructure> Langstruct::_attributes;
map<string, ReferenceStructure> Langstruct::_references;
map<string, string> Langstruct::_datatypes;

void showMaps ()
{

}

/*
 */
void Langstruct::init ()
{
  QFile ifs (":/config/NCL_STRUCTURE");
  //    ifstream ifs (":/config/langstruct.in", ifstream::in);

  if (!ifs.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  istringstream strTok;

  string line,
      component;

  while (!ifs.atEnd())
  {
    line = QString (ifs.readLine()).toStdString();

    if(!line.size()) continue;
    if(line.substr(0, 2) == "//") continue;

    std::cout << line.c_str();

    strTok.clear ();
    strTok.str(line);

    getline(strTok, component, '(');

    if(!component.size()) continue;

    std::stringstream trimmer;
    trimmer << component.c_str();
    trimmer >> component;

    std::cout << "########### COMPONENT = " << component.c_str() << std::endl;

    //        assert (component == "ELEMENT" || component == "ATTRIBUTE"
    //                || component == "REFERENCE" || component == "DATATYPE");

    if (component == "ELEMENT")
    {
      string name,
          parent,
          cardinality,
          scope;

      getline(strTok, name, ',');
      getline(strTok, parent, ',');
      getline(strTok, cardinality, ',');
      getline(strTok, scope, ')');

      // Include an ElementStructure
      if (!Langstruct::_elements.count (name))
      {
        ElementStructure el (name, scope == "true");
        Langstruct::_elements[name] = el;
      }

      Langstruct::_elements[name].addParent(parent, cardinality[0]);
    }

    else if (component == "ATTRIBUTE")
    {
      string element,
          name,
          required,
          datatype;

      getline(strTok, element, ',');
      getline(strTok, name, ',');
      getline(strTok, required, ',');
      getline(strTok, datatype, ')');

      //Include an AttributeStructure. All Elements must be already included
      assert (Langstruct::_elements.count (element));
      if (!Langstruct::_attributes.count (name))
      {
        AttributeStructure att (name);
        Langstruct::_attributes[name] = att;
      }

      Langstruct::_elements[element].addAtt(name, required == "true");
      Langstruct::_attributes[name].addElement(element, required == "true");
      Langstruct::_attributes[name].addRegex(datatype, element);
    }

    else if (component == "REFERENCE")
    {
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
      if (persAttPos != perspective.npos)
      {
        perspectiveAtt = perspective.substr(persAttPos + 1,
                                            perspective.size() - 1);
        perspective.erase (persAttPos, perspective.size () - 1);
      }

      assert (Langstruct::_elements.count (from));
      assert (Langstruct::_attributes.count (fromAtt));
      assert (Langstruct::_elements.count (to));
      assert (Langstruct::_attributes.count (toAtt));

      ReferenceStructure ref = ReferenceStructure (from, fromAtt,
                                                   to, toAtt, perspective,
                                                   perspectiveAtt);

      Langstruct::_references.insert (pair<string, ReferenceStructure>
                                      (from + fromAtt + to, ref));
    }

    // DATATYPE
    else if(component == "DATATYPE")
    {
      string datatype, regex, values;

      getline(strTok, datatype, ',');

      getline(strTok, regex, '\"');
      getline(strTok, regex, '\"');

      getline(strTok, values, '\"');
      getline(strTok, values, '\"');

      std::cout << "########### DATATYPE = " <<
                   datatype.c_str() << std::endl;
      std::cout << "########### REGEX = " << regex.c_str() << std::endl;
      //
      //            RE2 re(regex);
      //            assert(re.ok());  // compiled; if not, see re.error();

      Langstruct::_datatypes[datatype] = regex;
    }
  }

  ifs.close();

  showMaps();
}


/*
 */
bool Langstruct::existElement(const string el)
{
  return Langstruct::_elements.count (el);
}


/*
 */
bool Langstruct::existAttribute(const string element, const string att)
{
  if (!Langstruct::_elements.count(element))
  {
    return false;
  }

  return Langstruct::_elements[element].getAtts ().count (att) != 0;
}


/*
 */
bool Langstruct::existParent(const string child, const string parent)
{
  if (!Langstruct::_elements.count(child))
  {
    return false;
  }

  return Langstruct::_elements[child].getParents().count (parent);
}


/*
 */
set<string> Langstruct::getRequiredAttributes(const string element)
{
  set<string> requiredAtts;

  if (!Langstruct::_elements.count(element))
  {
    return requiredAtts;
  }

  map<string, bool> atts = Langstruct::_elements[element].getAtts ();



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
  if (!Langstruct::_elements.count(element))
  {
    return childrenNames;
  }

  map<string, ElementStructure>::iterator it = Langstruct::_elements.begin();
  for ( ; it != Langstruct::_elements.end (); ++it)
    if ((it -> second).getParents().count(element))
      childrenNames.push_back (it -> first);

  return childrenNames;
}


/*
 */
bool Langstruct::defineScope(const string element)
{
  if (!Langstruct::_elements.count(element))
    return false;

  return Langstruct::_elements[element].isScopeDefined();
}


/*
 */
map<string, bool> Langstruct::getAttributes(const string element)
{
  if (!Langstruct::_elements.count(element))
  {
    return *(new map<string,bool> ());
  }

  return Langstruct::_elements[element].getAtts();
}


/*
 */
char Langstruct::getCardinality(const string &element, const string &parent)
{
  if (!Langstruct::_elements.count(element))
  {
    return '0';
  }

  //TODO: colocar retorno como '0' pra esse caso?
  if (!Langstruct::_elements[element].getParents().count (parent))
  {
    return '0';
  }

  return  Langstruct::_elements[element].getParents().at (parent);
}


const ReferenceStructure *Langstruct::getReference (const string &elFrom,
                                                    const string &elFromAtt,
                                                    const string &elTo)
{
  if (!Langstruct::_references.count (elFrom + elFromAtt + elTo))
    return NULL;

  return &Langstruct::_references.at (elFrom + elFromAtt + elTo);
}

const vector <ReferenceStructure> Langstruct::getReferences(
    const string &elFrom,
    const string &elFromAtt)
{
  vector <ReferenceStructure> references;

  map<string, ReferenceStructure>::iterator it = _references.begin();
  for (; it != _references.end (); ++it)
  {
    ReferenceStructure referenceStructure = (*it).second;

    if (referenceStructure.getFrom() == elFrom &&
        referenceStructure.getFromAtt() == elFromAtt)
      references.push_back(referenceStructure);
  }

  return references;
}

bool Langstruct::isValidAttribute(const string attName, const string attValue, 
                                  const string element)
{
  if (!Langstruct::_elements.count(element))
  {
    return false;
  }

  if (!Langstruct::_elements[element].getAtts().count (attName))
  {
    return false;
  }

  string pattern = Langstruct::_datatypes[Langstruct::_attributes[attName].
      getDatatype(element)];

#ifdef WITH_LIBBOOST
  const boost::regex reg (pattern);

  return boost::regex_match (attValue, reg);
#else
  QRegExp rx(QString(pattern.c_str()));
  return rx.exactMatch(QString(attValue.c_str()));
#endif
}


/*
 */
bool Langstruct::isElementReferenceDependent(const string element)
{
  bool dependent = false;

  map<string, ReferenceStructure>::iterator it =
      Langstruct::_references.begin();
  for (; it != Langstruct::_references.end (); ++it)
  {
    if ((it -> second).getFrom() == element)
    {
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

  map<string, ReferenceStructure>::iterator it =
      Langstruct::_references.begin();
  for (; it != Langstruct::_references.end (); ++it)
  {
    if ((it -> second).getTo() == element)
    {
      referenced = true;
      break;
    }
  }

  return referenced;
}


/*
 */
bool Langstruct::isAttributeReferenceDependent(const string &element,
                                               const string &att)
{    
  if (!Langstruct::_elements.count(element))
  {
    return false;
  }

  if (!Langstruct::_elements[element].getAtts().count (att))
  {
    return false;
  }


  bool dependent = false;

  map<string, ReferenceStructure>::iterator it = Langstruct::_references.begin();
  for (; it != Langstruct::_references.end (); ++it)
  {
    if ((it -> second).getFrom() == element)
      if ((it -> second).getFromAtt() == att)
      {
        dependent = true;
        break;
      }
  }

  return dependent;
}

