#include "ComposerQDockWidget.h"
#include <QPalette>
#include <QFocusFrame>

ComposerQDockWidget::ComposerQDockWidget(const QString &title,
                                                 QWidget *parent,
                                                 Qt::WindowFlags flags):
    QDockWidget(title, parent, flags)
{
    QFocusFrame *frame = new QFocusFrame(0);
    frame->setWidget(this);
}

void ComposerQDockWidget::enterEvent(QEvent *evt)
{
//    qDebug() << "focusInEvent";
//    titleBarWidget()->setStyleSheet("background: black;");
}

void ComposerQDockWidget::leaveEvent(QEvent *evt)
{
//    qDebug() << "focusOutEvent";
//    titleBarWidget()->setStyleSheet("background: lightgray;");
}
