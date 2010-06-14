#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <QLabel>
#include <QTextEdit>
#include <QWidget>
#include <QGroupBox>
#include <QToolBox>
#include "nclregion.h"
#define HEIGHT 20
#define WIDTH 60
#define TAM 8

class Attributes : public QGroupBox
{
    Q_OBJECT

public:
    Attributes (QGroupBox *parent=0);
    QSize sizeHint();

signals:
    void IdChanged (QString id, QString newId);
    void LeftChanged (QString id, QString newLeft);
    void TopChanged (QString id, QString newTop);
    void RightChanged (QString id, QString newRight);
    void BottomChanged (QString id, QString newBottom);
    void WidthChanged (QString id, QString newWidth);
    void HeightChanged (QString id, QString newHeight);
    void ZIndexChanged (QString id, QString newZIndex);
    void TitleChanged (QString id, QString newTitle);


public slots:
    void fillFields (NCLRegion *region);

    void changeId ();
    void changeLeft ();
    void changeTop ();
    void changeRight ();
    void changeBottom ();
    void changeWidth ();
    void changeHeight ();
    void changeZIndex ();
    void changeTitle ();

private:
    NCLRegion *region;

    QLabel *labelId;
    QLabel *labelLeft;
    QLabel *labelTop;
    QLabel *labelRight;
    QLabel *labelBottom;
    QLabel *labelWidth;
    QLabel *labelHeight;
    QLabel *labelZIndex;
    QLabel *labelTitle;

    QTextEdit *editId;
    QTextEdit *editLeft;
    QTextEdit *editTop;
    QTextEdit *editRight;
    QTextEdit *editBottom;
    QTextEdit *editWidth;
    QTextEdit *editHeigth;
    QTextEdit *editZIndex;
    QTextEdit *editTitle;

    QGroupBox *group;
};


#endif // ATTRIBUTES_H
