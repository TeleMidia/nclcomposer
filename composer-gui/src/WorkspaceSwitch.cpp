#include "../include/WorkspaceSwitch.h"

WorkspaceSwitch::WorkspaceSwitch(QWidget *parent) :
    QDialog(parent)
{
    init();
}

void WorkspaceSwitch::init()
{
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("Choose the workspace "
                                    "path for this session"),this));
    wsPath = new QLineEdit(this);
    wsPath->setEnabled(false);
    layout->addWidget(wsPath,1,0);
    openDir = new QPushButton(tr("Choose Directory"),this);
    connect(openDir,SIGNAL(clicked()),SLOT(openDirectory()));
    layout->addWidget(openDir,1,1);
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line,4,0,1,2);

    bOk = new QPushButton(tr("OK"),this);
    connect(bOk,SIGNAL(clicked()),SLOT(accept()));
    bCancel = new QPushButton(tr("Cancel"), this);
    connect(bCancel,SIGNAL(clicked()),SLOT(reject()));

    QHBoxLayout *bL = new QHBoxLayout(this);
    bL->addWidget(bCancel);
    bL->addWidget(bOk);
    layout->addLayout(bL,5,1);

    setLayout(layout);

}

void WorkspaceSwitch::openDirectory()
{
    QFileDialog::Options options = QFileDialog::ShowDirsOnly;
    QDir *dir = new QDir(QDir::homePath());
    QString directoryURI = QFileDialog::getExistingDirectory(this,
                                tr("Select the target directory"),
                                dir->absolutePath(),
                                options);
    wsPath->setText(directoryURI);
}
