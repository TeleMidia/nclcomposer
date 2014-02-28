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

#ifndef VALIDATOR_H_
#define VALIDATOR_H_

#include <list>

#include "../nclmodel/nclmodel.h"
#include "../langstruct/langstruct.h"
#include "../message/message.h"
#include <core/util/ComposerSettings.h>
#include <QDebug>

using namespace composer::core::util;

class Validator
{
public:
  static vector<pair<void *, string> > validate (Model &,
                                                 string messagesLanguage);
};


#endif /* VALIDATOR_H_ */
