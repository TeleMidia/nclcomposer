/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "NCLStructure.h"

namespace composer {
namespace language{

INIT_SINGLETON(NCLStructure)

NCLStructure::NCLStructure()
{
  attributes = new map <QString, map <QString, bool > * > ();
  attributes_ordered = new map<QString, deque <QString> * > ();
  nesting = new map <QString, map <QString, char > * > ();
  dataTypes = new map <QString, QString> ();
  dataTypeDefaultSuggestions = new map <QString, QStringList>();
  attributesDatatype = new map <QString, map<QString, QString> *> ();
  references = new QMultiMap <QString, AttributeReferences*> ();

  loadStructure();
}

NCLStructure::~NCLStructure()
{
  //TODO: Destructor
  map <QString, map <QString, bool> *>::iterator it; /**< TODO */
  for(it = attributes->begin(); it != attributes->end(); ++it)
  {
    map <QString, bool> *content = it->second;
    content->clear();
    delete content;
  }
  attributes->clear();
  delete attributes;

  map <QString, map <QString, char> *>::iterator it2; /**< TODO */
  for(it2 = nesting->begin(); it2 != nesting->end(); ++it2)
  {
    map <QString, char> *content = it2->second;
    content->clear();
    delete content;
  }
  nesting->clear();
  delete nesting;

  dataTypes->clear();
  delete dataTypes;

  dataTypeDefaultSuggestions->clear();
  delete dataTypeDefaultSuggestions;

  map <QString, map<QString, QString> *>::iterator it3;
  for(it3 = attributesDatatype->begin(); it3 != attributesDatatype->end();
      ++it3)
  {
    map<QString, QString> *content = it3->second;
    content->clear();
    delete content;
  }
  attributesDatatype->clear();
  delete attributesDatatype;

  //TODO: DELETE EACH INTERNAL POINTER
  references->clear();
}

// TODO: This function should be based on lex and yacc to a better
// implementation.
void NCLStructure::loadStructure(){
  QFile fInput ( NCLSTRUCTURE_FILE );
  if(!fInput.open(QIODevice::ReadOnly)){
    qErrnoWarning("Should not open the NCL_STRUCTURE file.");
    return;
  }

  QTextStream in (&fInput);
  while(!in.atEnd()){
    QString line = in.readLine();
    vector <QString> tokens = parseLine(line);

    //this is a commentary
    if(tokens.size() == 0)
      continue;

    if(tokens[0].toLower() == "datatype")
    {
      bool error = false;
      if(tokens.size() >= 3)
      {
        qDebug() << "I'm reading a new DATATYPE element " << tokens[0]
                 << tokens[1] << " " << tokens[2];
        addDatatype(tokens[1], tokens[2]);
        if (tokens.size() >= 4)
        {
          addDatatypeDefaultSuggestions(tokens[1], tokens[3]);
        }
      }
      else error = true;
      if (error)
      {
        qErrnoWarning("element primitive must have exactly 2 or 3 \
                      arguments (NAME, REGEX, DEFAULT_SUGGESTIONS)");
      }
    }
    else if(tokens[0].toLower() == "element")
    {
      // qDebug() << "I'm reading a new ELEMENT element";
      // qDebug() << "Adding Element -- " << tokens[1];

      if(tokens.size() == 5)
      {
        bool define_scope = false;
        if(tokens[4].toLower() == "true")
          define_scope = true;
        addElement(tokens[1], tokens[2], 0, define_scope);
      }
      else
      {
        qErrnoWarning("element primitive must have exactly 3 arguments \
                      (ELEMENT NAME, ELEMENT FATHER, CARDINALITY)");
      }

    }
    else if(tokens[0].toLower() == "attribute")
    {
      // qDebug() << "I'm reading a new ATTRIBUTE element" << endl;

      if(tokens.size() == 5)
      {
        bool required = false;
        if(tokens[3].toLower() == "true")
          required = true;

        addAttribute(tokens[1], tokens[2], tokens[4], required);
      }
      else
      {
        qErrnoWarning("attribute primitive must be exactly 4 arguments \
                      (ELEMENT NAME, ATTR NAME, ATTR TYPE, REQUIRED)");
      }

    }
    else if(tokens[0].toLower() == "scope")
    {
      qDebug() << "I'm reading a new SCOPE element - This is not supported yet"
               << endl;
    }
    else if(tokens[0].toLower() == "reference")
    {
      // qDebug() << "I'm reading a new REFERENCE element" << endl;
      addReference(tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);

    }
    //qDebug() << line << endl;
  }
  fInput.close();
}

// TODO: This function should be based on lex and yacc to a better
// implementation.
vector <QString> NCLStructure::parseLine(QString line)
{
  vector <QString> ret;
  QChar ch;
  int size = line.size(), i = 0;
  QString token;
  bool reading_attributes = false, readingstring = false;

  while (i < line.size()) {
    ch = line.at(i);
    if(!readingstring){
      if (ch == '/') {
        if (i+1 < size && line[i+1] == '/') {
          // comment was found, it will ignore the remaining
          // caracteres in the line
          token = "//";
          break;
        }
      }
      else if (ch == '\"')
      {
        readingstring = true;
      }
      else if(ch == '(') {
        if(token != "")
          ret.push_back(token);
        token = "";
        reading_attributes = true;
      }
      else if(ch == ',') {
        if(reading_attributes && token != "")
          ret.push_back(token);
        token = "";
      }
      else if (ch == ')') {
        if(reading_attributes && token != "")
          ret.push_back(token);
        reading_attributes = false;
        token = "";
      }
      else {
        if(!ch.isSpace())
          token.append(line.at(i));
      }
    }
    else {
      if(ch == '\"')
        readingstring = false;
      else
        token.append(line.at(i));
    }
    i++;
  }
  return ret;
}

//TODO: SCOPE
void NCLStructure::addElement(QString name, QString father, char cardinality,
                              bool define_scope)
{
  if(!nesting->count(father))
    (*nesting)[father] = new map <QString, char>();

  if(!attributes->count(name))
    (*attributes)[name] = new map <QString, bool>();

  if(!attributesDatatype->count(name))
    (*attributesDatatype)[name] = new map <QString, QString>();

  if(!attributes_ordered->count(name))
    (*attributes_ordered)[name] = new deque <QString>();

  (*(*nesting)[father])[name] = cardinality;
  this->define_scope[name] = define_scope;
}

void NCLStructure::addAttribute ( QString element, QString attr, QString type,
                                  bool required)
{
  if(!attributes->count(element))
    (*attributes)[element] = new map <QString, bool>();

  if(!attributesDatatype->count(element))
    (*attributesDatatype)[element] = new map <QString, QString>();

  if(!attributes_ordered->count(element))
    (*attributes_ordered)[element] = new deque <QString>();

//qDebug() << "NCLStructure::addAttribute (" << element << ", " << attr << ")";
  (*(*attributes)[element])[attr] = required;
  (*(*attributes_ordered)[element]).push_back(attr);
  (*(*attributesDatatype)[element])[attr] = type;
}

void NCLStructure::addReference( QString element, QString attr,
                                 QString ref_element, QString ref_attr,
                                 QString scope)
{
  AttributeReferences *ref = new AttributeReferences (element, attr,
                                                      ref_element, ref_attr,
                                                      scope);
  references->insert(element, ref);
}

void NCLStructure::addDatatype (QString name, QString regex)
{
  qDebug() << "NCLStructure::addDatatype (" << name << ", " << regex << ")";
  (*dataTypes)[name] = regex;
}

void NCLStructure::addDatatypeDefaultSuggestions( QString datatype,
                                                  QString values)
{
  // qDebug() << "NCLStructure::addDatatypeDefaultSuggestion (" << datatype
  //   << ", " << values << ")";
  if(values != "")
    (*dataTypeDefaultSuggestions)[datatype] = values.split(',');
  else
    (*dataTypeDefaultSuggestions)[datatype] = QStringList();
}

QString NCLStructure::getAttributeDatatype(QString element, QString name)
{
  if( attributesDatatype->count(element) &&
      (*attributesDatatype)[element]->count(name))
  {
    return (*(*attributesDatatype)[element])[name];
  }
  return QString("Unknown");
}

QStringList NCLStructure::getDatatypeDefaultSuggestions(QString datatype)
{
  if( dataTypeDefaultSuggestions->count(datatype))
  {
    return (*dataTypeDefaultSuggestions)[datatype];
  }
  return QStringList();
}

map <QString, bool> *NCLStructure::getAttributes(QString element)
{
  if(attributes->count(element))
    return (*attributes)[element];
  return NULL;
}

deque <QString> *NCLStructure::getAttributesOrdered(QString element)
{
  if(attributes_ordered->count(element))
    return (*attributes_ordered)[element];
  return NULL;
}

map <QString, map <QString, char> *> *NCLStructure::getNesting()
{
  return this->nesting;
}

map <QString, char> * NCLStructure::getChildren (QString tagname)
{
  if(nesting->count(tagname))
    return (*nesting)[tagname];
  return NULL;
}

vector <AttributeReferences*> NCLStructure::getReferences ( QString element,
                                                            QString attr)
{
  vector <AttributeReferences *> ref;
  foreach( AttributeReferences *value, references->values(element) )
  {
    if(value->getAttribute() == attr)
      ref.push_back(value);
  }
  return ref;
}

bool NCLStructure::defineScope(QString tagname)
{
  if(define_scope.count(tagname))
    return define_scope[tagname];
  return false;
}

} } //end namespace
