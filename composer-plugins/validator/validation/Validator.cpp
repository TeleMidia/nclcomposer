/*
 * Validator.cpp
 *
 *  Created on: 29 Aug 2011
 *      Author: rios
 */

#include "Validator.h"
#include "semanticvalidation.h"
#include "structuralvalidation.h"
#include "../message/message.h"
#include <QDebug>
#include <set>


namespace nclValidator {

vector<pair<void *, string> >Validator::validate(Model &model)
 {
    vector<pair<void *, string> > msgs;

    set<virtualId> markeds = model.markedElements(),
                   affecteds = model.affectedElements(),
                   errorInLastPass = model.elementsWithErrorInLastPass();


    Message messageFactory;
//    //qDebug () << "Begin errorInLastPass";
    set<virtualId>::iterator vIds = errorInLastPass.begin();

    model.clearElementsWithErrorInLastPass();

    for (ModelElement *el = NULL; vIds != errorInLastPass.end(); ++vIds) {
        el = model.element(*vIds);
        //qDebug () << "sdiuhusid";
        if (el){
            StructuralValidation::structuralValidation (*el, model, msgs, messageFactory);
            SemanticValidation::semanticValidation (*el, model, msgs, messageFactory);
        }
    }

    //qDebug () <<  "End errorInLastPass";
    vIds = markeds.begin();
    //qDebug () << "Begin markeds";
    for (ModelElement *el = NULL; vIds != markeds.end(); ++vIds) {
        el = model.element(*vIds);

        if (el){
            StructuralValidation::structuralValidation (*el, model, msgs, messageFactory);
            SemanticValidation::semanticValidation (*el, model, msgs, messageFactory);
        }
    }
    //qDebug () << "End markeds";
    vIds = affecteds.begin();

    //qDebug () << "Begin affecteds";
    for (ModelElement *el = NULL; vIds != affecteds.end(); ++vIds) {
        el = model.element(*vIds);

        if (el)
            SemanticValidation::semanticValidation (*el, model, msgs, messageFactory);
    }
    //qDebug () << "End affecteds";
    return msgs;

}

}
