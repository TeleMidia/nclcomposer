/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTMEDIA_H
#define QNSTMEDIA_H

#include "qnstcontent.h"

#include "qnstport.h"
#include "qnstarea.h"
#include "qnstproperty.h"

class QnstMedia : public QnstContent
{
    Q_OBJECT

public:
    QnstMedia(QnstNode* parent = 0);

    ~QnstMedia();

public slots:
    void addArea();

private:
    void createConnection();
};

#endif // QNSTMEDIA_H
