/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnstcomposite.h"

QnstComposite::QnstComposite(QnstNode* parent)
    : QnstNode(parent)
{
    bodyAction->setEnabled(false);

    portAction->setEnabled(true);
    propertyAction->setEnabled(true);
    contextAction->setEnabled(true);
    switchAction->setEnabled(true);
    mediaAction->setEnabled(true);
}

QnstComposite::~QnstComposite()
{

}
