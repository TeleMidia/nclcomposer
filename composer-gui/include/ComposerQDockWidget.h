#ifndef COMPOSERQDOCKWIDGET_H
#define COMPOSERQDOCKWIDGET_H

#include <QWidget>
#include <QDockWidget>
#include <QDebug>

class ComposerQDockWidget : public QDockWidget
{
public:
     explicit ComposerQDockWidget(const QString &title, QWidget *parent = 0,
                                  Qt::WindowFlags flags = 0);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
};

#endif // COMPOSERQDOCKWIDGET_H
