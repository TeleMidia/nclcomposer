#include "fileattrinput.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

FileAttrInput::FileAttrInput(QString question, QString name, QString type, QString value, QWidget *parent)
    : AttrInput (question, name, type, value, parent)
{
  QHBoxLayout *mainLayout = new QHBoxLayout;
  QHBoxLayout *lineLayout = new QHBoxLayout;

  QLabel *label = new QLabel (question);

  _answer = new QLineEdit;
  QPushButton *fileSearch = new QPushButton ("&Browse...");

  connect (fileSearch, SIGNAL(pressed()), this, SLOT(searchFile()));

  lineLayout->addWidget(_answer);
  lineLayout->addWidget(fileSearch);

  mainLayout->addWidget(label);
  mainLayout->addLayout(lineLayout);

  setLayout(mainLayout);
}

void FileAttrInput::searchFile()
{
  QString filePath = QFileDialog::getOpenFileName(this, "File", "Select a file");
  if (filePath != "")
    setAnswer(filePath);
}
