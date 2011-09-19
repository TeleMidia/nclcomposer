/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTCONTENT_H
#define QNSTCONTENT_H

#include "qnstnode.h"


class QnstContent : public QnstNode
{
public:
    QnstContent(QnstNode* parent = 0);

    ~QnstContent();
};

#endif // QNSTCONTENT_H
