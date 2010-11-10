#ifndef INTROPAGE_H
#define INTROPAGE_H

#include <QWizardPage>
#include <QLabel>
#include <QVBoxLayout>

class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit IntroPage(QWidget *parent = 0);
    inline void setLabelText(QString text) {
        if (labelIntro)
            labelIntro->setText(text);
    }

signals:
private:
    QLabel *labelIntro;
public slots:


};

#endif // INTROPAGE_H
