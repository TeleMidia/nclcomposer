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

#include "Validator.h"
#include "../message/message.h"
#include "semanticvalidation.h"
#include "structuralvalidation.h"
#include <QDebug>
#include <set>

vector<pair<void *, string> >
Validator::validate (Model &model, const string &messagesLanguage = "en")
{
  vector<pair<void *, string> > msgs;

  set<virtualId> markeds = model.markedElements (),
                 affecteds = model.affectedElements (),
                 errorInLastPass = model.elementsWithErrorInLastPass ();

  Message messageFactory (messagesLanguage);
  set<virtualId>::iterator vIds = errorInLastPass.begin ();

  model.clearElementsWithErrorInLastPass ();

  for (ModelElement *el = NULL; vIds != errorInLastPass.end (); ++vIds)
  {
    el = model.element (*vIds);
    if (el)
    {
      StructuralValidation::structuralValidation (*el, model, msgs,
                                                  messageFactory);
      SemanticValidation::semanticValidation (*el, model, msgs,
                                              messageFactory);
    }
  }

  vIds = markeds.begin ();
  for (ModelElement *el = NULL; vIds != markeds.end (); ++vIds)
  {
    el = model.element (*vIds);

    if (el)
    {
      StructuralValidation::structuralValidation (*el, model, msgs,
                                                  messageFactory);
      SemanticValidation::semanticValidation (*el, model, msgs,
                                              messageFactory);
    }
  }
  vIds = affecteds.begin ();

  for (ModelElement *el = NULL; vIds != affecteds.end (); ++vIds)
  {
    el = model.element (*vIds);

    if (el)
      SemanticValidation::semanticValidation (*el, model, msgs,
                                              messageFactory);
  }
  return msgs;
}
