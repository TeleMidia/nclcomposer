#include "SearchBox.h"
#include "ui_SearchBox.h"

#include <QShowEvent>

SearchBox::SearchBox(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SearchBox)
{
  ui->setupUi(this);

  connect( ui->lineEdit_Find,
           SIGNAL(returnPressed()),
           SLOT(on_pushButton_FindNext_clicked()) );

  connect( ui->lineEdit_Find,
           SIGNAL(shiftReturnPressed()),
           SLOT(on_pushButton_FindPrevious_clicked()) );

  connect( ui->lineEdit_Find,
           SIGNAL(escPressed()),
           SLOT(on_pushButton_hide_clicked()) );
}

SearchBox::~SearchBox()
{
  delete ui;
}

const QString SearchBox::text() const
{
  return ui->lineEdit_Find->text();
}

const QString SearchBox::replaceText() const
{
  return ui->lineEdit_ReplaceWith->text();
}

void SearchBox::setFocusToFindLineEdit()
{
  ui->lineEdit_Find->setFocus();
  ui->lineEdit_Find->selectAll();
}

void SearchBox::on_pushButton_hide_clicked()
{
  emit hideButtonClicked();
}

void SearchBox::on_pushButton_FindPrevious_clicked()
{
  emit findPrevious(ui->lineEdit_Find->text());
}

void SearchBox::on_pushButton_FindNext_clicked()
{
  emit findNext(ui->lineEdit_Find->text());
}

void SearchBox::on_pushButton_ReplaceAndFind_clicked()
{
  emit replace( ui->lineEdit_Find->text(),
                ui->lineEdit_ReplaceWith->text(),
                true );
}

void SearchBox::on_pushButton_Replace_clicked()
{
  emit replace( ui->lineEdit_Find->text(),
                ui->lineEdit_ReplaceWith->text(),
                false );
}

void SearchBox::on_pushButton_ReplaceAll_clicked()
{
  emit replaceAll( ui->lineEdit_Find->text(),
                   ui->lineEdit_ReplaceWith->text() );
}

void SearchBox::on_lineEdit_Find_textChanged(const QString &arg1)
{
  if(ui->checkBox_SearchWhileTyping->isChecked())
    emit findNext(ui->lineEdit_Find->text());
}
