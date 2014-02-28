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

#include "ReferenceStructure.h"


ReferenceStructure::ReferenceStructure(string _from, string _fromAtt,
																			 string _to, string _toAtt,
																			 string _perspective,
																			 string _perspectiveAtt)
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
