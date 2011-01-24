#include "wizard/ProjectEntryPage.h"


ProjectEntryPage::ProjectEntryPage(QWidget *parent) :
        QWizardPage(parent)
{
    setTitle(tr("Fill the information"));
    setSubTitle(tr("Specify basic information about the project. \n"
                "Please fill both fields"));
    isOpen = false;
    initGUI();

}

ProjectEntryPage::~ProjectEntryPage() {

}

void ProjectEntryPage::initGUI() {

    if (defaultLocation == "")
        defaultLocation = QDir::homePath();

    nameLabel = new QLabel(tr("Name"),this);
    nameLineEdit = new QLineEdit(this);

    checkCpy = new QCheckBox(tr("Open an existing project"), this);
    checkCpy->setChecked(false);
    connect(checkCpy,SIGNAL(stateChanged(int)),SLOT(setOpenProject(int)));

    locationLabel = new QLabel(tr("Location"),this);
    directoryLine = new QLineEdit(this);
    directoryLine->setText(defaultLocation);
    directoryLine->setEnabled(false);

    locatorButton = new QPushButton(tr("Choose project location"),this);
    connect(locatorButton,SIGNAL(clicked()),
                this,SLOT(setProjectDirectory()));


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(nameLabel);
    layout->addWidget(nameLineEdit);
    layout->addWidget(checkCpy);
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
     if (directoryURI.endsWith(QDir::separator()))
         directoryURI.chop(1);

     if (isOpen)
     {
        QString aux = directoryURI;
        directoryLine->setText(aux.remove(
                aux.lastIndexOf(QDir::separator()),aux.length()));

        nameLineEdit->setText(directoryURI.remove(
                0,directoryURI.lastIndexOf(QDir::separator()) + 1));

     } else {
        if(!directoryURI.isEmpty()) directoryLine->setText(directoryURI);
     }
     delete dir;
     dir = NULL;
 }

void ProjectEntryPage::setOpenProject(int state)
{
    if (state == Qt::Checked) {
        locatorButton->setText(tr("Open Project"));
        nameLabel->setEnabled(false);
        nameLineEdit->setEnabled(false);
        isOpen = true;
    } else if(state == Qt::Unchecked) {
        locatorButton->setText(tr("Choose project location"));
        nameLabel->setEnabled(false);
        nameLineEdit->setEnabled(false);
        isOpen = false;
    }
}
