#ifndef PROJECTENTRYPAGE_H
#define PROJECTENTRYPAGE_H

#include <QWizardPage>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QPushButton>
#include <QCheckBox>
#include <QDebug>

class ProjectEntryPage : public QWizardPage
{
    Q_OBJECT
    public:
        ProjectEntryPage(QWidget *parent = 0);
        ~ProjectEntryPage();

    private:
        QLabel *nameLabel;
        QLineEdit *nameLineEdit;
        QLabel *locationLabel;
        QLineEdit *directoryLine;
        QPushButton *locatorButton;
        QString defaultLocation;
        QCheckBox *checkCpy;
        bool isOpen;

        void initGUI();
    private slots:
        void setProjectDirectory();
        void setOpenProject(int state);
};

#endif // PROJECTENTRYPAGE_H
