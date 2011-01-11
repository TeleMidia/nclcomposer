#include "../../../include/gui/wizard/EntryPage.h"

EntryPage::EntryPage(QWidget *parent) :
    QWizardPage(parent)
{
    this->defaultLocation = "";
    this->isFile = false;

}

void EntryPage::initGUI() {
    //TODO - mudar quando tiver noção de workspace
    if (defaultLocation == "")
        defaultLocation = QCoreApplication::applicationDirPath();

    setTitle(tr("Creation"));
    setSubTitle(tr("Please fill both fields."));

    nameLabel = new QLabel(tr("Name"),this);
    nameLineEdit = new QLineEdit(this);

    locationLabel = new QLabel(tr("Location"),this);
    directoryLine = new QLineEdit(this);
    directoryLine->setText(defaultLocation);
    directoryLine->setEnabled(false);
    QPushButton *locatorButton;
    if(isFile) {
        locatorButton =
            new QPushButton(tr("Choose NCL Document"),this);
        connect(locatorButton,SIGNAL(clicked()),
                this,SLOT(setDocumentLocation()));
    } else {
        locatorButton =
            new QPushButton(tr("Change location"),this);
        connect(locatorButton,SIGNAL(clicked()),
                this,SLOT(setProjectDirectory()));
    }

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameLineEdit, 0, 1);
    layout->addWidget(locationLabel, 1, 0);
    layout->addWidget(directoryLine, 1, 1);
    layout->addWidget(locatorButton, 1, 2);
    setLayout(layout);
    registerField("name*", nameLineEdit);
    registerField("location", directoryLine);
}

void EntryPage::setProjectDirectory() {
     QFileDialog::Options options = QFileDialog::ShowDirsOnly;
     QDir *dir = new QDir(defaultLocation);
     QString directoryURI = QFileDialog::getExistingDirectory(this,
                                 tr("Select the target directory"),
                                 dir->absolutePath(),
                                 options);
     if(!directoryURI.isEmpty()) directoryLine->setText(directoryURI);
     delete dir;
     dir = NULL;
 }

void EntryPage::setDocumentLocation() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open NCL Document"),
                                                    defaultLocation,
                                                    tr("NCL Document (*.ncl)"));
    if(fileName != "") directoryLine->setText(fileName);

}
