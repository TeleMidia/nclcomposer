/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef NCLVALIDATOR_H
#define NCLVALIDATOR_H

#include <vector>
using namespace std;

#include <QString>
#include <QFile>

class NCLValidator
{
private:
    vector <QString> errorMessages;
    vector <QString> warningMessages;

public:
    NCLValidator();
    bool validate (const QFile file);
    bool validate (const QString &text);

    bool begin();
    QString getNextErrorMessage();
    QString getNextWarningMessage();
};

#endif // NCLVALIDATOR_H
