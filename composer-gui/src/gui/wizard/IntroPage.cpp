#include "../../../include/gui/wizard/IntroPage.h"

IntroPage::IntroPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("Introduction");
    labelIntro = new QLabel(this);
    labelIntro->setWordWrap(true);
    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->addWidget(labelIntro);
    setLayout(boxLayout);
}
