#include "include/gui/wizard/DocumentEntryPage.h"

DocumentEntryPage::DocumentEntryPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Fill the information"));
    setSubTitle(tr("Specify basic information about the document. \n"
                "You can add a existing document to project"));
    setCopy(false);
    initGUI();
}

void DocumentEntryPage::initGUI() {

    if (defaultLocation == "")
        defaultLocation = QDir::homePath();

    nameLabel = new QLabel(tr("Name"),this);
    nameLineEdit = new QLineEdit(this);

    locationLabel = new QLabel(tr("Location"),this);
    directoryLine = new QLineEdit(this);
    directoryLine->setText(defaultLocation);

    locatorButton = new QPushButton(tr("Open Document"),this);
    connect(locatorButton,SIGNAL(clicked()),SLOT(setDocumentLocation()));

    copyBox = new QGroupBox(this);
    copyBox->setTitle("Copy a NCL Document");
    copyBox->setCheckable(true);
    connect(copyBox,SIGNAL(clicked(bool)),SLOT(setCopy(bool)));


    QVBoxLayout *layoutGroup = new QVBoxLayout(copyBox);
    layoutGroup->addWidget(locationLabel);
    layoutGroup->addWidget(directoryLine);
    layoutGroup->addWidget(locatorButton);
    copyBox->setLayout(layoutGroup);
    copyBox->setChecked(false);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(nameLabel);
    layout->addWidget(nameLineEdit);
    layout->addWidget(copyBox);

    setLayout(layout);
    registerField("name*", nameLineEdit);
    registerField("location", directoryLine);
    registerField("copyCheck", copyBox);

}

void DocumentEntryPage::setDocumentLocation() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open NCL Document"),
                                                    defaultLocation,
                                                    tr("NCL Document (*.ncl)"));
    if(fileName != "") {
        directoryLine->setText(fileName);
        nameLineEdit->setText(fileName.remove(0,
                              fileName.lastIndexOf(QDir::separator())+1));
    }

}
