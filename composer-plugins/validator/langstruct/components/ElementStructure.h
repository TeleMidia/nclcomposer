/*
 * ElementStructure.h
 *
 *  Created on: 08/08/2011
 *      Author: Mária Gonçalves
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <string>
#include <vector>
#include <map>

namespace nclValidator {

using namespace std;

class ElementStructure {
    private:
        string name;
        map<string, /*required*/bool> atts;
        map</*parent.name*/string, /*this cardinality*/char> parents;
        bool defineScope;

    public:
        ElementStructure();
        ElementStructure(string, bool);

        void addAtt(string _name, bool _required);
        void addParent(string _parent, char _cardinality);

        string getName() const;
        map<string, char> getParents () const;
        char getCardinality(string _parent); //TODO: PQ CONST N FUNFA?
        map<string, bool> getAtts() const;

        bool isScopeDefined () const;

        virtual ~ElementStructure();
};

}

#endif /* ELEMENT_H_ */
