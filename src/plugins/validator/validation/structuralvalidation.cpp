/*
* Copyright 2011-2013 Laws/UFMA.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library. If not, see
* <http://www.gnu.org/licenses/>.
*/

#include "structuralvalidation.h"
#include <assert.h>


/*******************************************************************************
 ** This function will validate element atts. It will make the follow tests:	**
 ** -All atts exists?																													**
 ** -All att are syntactically valid?																					**
 ** -Element has declared a repeated att?																			**
 ** -'id' (if so) is unique?																									**
 ** -Element has declared all required atts?																	**
 ** -Media (if so) has or ('src' or 'type') or refer?													**
 ******************************************************************************/
void testAttributes (const ModelElement &el, Model &model,
                     vector<pair<void *, string> > &msgs,
                     Message &messageFactory)
{
  map<string, bool> atts = Langstruct::getAttributes(el.elementName());

  vector<Attribute> declaredAtts = el.attributes();

  set<string> declaredAttsSet;
  /* Save all elements declared.
   * Used for test if all required atts were declared.*/

  vector<Attribute>::iterator attsIt = declaredAtts.begin ();
  for (Attribute att; attsIt != declaredAtts.end ();
       declaredAttsSet.insert (att.name()), ++attsIt)
  {
    att = *attsIt;

    // Att exist?
    if (atts.count (att.name()) == 0)
    {
      fprintf (stderr, "'%s' doesn't have an attribute '%s'\n",
               el.elementName().c_str (), att.name().c_str ());

      /* If 'att' don't exist, then it can't be validated
                         * and can't be an 'id' attribute.*/

      msgs.push_back(pair<void *, string> (el.data(),
                                           messageFactory.createMessage(
                                             2002, 2, att.name().c_str(),
                                             el.elementName().c_str())));

      model.addElementWithErrorInLastPass(el.id());

      continue;
    }

    // Is syntacticly valid?
    if (!Langstruct::isValidAttribute(att.name(), att.value(),
                                      el.elementName()))
    {
      fprintf (stderr, "'%s' attribute of '%s' element can't have '%s' as "
               "value\n", att.name().c_str (), el.elementName().c_str (),
               att.value().c_str ());

      msgs.push_back(pair<void *, string> (el.data(),
                                           messageFactory.createMessage(
                                             2004, 2, att.name().c_str(),
                                             el.elementName().c_str())));
      model.addElementWithErrorInLastPass(el.id());
    }

    // Repeated atts?
    if (declaredAttsSet.count (att.name ()) != 0)
    {
      fprintf (stderr, "'%s' attribute is already declared in element '%s'\n",
               att.value().c_str (), el.elementName().c_str ());
      msgs.push_back(pair<void *, string> (el.data(),
                                           messageFactory.createMessage(
                                             2013, 2, att.name().c_str(),
                                             el.elementName().c_str())));
      model.addElementWithErrorInLastPass(el.id());
    }
  }

  // REQUIRED ATTS TEST
  map<string, bool>::iterator it = atts.begin ();
  for (; it != atts.end (); ++it)
  {
    string att = (*it).first;

    // Att is required AND was not declared?
    if ((*it).second && declaredAttsSet.count(att) == 0)
    {
      fprintf (stderr, "'%s' attribute MUST be declared in '%s' element\n",
               att.c_str (), el.elementName().c_str ());

      msgs.push_back(pair<void *, string> (el.data(),
                                           messageFactory.createMessage(
                                             2003, 2, att.c_str(),
                                             el.elementName().c_str())));
      model.addElementWithErrorInLastPass(el.id());
    }
  }

  //TODO: casos especiais de media e transition
}


/* Test the children cardinality of the given element.
 * Errors will be enqueued to msg error vector. */
void testCardinality (const ModelElement &el, Model &model,
                      map<string, int> &cardinalityMap,
                      vector<pair<void *, string> > &msgs,
                      Message &messageFactory)
{
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
  for (char cardinality = ' '; childNameIt != childrenNames.end ();
       ++childNameIt)
  {
    cardinality = Langstruct::getCardinality(*childNameIt, el.elementName());

    switch (cardinality)
    {
      // Child CAN appear any times.
      case '*': break;

        // Child MUST be declared only once
      case '1': if (cardinalityMap[*childNameIt] != 1)
        {
          fprintf (stderr, "'%s', child of '%s', MUST be declared once\n",
                   (*childNameIt).c_str (), el.elementName().c_str ());
          msgs.push_back(pair<void *, string> (el.data(),
                                               messageFactory.createMessage(
                                                 2008, 2,
                                                 el.elementName().c_str(),
                                                 childNameIt->c_str())));

          model.addElementWithErrorInLastPass(el.id());
        }
        break;

        // Child CAN be declared only once
      case '?': if (cardinalityMap[*childNameIt] > 1)
        {
          fprintf (stderr, "'%s', child of '%s', CAN be declared once\n",
                   (*childNameIt).c_str (), el.elementName().c_str ());

          msgs.push_back(pair<void *, string> (el.data(),
                                               messageFactory.createMessage(
                                                 2006, 2,
                                                 el.elementName().c_str(),
                                                 childNameIt->c_str())));

          model.addElementWithErrorInLastPass(el.id());
        }
        break;

        // Child MUST be declared, AT LEAST, one time
      case '+': if (cardinalityMap[*childNameIt] < 1)
        {
          fprintf (stderr, "'%s', child of '%s', MUST be declared at least one"
                   "time\n", (*childNameIt).c_str (), el.elementName().c_str());
          msgs.push_back(pair<void *, string> (el.data(),
                                               messageFactory.createMessage(
                                                 2007, 2,
                                                 el.elementName().c_str(),
                                                 childNameIt->c_str())));

          model.addElementWithErrorInLastPass(el.id());
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
      default:
        fprintf (stderr, "Cardinality of '%s', child of '%s', is unknown\n",
                 (*childNameIt).c_str (), el.elementName().c_str ());
        break;
    }
  }

  //TODO: Acrescentar condição count*Operator > 1 nessas condições.
  // Test hashtag cardinality children
  if (hasHashtagChild && countHashtagOperator == 0)
  {
    fprintf (stderr, "'%s' MUST have one of these child: %s\n",
             el.elementName().c_str (), hashTagChildren.c_str ());

    msgs.push_back(pair<void *, string> (el.data(),
                                         messageFactory.createMessage(
                                           2007, 2, el.elementName().c_str(),
                                           hashTagChildren.c_str())));
    model.addElementWithErrorInLastPass(el.id());
  }

  // Test a cardinality children
  if (hasAChild)
  {
    if (countAOperator == 0)
    {
      fprintf (stderr, "'%s' MUST have one of these child: %s\n",
               el.elementName().c_str (), aChildren.c_str ());

      msgs.push_back(pair<void *, string> (el.data(),
                                           messageFactory.createMessage(
                                             2007, 2, el.elementName().c_str(),
                                             aChildren.c_str())));
      model.addElementWithErrorInLastPass(el.id());
    }
    else if (countAOperator > 1)
    {
      fprintf (stderr, "'%s' HAS to have just one of these child: %s\n",
               el.elementName().c_str (), aChildren.c_str ());

      msgs.push_back(pair<void *, string> (el.data(),
                                           messageFactory.createMessage(
                                             2014, 2, el.elementName().c_str(),
                                             aChildren.c_str())));
      model.addElementWithErrorInLastPass(el.id());
    }
  }

  // Test b cardinality children
  if (hasBChild)
  {
    if (countBOperator == 0)
    {
      fprintf (stderr, "'%s' MUST have one of these child: %s\n",
               el.elementName().c_str (), bChildren.c_str ());

      msgs.push_back(pair<void *, string> (el.data(),
                                           messageFactory.createMessage(
                                             2007, 2, el.elementName().c_str(),
                                             bChildren.c_str())));
      model.addElementWithErrorInLastPass(el.id());
    }
    else if (countBOperator > 1)
    {
      fprintf (stderr, "'%s' HAS to have just one of these child: %s\n",
               el.elementName().c_str (), bChildren.c_str ());

      msgs.push_back(pair<void *, string> (el.data(),
                                           messageFactory.createMessage(
                                             2014, 2, el.elementName().c_str(),
                                             bChildren.c_str())));

      model.addElementWithErrorInLastPass(el.id());
    }
  }

  return;
}


/*******************************************************************************
 ** This function will performs a recursively syntactic validation on elements.*
 ** It wil make the follow tests:  	 																					 *
 **																																						 *
 ** -Element exists?																													 *
 ** -All att are syntactically valid?									 												 *
 ** -Is child of parent?									 		 																 *
 ** -All children cardinality are right?									 										 *
 ******************************************************************************/
void StructuralValidation::structuralValidation(const ModelElement &el,
                                                Model &model,
                                                vector<pair<void *, string> >
                                                &msgs,
                                                Message &messageFactory)
{
  // FUNDAMENTAL TEST. Test if element exist.
  if (!Langstruct::existElement(el.elementName()))
  {
    // NoSuchElementError
    // Create a Message for unknow element
    fprintf (stderr, "NoSuchElementError: '%s'\n", el.elementName().c_str ());

    msgs.push_back(pair<void *, string> (el.data(),
                                         messageFactory.createMessage(
                                           2001, 1, el.elementName().c_str())));
    model.addElementWithErrorInLastPass(el.id());

    return;
  }

  //TODO: create a validate atts function
  testAttributes (el, model, msgs, messageFactory);

  string parent = el.parent() != "" ? model.element(el.parent())->elementName():
                                      "";
  if (!Langstruct::existParent(el.elementName(), parent))
  {
    // NoSuchParentError
    // Create a Message for unknow parent
    fprintf (stderr, "'%s' is not child of '%s'\n", el.elementName().c_str (),
             parent.c_str ());

    msgs.push_back(pair<void *, string> (el.data(),
                                         messageFactory.createMessage(
                                           2005, 2, el.elementName().c_str(),
                                           parent.c_str())));
    model.addElementWithErrorInLastPass(el.id());
  }

  // Validate children syntacticly and their cardinality
  vector<virtualId> children = el.children();
  map<string, int> cardinalityMap;

  vector<virtualId>::iterator childIt = children.begin ();
  for (ModelElement *child; childIt != children.end(); ++childIt)
  {
    child = model.element(*childIt);
    assert (child);

    ++cardinalityMap[child -> elementName()];

    // validate recursively
    //		syntacticValidation(*child, model, msgs);
  }

  testCardinality (el, model, cardinalityMap, msgs, messageFactory);

  ModelElement *parentElement = model.element(el.parent());
  if (parentElement)
  {
    if (model.markedElements().count(parentElement->id()) > 0)
      return;

    cardinalityMap.clear();
    children = parentElement->children();
    childIt = children.begin ();
    for (ModelElement *child; childIt != children.end(); ++childIt)
    {
      child = model.element(*childIt);
      assert (child);

      ++cardinalityMap[child -> elementName()];

    }

    testCardinality(*parentElement, model, cardinalityMap, msgs,messageFactory);
  }


  return;
}
