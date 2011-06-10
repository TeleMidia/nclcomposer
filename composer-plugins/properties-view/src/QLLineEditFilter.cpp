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
