#ifndef NCLSTRUCTURE_H
#define NCLSTRUCTURE_H

#include <QtDebug>

#include <QFile>
#include <QTextStream>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>
#include <map>

#include <util/AttributeReferences.h>

using namespace std;

#define NCLSTRUCTURE_FILE ":/config/NCL_STRUCTURE"

/**
    *
*/
class NCLStructure
{
private:
    static NCLStructure *instance;
    map <QString, map <QString, bool> *> *attributes;
    map <QString, map <QString, char> *> *nesting;
    map <QString, QString > *dataType;
    QMultiMap <QString, AttributeReferences* > *references;

    //Default Constructor
    NCLStructure();

    vector <QString> parseLine(QString line);

public:
    static NCLStructure *getInstance();
    void loadStructure();
    void clear();

    //primitives
    void addElement (QString name, QString father, char cardinality);

    void addAttribute (QString element,
                       QString attr,
                       QString type,
                       bool required);

    void addReference (QString element,
                       QString attr,
                       QString ref_element,
                       QString ref_attr);

    map <QString, map <QString, char> *> *getNesting();

    map <QString, bool> *getAttributes (QString element);

    map <QString, char> *getChildren (QString tagname);

    vector <AttributeReferences *>
            getReferences (QString element, QString attr);
};

#endif // NCLSTRUCTURE_H
