/*
 * definitions.h
 *
 *  Created on: Jul 28, 2011
 *      Author: Rodrigo Costa
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <iostream>

namespace nclValidator {

using namespace std;


/**
 * Utilizado internamente pelo modelo e pelo validador
 */
typedef string virtualId;

/**
 * Definição de um Attribute
 */
class Attribute{
public:
	inline explicit Attribute () {};
	inline explicit Attribute (string name, string value) {
														_name = name;
														_value = value;
													}

	inline string name () const { return _name; }
	inline string value () const { return _value; }

	inline void setName (string name) { _name = name; }
	inline void setValue (string value) { _value = value; }

private:
	string _name;
	string _value;
};

}
#endif /* DEFINITIONS_H_ */
