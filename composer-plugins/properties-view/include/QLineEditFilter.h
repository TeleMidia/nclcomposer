#ifndef QLINEEDITFILTER_H
#define QLINEEDITFILTER_H

#include <QLineEdit>
#include <QFocusEvent>

class QLineEditFilter : public QLineEdit
{
public:
    QLineEditFilter(QWidget *parent);
    virtual ~QLineEditFilter();

protected:
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);

private:
    bool isFilterText;
};

#endif // QLINEEDITFILTER_H
