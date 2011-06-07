#include "QLineEditFilter.h"

QLineEditFilter::QLineEditFilter(QWidget *parent) : QLineEdit(parent)
{
    setStyleSheet("color: #bbbbbb;");
    setText(tr("Filter..."));
    isFilterText = true;
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
