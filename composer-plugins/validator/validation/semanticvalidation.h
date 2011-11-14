#ifndef SEMANTICVALIDATION_H
#define SEMANTICVALIDATION_H


#include "../nclmodel/nclmodel.h"
#include "../message/message.h"
#include <cstdio>

namespace nclValidator {

class SemanticValidation {
public:
    static void semanticValidation (const ModelElement &el, Model &model, vector<pair<void *, string> > &msgs, Message &messageFactory);
};

}

#endif // SEMANTICVALIDATION_H
