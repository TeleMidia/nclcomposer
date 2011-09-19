/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTCONTEXT_H
#define QNSTCONTEXT_H

#include "qnstcomposite.h"

#include "qnstswitch.h"
#include "qnstport.h"
#include "qnstproperty.h"
#include "qnstmedia.h"

class QnstContext : public QnstComposite
{
    Q_OBJECT

public:
    QnstContext(QnstNode* parent = 0);

    ~QnstContext();

public slots:
    void addContext();

    void addSwitch();

    void addPort();

    void addProperty();

    void addMedia();

private:
    void createConnection();
};

#endif // QNSTCONTEXT_H
