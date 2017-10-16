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

CPR_NCLPROFILE_BEGIN_NAMESPACE

INIT_SINGLETON (NCLStructure)

#include <QDebug>

NCLStructure::NCLStructure ()
{
  _attributes = new map<QString, map<QString, bool> *> ();
  _attributes_ordered = new map<QString, deque<QString> *> ();
  _nesting = new map<QString, map<QString, char> *> ();
  _elements_ordered = new deque<QString> ();
  _datatypes = new map<QString, QString> ();
  _datatypeDefaultSuggestions = new map<QString, QStringList> ();
  _attributesDatatype = new map<QString, map<QString, QString> *> ();
  _references = new QMultiMap<QString, AttributeReferences *> ();

  loadStructure ();
}

NCLStructure::~NCLStructure ()
{
  // TODO: Destructor
  map<QString, map<QString, bool> *>::iterator it; /**< TODO */
  for (it = _attributes->begin (); it != _attributes->end (); ++it)
  {
    map<QString, bool> *content = it->second;
    content->clear ();
    delete content;
  }
  _attributes->clear ();
  delete _attributes;

  map<QString, map<QString, char> *>::iterator it2; /**< TODO */
  for (it2 = _nesting->begin (); it2 != _nesting->end (); ++it2)
  {
    map<QString, char> *content = it2->second;
    content->clear ();
    delete content;
  }
  _nesting->clear ();
  delete _nesting;
  delete _elements_ordered;

  _datatypes->clear ();
  delete _datatypes;

  _datatypeDefaultSuggestions->clear ();
  delete _datatypeDefaultSuggestions;

  map<QString, map<QString, QString> *>::iterator it3;
  for (it3 = _attributesDatatype->begin (); it3 != _attributesDatatype->end ();
       ++it3)
  {
    map<QString, QString> *content = it3->second;
    content->clear ();
    delete content;
  }
  _attributesDatatype->clear ();
  delete _attributesDatatype;

  // TODO: DELETE EACH INTERNAL POINTER
  _references->clear ();
}

// TODO: This function should be based on lex and yacc to a better
// implementation.
void
NCLStructure::loadStructure ()
{
  QFile fInput (NCLSTRUCTURE_FILE);
  if (!fInput.open (QIODevice::ReadOnly))
  {
    qErrnoWarning ("Should not open the NCL_STRUCTURE file.");
    return;
  }

  QTextStream in (&fInput);
  int nlines = 0;
  while (!in.atEnd ())
  {
    QString line = in.readLine ();
    nlines++;
    QStringList tokens = parseLine (line);

    // this is a commentary
    if (tokens.size () == 0)
      continue;

    if (tokens[0].toLower () == "datatype")
    {
      bool error = false;
      if (tokens.size () >= 3)
      {
        addDatatype (tokens[1], tokens[2]);
        if (tokens.size () >= 4)
        {
          addDatatypeDefaultSuggestions (tokens[1], tokens[3]);
        }
      }
      else
        error = true;
      if (error)
      {
        qErrnoWarning ("element primitive must have exactly 2 or 3 \
                      arguments (NAME, REGEX, DEFAULT_SUGGESTIONS)");
      }
    }
    else if (tokens[0].toLower () == "element")
    {
      if (tokens.size () == 5)
      {
        bool define_scope = false;
        if (tokens[4].toLower () == "true")
          define_scope = true;
        addElement (tokens[1], tokens[2], 0, define_scope);
      }
      else
      {
        QString msg = QString (NCLSTRUCTURE_FILE) + ":"
                      + QString::number (nlines)
                      + " ELEMENT primitive must have exactly 4 arguments "
                        "(ELEMENT NAME, ELEMENT FATHER, CARDINALITY) ";

        qErrnoWarning (msg.toStdString ().c_str ());
      }
    }
    else if (tokens[0].toLower () == "attribute")
    {
      if (tokens.size () == 5)
      {
        bool required = false;
        if (tokens[3].toLower () == "true")
          required = true;

        addAttribute (tokens[1], tokens[2], tokens[4], required);
      }
      else
      {
        qErrnoWarning ("attribute primitive must have exactly 4 arguments \
                      (ELEMENT NAME, ATTR NAME, ATTR TYPE, REQUIRED)");
      }
    }
    else if (tokens[0].toLower () == "scope")
    {
      qWarning() << "I'm reading a new SCOPE element. "
                    "This is not supported yet";
    }
    else if (tokens[0].toLower () == "reference")
    {
      addReference (tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
    }
  }
  fInput.close ();
}

// TODO: This function should be based on lex and yacc to a better
// implementation.
QStringList
NCLStructure::parseLine (const QString &line)
{
  QStringList ret;
  QChar ch;
  int size = line.size (), i = 0;
  QString token;
  bool reading_attributes = false, readingstring = false;

  while (i < line.size ())
  {
    ch = line.at (i);
    if (!readingstring)
    {
      if (ch == '/')
      {
        if (i + 1 < size && line[i + 1] == '/')
        {
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
      else if (ch == '(')
      {
        if (token != "")
          ret.push_back (token);
        token = "";
        reading_attributes = true;
      }
      else if (ch == ',')
      {
        if (reading_attributes && token != "")
          ret.push_back (token);
        token = "";
      }
      else if (ch == ')')
      {
        if (reading_attributes && token != "")
          ret.push_back (token);
        reading_attributes = false;
        token = "";
      }
      else
      {
        if (!ch.isSpace ())
          token.append (line.at (i));
      }
    }
    else
    {
      if (ch == '\"')
        readingstring = false;
      else
        token.append (line.at (i));
    }
    i++;
  }

  return ret;
}

// TODO: SCOPE
void
NCLStructure::addElement (const QString &name, const QString &father,
                          char cardinality, bool define_scope)
{
  if (!_nesting->count (father))
    (*_nesting)[father] = new map<QString, char> ();

  if (!_attributes->count (name))
    (*_attributes)[name] = new map<QString, bool> ();

  if (!_attributesDatatype->count (name))
    (*_attributesDatatype)[name] = new map<QString, QString> ();

  if (!_attributes_ordered->count (name))
    (*_attributes_ordered)[name] = new deque<QString> ();

  deque<QString>::iterator location
      = find (_elements_ordered->begin (), _elements_ordered->end (), name);
  if (location == _elements_ordered->end ())
    _elements_ordered->push_back (name);

  (*(*_nesting)[father])[name] = cardinality;
  this->_define_scope[name] = define_scope;
}

void
NCLStructure::addAttribute (const QString &element, const QString &attr,
                            const QString &type, bool required)
{
  if (!_attributes->count (element))
    (*_attributes)[element] = new map<QString, bool> ();

  if (!_attributesDatatype->count (element))
    (*_attributesDatatype)[element] = new map<QString, QString> ();

  if (!_attributes_ordered->count (element))
    (*_attributes_ordered)[element] = new deque<QString> ();

  (*(*_attributes)[element])[attr] = required;
  (*(*_attributes_ordered)[element]).push_back (attr);
  (*(*_attributesDatatype)[element])[attr] = type;
}

void
NCLStructure::addReference (const QString &element, const QString &attr,
                            const QString &ref_element,
                            const QString &ref_attr, const QString &scope)
{
  AttributeReferences *ref
      = new AttributeReferences (element, attr, ref_element, ref_attr, scope);
  _references->insert (element, ref);
}

void
NCLStructure::addDatatype (const QString &name, const QString &regex)
{
  (*_datatypes)[name] = regex;
}

void
NCLStructure::addDatatypeDefaultSuggestions (const QString &datatype,
                                             const QString &values)
{
  if (values != "")
    (*_datatypeDefaultSuggestions)[datatype] = values.split (',');
  else
    (*_datatypeDefaultSuggestions)[datatype] = QStringList ();
}

QString
NCLStructure::getAttributeDatatype (const QString &element,
                                    const QString &name)
{
  if (_attributesDatatype->count (element)
      && (*_attributesDatatype)[element]->count (name))
  {
    return (*(*_attributesDatatype)[element])[name];
  }

  return QString ("Unknown");
}

QStringList
NCLStructure::getDatatypeDefaultSuggestions (const QString &datatype)
{
  if (_datatypeDefaultSuggestions->count (datatype))
  {
    return (*_datatypeDefaultSuggestions)[datatype];
  }

  return QStringList ();
}

map<QString, bool> *
NCLStructure::getAttributes (const QString &element)
{
  if (_attributes->count (element))
    return (*_attributes)[element];

  return nullptr;
}

deque<QString> *
NCLStructure::getAttributesOrdered (const QString &element)
{
  if (_attributes_ordered->count (element))
    return (*_attributes_ordered)[element];

  return nullptr;
}

map<QString, map<QString, char> *> *
NCLStructure::getNesting ()
{
  return this->_nesting;
}

map<QString, char> *
NCLStructure::getChildren (const QString &tagname)
{
  if (_nesting->count (tagname))
    return (*_nesting)[tagname];
  return nullptr;
}

deque<QString> *
NCLStructure::getElementsOrdered ()
{
  return _elements_ordered;
}

vector<AttributeReferences *>
NCLStructure::getReferences (const QString &element, const QString &attr)
{
  vector<AttributeReferences *> ref;
  foreach (AttributeReferences *value, _references->values (element))
  {
    if (value->getAttribute () == attr)
      ref.push_back (value);
  }
  return ref;
}

bool
NCLStructure::defineScope (const QString &tagname)
{
  if (_define_scope.count (tagname))
    return _define_scope[tagname];

  return false;
}

CPR_NCLPROFILE_END_NAMESPACE
