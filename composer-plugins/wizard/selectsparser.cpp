#include "selectsparser.h"

#include <QDebug>

SelectsParser::SelectsParser(QString selectsAttr)
{
    setSeletsAttributeValue(selectsAttr);
}

void SelectsParser::setSeletsAttributeValue(QString selectsAttr)
{
    _element.clear();
    _parentElement.clear();

    int index = selectsAttr.indexOf(">");
    if (index != -1){
        QString parentPattern = selectsAttr.left(index).trimmed();
        parse(parentPattern, _parentElement);

        selectsAttr = selectsAttr.mid(index + 1).trimmed();
    }


    parse (selectsAttr, _element);

}

void SelectsParser::parse(QString pattern, SelectsElement &element)
{
    if (pattern != ""){
        if (pattern == "*") //*
            element.setTagname("*");
        else {
            int openBracketIndex = pattern.indexOf("[");
            if (openBracketIndex != -1){ //E[attr...]
                element.setTagname(pattern.left(openBracketIndex));
                pattern = pattern.mid(openBracketIndex + 1);

                int closeBracketIndex = pattern.indexOf("]");

                int equalsIndex = pattern.indexOf("=");
                if (equalsIndex == -1){ //E[attr]
                    element.setAttributeName(pattern.left(closeBracketIndex));
                }
                else{
                    int tileIndex = pattern.indexOf("~");
                    if (tileIndex != -1){ //E[attr=value]
                        element.setAttributeName(pattern.left(tileIndex));
                        if (closeBracketIndex > equalsIndex)
                            element.setAttributeValue(pattern.mid(equalsIndex + 1, closeBracketIndex - equalsIndex - 1));
                    }
                    else{//E[attr~=value]
                        element.setAttributeName(pattern.left(equalsIndex));
                        if (closeBracketIndex > equalsIndex)
                            element.setAttributeValue(pattern.mid(equalsIndex + 1, closeBracketIndex - equalsIndex - 1));
                    }
                }
            }
            else{
                int dotIndex = pattern.indexOf(".");
                if (dotIndex != -1){ //E.attr]
                    element.setAttributeName("class");
                    element.setAttributeValue(pattern.mid(openBracketIndex));
                }
                else{
                    int colonIndex = pattern.indexOf(":");
                    if (colonIndex != -1){
                        qDebug () << QString("The value of 'selects' attribute '" + pattern + " isn't supported yet!");
                    }
                    else
                        element.setTagname(pattern);
                }
            }
        }
    }
}
