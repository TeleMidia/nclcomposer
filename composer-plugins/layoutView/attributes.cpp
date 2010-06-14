#include "attributes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <iostream>


Attributes::Attributes (QGroupBox *parent)
        : QGroupBox (parent)
{
    //group = new QGroupBox (tr("Properties"));

    setTitle(tr("Properties"));

    //setCheckable(true);

    labelId = new QLabel ("Id   ");
    labelLeft = new QLabel ("Left");
    labelTop = new QLabel ("Top ");
    labelRight = new QLabel ("Right    ");
    labelBottom = new QLabel ("Bottom");
    labelWidth = new QLabel ("Width ");
    labelHeight = new QLabel ("Height");
    labelZIndex = new QLabel ("zIndex");
    labelTitle = new QLabel ("Title     ");

    editId = new QTextEdit ;
    editId->setFixedHeight(HEIGHT);
    editId->setFixedWidth(WIDTH);

    editLeft = new QTextEdit ;
    editLeft->setFixedHeight(HEIGHT);
    editLeft->setFixedWidth(WIDTH);

    editTop = new QTextEdit ;
    editTop->setFixedHeight(HEIGHT);
    editTop->setFixedWidth(WIDTH - 1);

    editRight = new QTextEdit ;
    editRight->setFixedHeight(HEIGHT);
    editRight->setFixedWidth(WIDTH);

    editBottom = new QTextEdit;
    editBottom->setFixedHeight(HEIGHT);
    editBottom->setFixedWidth(WIDTH);

    editWidth = new QTextEdit ;
    editWidth->setFixedHeight(HEIGHT);
    editWidth->setFixedWidth(WIDTH);

    editHeigth = new QTextEdit ;
    editHeigth->setFixedHeight(HEIGHT);
    editHeigth->setFixedWidth(WIDTH);

    editZIndex = new QTextEdit ;
    editZIndex->setFixedHeight(HEIGHT);
    editZIndex->setFixedWidth(WIDTH);

    editTitle = new QTextEdit ;
    editTitle->setFixedHeight(HEIGHT);
    editTitle->setFixedWidth(WIDTH);

    labelId->setBuddy(editId);
    labelLeft->setBuddy(editLeft);
    labelTop->setBuddy(editTop);
    labelRight->setBuddy(editRight);
    labelBottom->setBuddy(editBottom);
    labelWidth->setBuddy(editWidth);
    labelHeight->setBuddy(editHeigth);
    labelZIndex->setBuddy(editZIndex);
    labelTitle->setBuddy(editTitle);


    QHBoxLayout *layoutId = new QHBoxLayout;
    layoutId->addWidget(labelId);
    //layoutId->addSpacerItem(new QSpacerItem( WIDTH - 15, HEIGHT));
    layoutId->addWidget(editId);
    QHBoxLayout *layoutLeft = new QHBoxLayout;
    layoutLeft->addWidget(labelLeft);
    layoutLeft->addWidget(editLeft);
    QHBoxLayout *layoutTop = new QHBoxLayout;
    layoutTop->addWidget(labelTop);
    layoutTop->addWidget(editTop);

    QVBoxLayout *first = new QVBoxLayout;
    first->addLayout(layoutId);
    first->addLayout(layoutLeft);
    first->addLayout(layoutTop);

    QHBoxLayout *layoutTitle = new QHBoxLayout;
    layoutTitle->addWidget(labelTitle);
    //layoutLeft->addSpacerItem(new QSpacerItem( WIDTH - 28, HEIGHT));
    layoutTitle->addWidget(editTitle);
    QHBoxLayout *layoutRight = new QHBoxLayout;
    layoutRight->addWidget(labelRight);
    layoutRight->addWidget(editRight);
    QHBoxLayout *layoutBottom = new QHBoxLayout;
    layoutBottom->addWidget(labelBottom);
    layoutBottom->addWidget(editBottom);

    QVBoxLayout *second = new QVBoxLayout;
    second->addLayout(layoutTitle);
    second->addLayout(layoutRight);
    second->addLayout(layoutBottom);

    QHBoxLayout *layoutZIndex = new QHBoxLayout;
    layoutZIndex->addWidget(labelZIndex);
    //layoutTop->addSpacerItem(new QSpacerItem( WIDTH - 25, HEIGHT));
    layoutZIndex->addWidget(editZIndex);
    QHBoxLayout *layoutWidth = new QHBoxLayout;
    layoutWidth->addWidget(labelWidth);
    layoutWidth->addWidget(editWidth);
    QHBoxLayout *layoutHeight = new QHBoxLayout;
    layoutHeight->addWidget(labelHeight);
    layoutHeight->addWidget(editHeigth);

    QVBoxLayout *third = new QVBoxLayout;
    third->addLayout(layoutZIndex);
    third->addLayout(layoutWidth);
    third->addLayout(layoutHeight);

    /*QHBoxLayout *layoutRight = new QHBoxLayout;
    layoutRight->addWidget(labelRight);
    layoutRight->addSpacerItem(new QSpacerItem( WIDTH - 35, HEIGHT));
    layoutRight->addWidget(editRight);

    QHBoxLayout *layoutBottom = new QHBoxLayout;
    layoutBottom->addWidget(labelBottom);
    layoutBottom->addWidget(editBottom);

    QHBoxLayout *layoutWidth = new QHBoxLayout;
    layoutWidth->addWidget(labelWidth);
    layoutWidth->addSpacerItem(new QSpacerItem( WIDTH - 40, HEIGHT));
    layoutWidth->addWidget(editWidth);

    QHBoxLayout *layoutHeight = new QHBoxLayout;
    layoutHeight->addWidget(labelHeight);
    layoutHeight->addSpacerItem(new QSpacerItem( WIDTH - 44, HEIGHT));
    layoutHeight->addWidget(editHeigth);

    QHBoxLayout *layoutZIndex = new QHBoxLayout;
    layoutZIndex->addWidget(labelZIndex);
    layoutZIndex->addSpacerItem(new QSpacerItem( WIDTH - 44, HEIGHT));
    layoutZIndex->addWidget(editZIndex);

    QHBoxLayout *layoutTitle = new QHBoxLayout;
    layoutTitle->addWidget(labelTitle);
    layoutTitle->addSpacerItem(new QSpacerItem( WIDTH - 30, HEIGHT));
    layoutTitle->addWidget(editTitle);*/

    QHBoxLayout *layoutMain = new QHBoxLayout;
    layoutMain->addLayout(first);
    layoutMain->addSpacerItem(new QSpacerItem (100, 100));
    layoutMain->addLayout(second);
    layoutMain->addSpacerItem(new QSpacerItem (100, 100));
    layoutMain->addLayout(third);
/*    layoutMain->addLayout(layoutTop);
    layoutMain->addLayout(layoutBottom);
    layoutMain->addLayout(layoutWidth);
    layoutMain->addLayout(layoutHeight);
    layoutMain->addLayout(layoutZIndex);
    layoutMain->addLayout(layoutTitle);*/
    layoutMain->addStretch(1);
    setLayout(layoutMain);

    setFlat(true);
    connect (editId, SIGNAL (textChanged()),
             this, SLOT (changeId ()));

    connect (editLeft, SIGNAL (textChanged()),
             this, SLOT (changeLeft()));

    connect (editRight, SIGNAL (textChanged()),
             this, SLOT (changeRight()));

    connect (editTop, SIGNAL (textChanged()),
             this, SLOT (changeTop()));

    connect (editBottom, SIGNAL (textChanged()),
             this, SLOT (changeBottom()));

    connect (editWidth, SIGNAL (textChanged()),
            this, SLOT (changeWidth()));

    connect (editHeigth, SIGNAL (textChanged()),
             this, SLOT (changeHeight()));

    connect (editZIndex, SIGNAL (textChanged()),
             this, SLOT (changeZIndex()));

    connect (editTitle, SIGNAL (textChanged()),
             this, SLOT (changeTitle()));
}


void Attributes::changeId()
{
    //emit IdChanged(region->getId(), editId->text());
}

void Attributes::changeLeft()
{
    std::cout << "left";
   // emit LeftChanged(region->getId(), editLeft->text());
}

void Attributes::changeTop()
{
    //emit TopChanged(region->getId(), editTop->text());
}

void Attributes::changeRight()
{
   // emit RightChanged(region->getId(), editRight->text());
}

void Attributes::changeBottom()
{
   // emit BottomChanged(region->getId(), editBottom->text());
}

void Attributes::changeWidth()
{
    //emit WidthChanged(region->getId(), editWidth->text());
}

void Attributes::changeHeight()
{
    //emit HeightChanged(region->getId(), editHeigth->text());
}

void Attributes::changeZIndex()
{
   // emit ZIndexChanged(region->getId(), editZIndex->text());
}

void Attributes::changeTitle()
{
    //emit TitleChanged(region->getId(), editTitle->text());
}

void Attributes::fillFields(NCLRegion *region)
{
    if (region == NULL){
        editBottom->setText("");
        editHeigth->setText("");
        editId->setText("");
        editLeft->setText("");
        editRight->setText("");
        editTitle->setText("");
        editTop->setText("");
        editWidth->setText("");
        editZIndex->setText("");
        return;
    }

    /*editLeft->setHidden(false);
    editTop->setHidden(false);
    editRight->setHidden(false);
    editBottom->setHidden(false);
    editWidth->setHidden(false);
    editHeigth->setHidden(false);*/


    QString num;

    editId->setText(region->getId());

    if (region->leftAdjusted() != -1) {
        num.setNum(region->leftAdjusted() * 100);
        editLeft->setText(num);
        num.clear();
    }
    //else
    //    editLeft->setHidden(true);

    if (region->topAdjusted() != -1) {
        num.setNum(region->topAdjusted() * 100);
        editTop->setText(num);
        num.clear();
    }
    //else
    //    editTop->setHidden(true);

    if (region->rightAdjusted() != -1){
        num.setNum(region->rightAdjusted() * 100);
        editRight->setText(num);
        num.clear();
    }
    //else
    //    editRight->setHidden(true);

    if (region->bottomAdjusted() != -1){
        num.setNum(region->bottomAdjusted() * 100);
        editBottom->setText(num);
        num.clear();
    }
    //else
    //    editBottom->setHidden(true);

    if (region->widthAdjusted() != -1) {
        num.setNum(region->widthAdjusted() * 100);
        editWidth->setText(num);
        num.clear();
    }
    //else
    //    editWidth->setHidden(true);

    if (region->heightAdjusted()!= -1){
        num.setNum(region->heightAdjusted() * 100);
        editHeigth->setText(num);
        num.clear();
    }
    //else
    //    editHeigth->setHidden(true);


    num.setNum(region->zIndex());
    editZIndex->setText(num);
    num.clear();

    editTitle->setText(region->title());

}

QSize Attributes::sizeHint()
{
    return QSize (100, 100);
}
