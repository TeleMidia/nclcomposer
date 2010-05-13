#ifndef PROJECTWIZARD_H
#define PROJECTWIZARD_H

#include <QDebug>

#include <QWizard>
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <QCoreApplication>
#include <QWidget>
#include <QVariant>
#include <QDir>

class ProjectWizard : public QWizard {
    Q_OBJECT
    private:
        QString projectName;
        QString location;
    public:
        ProjectWizard(QWidget*);
        ~ProjectWizard();
        void init();
        void accept();
    signals:
        void infoReceived(QString,QString);

};

class IntroPage : public QWizardPage {
    Q_OBJECT
    public:
        IntroPage(QWidget *parent = 0);
    private:
        QLabel *labelIntro;
};

class EntryPage : public QWizardPage {
    Q_OBJECT
    public:
        EntryPage(QWidget *parent = 0);
    private:
        QLabel *nameLabel;
        QLineEdit *nameLineEdit;
        QLabel *locationLabel;
        QLineEdit *directoryLine;
    private slots:
        void setProjectDirectory();



};

#endif // PROJECTWIZARD_H
