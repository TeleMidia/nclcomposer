/*
 * Validator.cpp
 *
 *  Created on: 29 Aug 2011
 *      Author: rios
 */

#include "Validator.h"
#include <set>
#include <assert.h>

namespace nclValidator {

set<string> ids;


/**********************************************************************************
 ** This function will validate element atts. It will make the follow tests:  	 **
 **																				 **
 ** -All atts exists?															 **
 ** -All att are syntactically valid?											 **
 ** -Element has declared a repeated att?										 **
 ** -'id' (if so) is unique?													 **
 ** -Element has declared all required atts?									 **
 ** -Media (if so) has or ('src' or 'type') or refer?							 **
 **********************************************************************************/
void testAttributes (const ModelElement &el, Model &model, vector<string> &msgs, Message &messageFactory) {
	map<string, bool> atts = Langstruct::getAttributes(el.elementName());

	vector<Attribute> declaredAtts = el.attributes();

	set<string> declaredAttsSet; /* Save all elements declared. Used for test
									if all required atts were declared.*/

	vector<Attribute>::iterator attsIt = declaredAtts.begin ();
	for (Attribute att; attsIt != declaredAtts.end (); declaredAttsSet.insert (att.name()), ++attsIt) {
		att = *attsIt;

		// Att exist?
		if (atts.count (att.name()) == 0) {
			fprintf (stderr, "'%s' doesn't have an attribute '%s'\n",
					el.elementName().c_str (), att.name().c_str ());
                        msgs.push_back(messageFactory.createMessage(2002, 2, att.name().c_str(), el.elementName().c_str()));

			/* If 'att' don't exist, then it can't be validated
			 * and can't be an 'id' attribute.*/
			continue;
		}

                // Is syntacticly valid?
                if (!Langstruct::isValidAttribute(att.name(), att.value(), el.elementName())){
			fprintf (stderr, "'%s' attribute of '%s' element can't have '%s' as value\n",
					att.name().c_str (), el.elementName().c_str (), att.value().c_str ());
                        msgs.push_back(messageFactory.createMessage(2004, 2, att.name().c_str(), el.elementName().c_str()));
                }

		// Is 'id' duplicated?
		if (att.name() == "id") {
                        if (ids.count (att.value()) != 0){
				fprintf (stderr, "'%s' id of element '%s' is already used\n",
						att.value().c_str (), el.elementName().c_str ());
                                msgs.push_back(messageFactory.createMessage(3001, 1, att.value().c_str()));
                        }
			else
				ids.insert (att.value());
		}

		// Repeated atts?
                if (declaredAttsSet.count (att.name ()) != 0){
			fprintf (stderr, "'%s' attribute is already declared in element '%s'\n",
					att.value().c_str (), el.elementName().c_str ());
                        msgs.push_back(messageFactory.createMessage(2013, 2, att.name().c_str(), el.elementName().c_str()));
                }
	}

	// REQUIRED ATTS TEST
	map<string, bool>::iterator it = atts.begin ();
	for (; it != atts.end (); ++it) {
		string att = (*it).first;

		// Att is required AND was not declared?
                if ((*it).second && declaredAttsSet.count(att) == 0){
			fprintf (stderr, "'%s' attribute MUST be declared in '%s' element\n",
					att.c_str (), el.elementName().c_str ());

                        msgs.push_back(messageFactory.createMessage(2003, 2, att.c_str(), el.elementName().c_str()));
                }
	}

	//TODO: casos especiais de media e transition
}


/* Test the children cardinality of the given element.
 * Errors will be enqueued to msg error vector. */
void testCardinality (const ModelElement &el, map<string, int> &cardinalityMap,
                      vector <string> &msgs, Message &messageFactory) {

	vector<string> childrenNames = Langstruct::getChildrenNames(el.elementName());

	// Flags to know if 'el' has some child with these cardinalities.
	bool hasHashtagChild = false,
		 hasAChild = false,
		 hasBChild = false;

	// Counter to know the number of 'el' children that have these cardinalities.
	int countHashtagOperator = 0,
		countAOperator = 0,
		countBOperator = 0;

	// The names of 'el' children that have these cardinalities.
	string hashTagChildren = "",
		   aChildren = "",
		   bChildren = "";

	vector<string>::iterator childNameIt = childrenNames.begin ();
	for (char cardinality = ' '; childNameIt != childrenNames.end (); ++childNameIt) {
		cardinality = Langstruct::getCardinality(*childNameIt, el.elementName());

		switch (cardinality) {

			// Child CAN appear any times.
			case '*': break;

			// Child MUST be declared only once
                        case '1':
                            if (cardinalityMap[*childNameIt] != 1){
                                fprintf (stderr, "'%s', child of '%s', MUST be declared once\n",
                                        (*childNameIt).c_str (), el.elementName().c_str ());
                                msgs.push_back(messageFactory.createMessage(2008, 2, el.elementName().c_str(), childNameIt->c_str()));
                            }
                            break;

		    // Child CAN be declared only once
                        case '?':
                            if (cardinalityMap[*childNameIt] > 1){
                                fprintf (stderr, "'%s', child of '%s', CAN be declared once\n",
                                        (*childNameIt).c_str (), el.elementName().c_str ());
                                msgs.push_back(messageFactory.createMessage(2006, 2, el.elementName().c_str(), childNameIt->c_str()));
                            }
                            break;

		    // Child MUST be declared, AT LEAST, one time
                        case '+':
                            if (cardinalityMap[*childNameIt] < 1){
                                fprintf (stderr, "'%s', child of '%s', MUST be declared at least one time\n",
                                        (*childNameIt).c_str (), el.elementName().c_str ());
                                msgs.push_back(messageFactory.createMessage(2007, 2, el.elementName().c_str(), childNameIt->c_str()));
                            }
                            break;

			// AT LEAST one child with this operator SHOULD be declared
			case '#': hasHashtagChild = true;
					  countHashtagOperator += cardinalityMap[*childNameIt];
					  hashTagChildren.append ("'" + (*childNameIt) + "' ");
					  break;

			// AT LEAST one child with this operator SHOULD be declared
			case 'a': hasAChild = true;
					  countAOperator += cardinalityMap[*childNameIt];
					  aChildren.append ("'" + (*childNameIt) + "' ");
					  break;

			// AT LEAST one child with this operator SHOULD be declared
			case 'b': hasBChild = true;
					  countBOperator += cardinalityMap[*childNameIt];
					  bChildren.append ("'" + (*childNameIt) + "' ");
					  break;

			// Unknown element. TODO: I need this?
			default:  fprintf (stderr, "Cardinality of '%s', child of '%s', is unknown\n",
							(*childNameIt).c_str (), el.elementName().c_str ());
					  break;
		}
	}

	//TODO: Acrescentar condição count*Operator > 1 nessas condições.
	// Test hashtag cardinality children
        if (hasHashtagChild && countHashtagOperator == 0){
		  fprintf (stderr, "'%s' MUST have one of these child: %s\n",
				  el.elementName().c_str (), hashTagChildren.c_str ());

                  msgs.push_back(messageFactory.createMessage(2007, 2, el.elementName().c_str(), hashTagChildren.c_str()));
        }

	// Test a cardinality children
        if (hasAChild && countAOperator == 0){
		  fprintf (stderr, "'%s' MUST have one of these child: %s\n",
				  el.elementName().c_str (), aChildren.c_str ());
                  msgs.push_back(messageFactory.createMessage(2007, 2, el.elementName().c_str(), aChildren.c_str()));
        }

	// Test b cardinality children
        if (hasBChild && countBOperator == 0){
		  fprintf (stderr, "'%s' MUST have one of these child: %s\n",
				  el.elementName().c_str (), bChildren.c_str ());
                  msgs.push_back(messageFactory.createMessage(2007, 2, el.elementName().c_str(), bChildren.c_str()));
        }

	return;
}


/**********************************************************************************
 ** This function will performs a recursively syntactic validation on elements.  **
 ** It wil make the follow tests:  	 											 **
 **																				 **
 ** -Element exists?															 **
 ** -All att are syntactically valid?									 		 **
 ** -Is child of parent?									 		 			 **
 ** -All children cardinality are right?									 	 **
 **********************************************************************************/
void structuralValidation(const ModelElement &el, Model &model, vector<string> &msgs, Message &messageFactory)
{

	 // FUNDAMENTAL TEST. Test if element exist.
	if (!Langstruct::existElement(el.elementName())) {
		// NoSuchElementError
		// Create a Message for unknow element
		fprintf (stderr, "NoSuchElementError: '%s'\n", el.elementName().c_str ());
                msgs.push_back(messageFactory.createMessage(2001, 1, el.elementName().c_str()));
	}

	//TODO: create a validate atts function
        testAttributes (el, model, msgs, messageFactory);

	string parent = el.parent() != "" ? model.element(el.parent())->elementName(): "";
	if (!Langstruct::existParent(el.elementName(), parent)) {
		// NoSuchParentError
		// Create a Message for unknow parent
		fprintf (stderr, "'%s' is not child of '%s'\n", el.elementName().c_str (),	parent.c_str ());
                msgs.push_back(messageFactory.createMessage(2005, 2, el.elementName().c_str(), parent.c_str()));
	}

	// Validate children syntacticly and their cardinality
	vector<virtualId> children = el.children();
	map<string, int> cardinalityMap;

	vector<virtualId>::iterator childIt = children.begin ();
	for (ModelElement *child; childIt != children.end(); ++childIt) {
		child = model.element(*childIt);
		assert (child);

		++cardinalityMap[child -> elementName()];

		// validate recursively
//		syntacticValidation(*child, model, msgs);
	}

        testCardinality (el, cardinalityMap, msgs, messageFactory);

	return;
}


/* Perform a semantic validation.*/
void semanticValidation(const ModelElement &el, Model &model, vector<string> &msgs, Message &messageFactory)
{
	vector<Attribute> atts = el.attributes();

	vector<Attribute>::iterator attIt = atts.begin ();
	for (Attribute att; attIt != atts.end (); ++attIt) {
		att = *attIt;

		// Att make reference?
		if (Langstruct::isAttributeReferenceDependent(el.elementName(), att.name())) {
			vector<ModelElement *> pointedElements = model.elementByIdentifier(att.value());


			///////////////////////////////////////////////////////////////////
			// /* Um elemento que faz referência, sempre terá que fazer referẽncia,
			// * ou ele pode assumir um outro valor que não a referência?*/
			///////////////////////////////////////////////////////////////////

			// Elements points to nobody
			if (pointedElements.empty()) {
				fprintf (stderr, "'%s' attribute of '%s' element point to anybody\n",
						att.name().c_str(), el.elementName().c_str());
                                msgs.push_back(messageFactory.createMessage(2004, 2, att.name().c_str(), el.elementName().c_str()));
				continue;
			}
		}
	}


//	// TODO: Casos especiais
//	if (el.elementName() == "link" || el.elementName() == "causalConnector") {
//
//	}
}


// TODO: Criar comentário pra essa função
vector<string> Validator::validate(Model &model)
{
        vector<string> msgs;
        Message messageFactory;

	set<virtualId> markeds = model.markedElements(),
				   affecteds = model.affectedElements();

        set<virtualId>::iterator vIds = markeds.begin();
	for (ModelElement *el = NULL; vIds != markeds.end(); ++vIds) {

		el = model.element(*vIds);
                if (el)
                    std::cout << "validating element: " << el->elementName() << endl;

//		assert (el);
//		assert (el -> parent() != "" || el -> elementName() == "ncl");

                structuralValidation (*el, model, msgs, messageFactory);
                semanticValidation (*el, model, msgs, messageFactory);
	}
        vIds = affecteds.begin();
        for (ModelElement *el = NULL; vIds != affecteds.end(); ++vIds) {
            el = model.element(*vIds);
            if (el)
                std::cout << "validating element: " << el->elementName() << endl;

//		assert (el);
//		assert (el -> parent() != "" || el -> elementName() == "ncl");
            el = model.element(*vIds);
            if (el)
                std::cout << "validating element: " << el->elementName() << endl;

//		assert (el);
//		assert (el -> parent() != "" || el -> elementName() == "ncl");

            semanticValidation (*el, model, msgs, messageFactory);
        }

//        ModelElement *el = affecteds.begin();
//        while (el != affecteds.end()) {
//                semanticValidation (*el, msgs);
//                ++vIds;
//        }

	ids.clear ();

	return msgs;
}


}
