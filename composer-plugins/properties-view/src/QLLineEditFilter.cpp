/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "QLineEditFilter.h"

QLineEditFilter::QLineEditFilter(QWidget *parent) : QLineEdit(parent)
{
    setStyleSheet("color: #bbbbbb;");
    setText(tr("Filter..."));
    isFilterText = true;

    connect(this, SIGNAL(textEdited(QString)),
            this, SLOT(userTextEdited(QString)));
}

QLineEditFilter::~QLineEditFilter()
{

}

void QLineEditFilter::focusInEvent(QFocusEvent *focusEvent)
{
    if(isFilterText)
    {
        setText("");
        setStyleSheet("color: #000000;");
        isFilterText = false;
    }
    QLineEdit::focusInEvent(focusEvent);
}

void QLineEditFilter::focusOutEvent(QFocusEvent *focusEvent)
{
    if(text() == "") {
        isFilterText = true;
        setStyleSheet("color: #bbbbbb;");
        setText(tr("Filter..."));
    }
    QLineEdit::focusOutEvent(focusEvent);
}

void QLineEditFilter::userTextEdited(const QString &text)
{
    if(!isFilterText)
        emit filterTextChanged(text);
    else
        emit filterTextChanged(QString (""));
}
