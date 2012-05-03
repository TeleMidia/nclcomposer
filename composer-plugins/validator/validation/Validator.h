/*
 * Validator.h
 *
 *  Created on: 29 Aug 2011
 *      Author: rios
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

namespace nclValidator {

class Validator {
	public:
                static vector<pair<void *, string> > validate (Model &, string messagesLanguage);
};

}

#endif /* VALIDATOR_H_ */
