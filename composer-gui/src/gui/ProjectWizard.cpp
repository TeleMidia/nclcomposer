#include "../../include/gui/ProjectWizard.h"

ProjectWizard::ProjectWizard(QWidget *par) : QWizard(par) {
    addPage(new IntroPage(this));
    addPage(new EntryPage(this));
    setWindowTitle("New Project Wizard");
}

void ProjectWizard::init() {
    this->show();
    this->exec();
}

ProjectWizard::~ProjectWizard() {
    //qDebug() << "Calling Project Wizard Destructor";
}

IntroPage::IntroPage(QWidget *parent) : QWizardPage(parent) {
    setTitle("Introduction");

    labelIntro = new QLabel(tr("This wizard is going to help you create a"
                                  " new Project."),this);
    labelIntro->setWordWrap(true);
    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->addWidget(labelIntro);
    setLayout(boxLayout);
}

EntryPage::EntryPage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Creation"));
    setSubTitle(tr("Please fill both fields."));

    nameLabel = new QLabel(tr("Project Name:"), this);
    nameLineEdit = new QLineEdit(this);

    locationLabel = new QLabel(tr("Project Location:"),this);
    directoryLine = new QLineEdit(this);
    directoryLine->setEnabled(false);
    QPushButton *locatorButton =
                 new QPushButton(tr("Choose Location"),this);

    connect(locatorButton,SIGNAL(clicked()),this,SLOT(setProjectDirectory()));

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameLineEdit, 0, 1);
    layout->addWidget(locationLabel, 1, 0);
    layout->addWidget(directoryLine, 1, 1);
    layout->addWidget(locatorButton, 1, 2);
    setLayout(layout);

    registerField("projectName*", nameLineEdit);
    registerField("location*", directoryLine);
}

void EntryPage::setProjectDirectory() {
     QFileDialog::Options options = QFileDialog::ShowDirsOnly;
     QDir *dir = new QDir(QCoreApplication::applicationDirPath());
     dir->cdUp();
     dir->cdUp();
     QString directoryURI = QFileDialog::getExistingDirectory(this,
                                 tr("Select the target directory"),
                                 dir->absolutePath(),
                                 options);
     if(!directoryURI.isEmpty()) directoryLine->setText(directoryURI);
     delete dir;
     dir = NULL;
 }

void ProjectWizard::accept() {
    projectName = field("projectName").toString();
    location    = field("location").toString();
    emit infoReceived(projectName,location);
    QDialog::accept();
}
