#ifndef NCLSTRUCTURE_H
#define NCLSTRUCTURE_H

#include "NCLLanguageProfile_global.h"
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>
#include <map>
using namespace std;

#include <util/AttributeReferences.h>
using namespace composer::util;

#define NCLSTRUCTURE_FILE ":/config/NCL_STRUCTURE"

namespace composer {
namespace language{
/**
 * @brief
 *
*/
class NCLLANGUAGEPROFILESHARED_EXPORT NCLStructure
{
    ;

private:
    static NCLStructure *instance; /**< TODO */
    map <QString, map <QString, bool> *> *attributes; /**< TODO */
    map <QString, map <QString, char> *> *nesting; /**< TODO */
    map <QString, QString > *dataType; /**< TODO */
    QMultiMap <QString, AttributeReferences* > *references; /**< TODO */

    //Default Constructor
    /**
     * @brief
     *
    */
    NCLStructure();

    /**
     * @brief
     *
     * @param line
     * @return vector<QString>
    */
    vector <QString> parseLine(QString line);

public:
    /**
     * @brief
     *
     * @return NCLStructure *
    */
    static NCLStructure *getInstance();
    /**
     * @brief
     *
    */
    void loadStructure();
    /**
     * @brief
     *
    */
    void clear();

    //primitives
    /**
     * @brief
     *
     * @param name
     * @param father
     * @param cardinality
    */
    void addElement (QString name, QString father, char cardinality);

    /**
     * @brief
     *
     * @param element
     * @param attr
     * @param type
     * @param required
    */
    void addAttribute (QString element,
                       QString attr,
                       QString type,
                       bool required);

    /**
     * @brief
     *
     * @param element
     * @param attr
     * @param ref_element
     * @param ref_attr
    */
    void addReference (QString element,
                       QString attr,
                       QString ref_element,
                       QString ref_attr);

    /**
     * @brief
     *
     * @return map<QString, map<QString, char> *>
    */
    map <QString, map <QString, char> *> *getNesting();

    /**
     * @brief
     *
     * @param element
     * @return map<QString, bool>
    */
    map <QString, bool> *getAttributes (QString element);

    /**
     * @brief
     *
     * @param tagname
     * @return map<QString, char>
    */
    map <QString, char> *getChildren (QString tagname);

    vector <AttributeReferences *>
            /**
             * @brief
             *
             * @param element
             * @param attr
             * @return vector<AttributeReferences *>
            */
            getReferences (QString element, QString attr);
};

}} //end namespace
#endif // NCLSTRUCTURE_H
