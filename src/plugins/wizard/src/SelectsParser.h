#ifndef SELECTSPARSER_H
#define SELECTSPARSER_H

#include <QString>


class SelectsElement
{
public:
  inline explicit SelectsElement () { clear(); }

  inline void clear ()
  {
    _tagname = "";
    _attributeName = "";
    _attributeValue = "";
    _isNull = true;
  }

  inline bool isNull () const { return _isNull; }

  inline QString getTagname () const { return _tagname; }
  inline QString getAttributeName () const { return _attributeName; }
  inline QString getAttributeValue () const { return _attributeValue; }

  inline void setTagname (const QString &tagname)
  { _tagname = tagname; _isNull = false; }

  inline void setAttributeName (const QString &attributeName)
  {
    _attributeName = attributeName;
    _isNull = false;
  }

  inline void setAttributeValue (const QString &attributeValue)
  {
    _attributeValue = attributeValue;
    _isNull = false;
  }

private:
  QString _tagname;
  QString _attributeName;
  QString _attributeValue;
  bool _isNull;

};

class SelectsParser
{
public:
  explicit SelectsParser() {}

  explicit SelectsParser(QString);

  void setSeletsAttributeValue (QString);

  inline bool hasParentDependency () const
  {
    return !(_parentElement.isNull());
  }

  inline QString getElementTagname () const
  { return _element.getTagname(); }

  inline QString getElementAttributeName () const
  { return _element.getAttributeName(); }

  inline QString getElementAttributeValue () const
  { return _element.getAttributeValue(); }

  inline QString getParentTagname () const
  { return _parentElement.getTagname(); }

  inline QString getParentAttributeName () const
  { return _parentElement.getAttributeName(); }

  inline QString getParentAttributeValue () const
  { return _parentElement.getAttributeValue(); }

private:
  SelectsElement _parentElement;
  SelectsElement _element;

  void parse (QString selectsAttr, SelectsElement &element);
};


#endif // SELECTSPARSER_H
