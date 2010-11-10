#ifndef ENTRYPAGE_H
#define ENTRYPAGE_H

#include <QWizardPage>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QDir>
#include <QFileDialog>
#include <QCoreApplication>
#include <QPushButton>


class EntryPage : public QWizardPage
{
    Q_OBJECT
public:
    bool isFile;
    QString defaultLocation;
    explicit EntryPage(QWidget *parent = 0);
    void initGUI();


signals:

private:
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *locationLabel;
    QLineEdit *directoryLine;

private slots:
    void setProjectDirectory();
    void setDocumentLocation();

public slots:

};

#endif // ENTRYPAGE_H
