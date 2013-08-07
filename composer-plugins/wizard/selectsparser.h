#ifndef SELECTSPARSER_H
#define SELECTSPARSER_H

#include <QString>


class SelectsElement {

private:
    QString _tagname;
    QString _attributeName;
    QString _attributeValue;
    bool _isNull;

public:
    inline explicit SelectsElement () { clear(); }

    inline void clear () {
        _tagname = "";
        _attributeName = "";
        _attributeValue = "";
        _isNull = true;
    }

    inline bool isNull () const { return _isNull; }

    inline QString tagname () const { return _tagname; }
    inline QString attributeName () const { return _attributeName; }
    inline QString attributeValue () const { return _attributeValue; }

    inline void setTagname (QString tagname) { _tagname = tagname; _isNull = false; }
    inline void setAttributeName (QString attributeName) { _attributeName = attributeName; _isNull = false; }
    inline void setAttributeValue (QString attributeValue) { _attributeValue = attributeValue; _isNull = false; }
};

class SelectsParser
{
private:
    SelectsElement _parentElement;
    SelectsElement _element;

    void parse (QString selectsAttr, SelectsElement &element);

public:
    explicit SelectsParser() {}

    explicit SelectsParser(QString);

    void setSeletsAttributeValue (QString);

    inline bool hasParentDependency () const { return ! _parentElement.isNull(); }

    inline QString elementTagname () const { return _element.tagname(); }
    inline QString elementAttributeName () const { return _element.attributeName(); }
    inline QString elementAttributeValue () const { return _element.attributeValue(); }

    inline QString parentTagname () const { return _parentElement.tagname(); }
    inline QString parentAttributeName () const { return _parentElement.attributeName(); }
    inline QString parentAttributeValue () const { return _parentElement.attributeValue(); }
};


#endif // SELECTSPARSER_H
