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
