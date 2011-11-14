#ifndef STRUCTURALVALIDATION_H
#define STRUCTURALVALIDATION_H

#include "../nclmodel/nclmodel.h"
#include "../message/message.h"
#include <cstdio>

namespace nclValidator {


class StructuralValidation {
public:
    static void structuralValidation(const ModelElement &el, Model &model, vector<pair<void *, string> > &msgs, Message &messageFactory);
};

}

#endif // STRUCTURALVALIDATION_H
