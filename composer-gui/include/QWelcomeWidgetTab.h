#ifndef QWELCOMEWIDGETTAB_H
#define QWELCOMEWIDGETTAB_H

#include <QObject>
#include <QTableWidget>

class QWelcomeWidgetTab : public QTabWidget
{
    Q_OBJECT

public:
    QWelcomeWidgetTab( QWidget *parent = NULL ) : QTabWidget( parent )
    {
        setTabsExpanding( true );
    }

    bool expanding() const
    {
        return tabBar()->expanding();
    }

    void setTabsExpanding( bool expanding = true )
    {
        tabBar()->setExpanding( expanding );
    }
};
#endif // QWELCOMEWIDGETTAB_H
