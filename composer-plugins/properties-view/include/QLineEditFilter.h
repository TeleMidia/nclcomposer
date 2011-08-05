/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QLINEEDITFILTER_H
#define QLINEEDITFILTER_H

#include <QLineEdit>
#include <QFocusEvent>

class QLineEditFilter : public QLineEdit
{
    Q_OBJECT

public:
    QLineEditFilter(QWidget *parent);
    virtual ~QLineEditFilter();

protected:
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);

private slots:
    void userTextEdited(const QString &);

private:
    bool isFilterText;

signals:
    void filterTextChanged(const QString &);

};

#endif // QLINEEDITFILTER_H
