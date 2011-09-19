/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTBODY_H
#define QNSTBODY_H

#include "qnstcomposite.h"

#include "qnstswitch.h"
#include "qnstcontext.h"
#include "qnstport.h"
#include "qnstproperty.h"
#include "qnstmedia.h"

class QnstBody : public QnstComposite
{
    Q_OBJECT

public:
    QnstBody(QnstNode* parent = 0);

    ~QnstBody();

public slots:
    void addContext();

    void addSwitch();

    void addPort();

    void addProperty();

    void addMedia();

private:
    void createConnection();
};

#endif // QNSTBODY_H
