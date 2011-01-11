#ifndef DOCUMENTENTRYPAGE_H
#define DOCUMENTENTRYPAGE_H

#include <QWizardPage>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QPushButton>
#include <QGroupBox>

class DocumentEntryPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DocumentEntryPage(QWidget *parent = 0);
private:
    void initGUI();
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *locationLabel;
    QLineEdit *directoryLine;
    QPushButton *locatorButton;
    QGroupBox *copyBox;
    QString defaultLocation;
signals:

public slots:

private slots:
    void setDocumentLocation();

};

#endif // DOCUMENTENTRYPAGE_H
