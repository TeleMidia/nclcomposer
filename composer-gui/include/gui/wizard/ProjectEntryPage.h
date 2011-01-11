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
        QString defaultLocation;

        void initGUI();
    private slots:
        void setProjectDirectory();
};

#endif // PROJECTENTRYPAGE_H
