#include "SearchBox.h"
#include "ui_SearchBox.h"

#include <QShowEvent>

SearchBox::SearchBox (QWidget *parent)
    : QFrame (parent), _ui (new Ui::SearchBox)
{
  _ui->setupUi (this);

  connect (_ui->lineEdit_Find, SIGNAL (returnPressed ()),
           SLOT (on_pushButton_FindNext_clicked ()));

  connect (_ui->lineEdit_Find, SIGNAL (shiftReturnPressed ()),
           SLOT (on_pushButton_FindPrevious_clicked ()));

  connect (_ui->lineEdit_Find, SIGNAL (escPressed ()),
           SLOT (on_pushButton_hide_clicked ()));
}

SearchBox::~SearchBox () { delete _ui; }

const QString
SearchBox::text () const
{
  return _ui->lineEdit_Find->text ();
}

const QString
SearchBox::replaceText () const
{
  return _ui->lineEdit_ReplaceWith->text ();
}

void
SearchBox::setFocusToFindLineEdit ()
{
  _ui->lineEdit_Find->setFocus ();
  _ui->lineEdit_Find->selectAll ();
}

void
SearchBox::on_pushButton_hide_clicked ()
{
  emit hideButtonClicked ();
}

void
SearchBox::on_pushButton_FindPrevious_clicked ()
{
  emit findPrevious (_ui->lineEdit_Find->text ());
}

void
SearchBox::on_pushButton_FindNext_clicked ()
{
  emit findNext (_ui->lineEdit_Find->text ());
}

void
SearchBox::on_pushButton_ReplaceAndFind_clicked ()
{
  emit replace (_ui->lineEdit_Find->text (),
                _ui->lineEdit_ReplaceWith->text (), true);
}

void
SearchBox::on_pushButton_Replace_clicked ()
{
  emit replace (_ui->lineEdit_Find->text (),
                _ui->lineEdit_ReplaceWith->text (), false);
}

void
SearchBox::on_pushButton_ReplaceAll_clicked ()
{
  emit replaceAll (_ui->lineEdit_Find->text (),
                   _ui->lineEdit_ReplaceWith->text ());
}

void
SearchBox::on_lineEdit_Find_textChanged (const QString &arg1)
{
  Q_UNUSED (arg1);
  if (_ui->checkBox_SearchWhileTyping->isChecked ())
    emit findNext (_ui->lineEdit_Find->text ());
}
