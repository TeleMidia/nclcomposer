#include "include/wizard/ProjectEntryPage.h"


ProjectEntryPage::ProjectEntryPage(QWidget *parent) :
        QWizardPage(parent)
{
    setTitle(tr("Fill the information"));
    setSubTitle(tr("Specify basic information about the project. \n"
                "Please fill both fields"));
    initGUI();

}

ProjectEntryPage::~ProjectEntryPage() {

}

void ProjectEntryPage::initGUI() {

    if (defaultLocation == "")
        defaultLocation = QDir::homePath();

    nameLabel = new QLabel(tr("Name"),this);
    nameLineEdit = new QLineEdit(this);

    locationLabel = new QLabel(tr("Location"),this);
    directoryLine = new QLineEdit(this);
    directoryLine->setText(defaultLocation);
    directoryLine->setEnabled(false);

    QPushButton *locatorButton;
    locatorButton = new QPushButton(tr("Change location"),this);
    connect(locatorButton,SIGNAL(clicked()),
                this,SLOT(setProjectDirectory()));


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(nameLabel);
    layout->addWidget(nameLineEdit);
    layout->addWidget(locationLabel);
    layout->addWidget(directoryLine);
    layout->addWidget(locatorButton);
    setLayout(layout);
    registerField("name*", nameLineEdit);
    registerField("location", directoryLine);

}

void ProjectEntryPage::setProjectDirectory() {
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
