/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTCOMPOSITE_H
#define QNSTCOMPOSITE_H

#include "qnstnode.h"

class QnstComposite : public QnstNode
{
public:
    QnstComposite(QnstNode* parent = 0);

    ~QnstComposite();
};

#endif // QNSTCOMPOSITE_H
