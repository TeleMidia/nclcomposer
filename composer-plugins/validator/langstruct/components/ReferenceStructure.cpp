/*
 * ReferenceStructure.cpp
 *
 *  Created on: 09/08/2011
 *      Author: Mária Gonçalves
 */

#include "ReferenceStructure.h"

namespace nclValidator {

ReferenceStructure::ReferenceStructure(string _from, string _fromAtt, string _to,
		string _toAtt, string _perspective, string _perspectiveAtt)
{
	from = _from;
	fromAtt = _fromAtt;
	to = _to;
	toAtt = _toAtt;
	perspective = _perspective;
        perspectiveAtt = _perspectiveAtt;
}


string ReferenceStructure::getFrom() const
{
    return from;
}

string ReferenceStructure::getFromAtt() const
{
    return fromAtt;
}


string ReferenceStructure::getTo() const
{
    return to;
}

string ReferenceStructure::getToAtt() const
{
    return toAtt;
}


string ReferenceStructure::getPerspective() const
{
    return perspective;
}


string ReferenceStructure::getPerspectiveAtt() const
{
    return perspectiveAtt;
}


ReferenceStructure::~ReferenceStructure() {}

}
