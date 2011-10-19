/*
 * langstruct.h
 *
 *  Created on: Jul 28, 2011
 *  Author: José Rios
 */

#ifndef LANGSTRUCT_H_
#define LANGSTRUCT_H_

#include <string>
#include <vector>
#include <map>
#include <set>


namespace nclValidator {

using namespace std;

class Langstruct {
	public:
		static void init ();
		static bool existElement (const string);
		static bool existAttribute (const string, const string);
		static bool existParent (const string, const string);
		static bool isValidAttribute (const string, const string, const string);
		static bool isElementReferenceDependent (const string);
		static bool isElementReferenced (const string);
		static bool isAttributeReferenceDependent (const string &element, const string &att);
		static bool defineScope (const string);

		/* Returns the cardinality of an element in relation to the given parent. */
		static char getCardinality (const string &element, const string &parent);

		/* Returns the name of the children of a given element. */
		static vector<string> getChildrenNames (const string);
		static map<string, bool> getAttributes (const string);
		static set<string> getRequiredAttributes (const string);
};

}
#endif /* LANGSTRUCT_H_ */
