/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "util/AttributeReferences.h"

namespace composer {
    namespace util {

AttributeReferences:: AttributeReferences (    QString element,
                                               QString attr,
                                               QString ref_element,
                                               QString ref_attr)
{
    this->element = element;
    this->attr = attr;
    this->ref_element = ref_element;
    this->ref_attr = ref_attr;
}

QString AttributeReferences:: getElement()
{
    return this->element;
}

QString AttributeReferences::getAttribute()
{
    return this->attr;
}

QString AttributeReferences::getRefElement()
{
    return this->ref_element;
}

QString AttributeReferences::getRefAttribute()
{
    return this->ref_attr;
}

} } //end namespace
