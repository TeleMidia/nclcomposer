/*
 * AttributeStructure.h
 *
 *  Created on: 09/08/2011
 *      Author: Mária Gonçalves
 */

#ifndef ATTRIBUTESTRUCTURE_H_
#define ATTRIBUTESTRUCTURE_H_

#include <string>
#include <map>

namespace nclValidator {

using namespace std;

class AttributeStructure {
    private:
        string name;
        map<string, /*this required*/bool> elements;
        map</*element*/string, string> datatypes;

    public:
        AttributeStructure ();            
        AttributeStructure(const string &);
        virtual ~AttributeStructure();

        void addElement (const string &, bool);
        void addRegex (const string &, const string &);

        string getName() const;
        map<string,bool> getElements() const; //TODO: return const reference?
        string getDatatype(const string &element) const;
};

}

#endif /* ATTRIBUTESTRUCTURE_H_ */
