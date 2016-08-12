#include "StructuralLinkDialog.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QStringList>

#include <QKeyEvent>
#include <QListView>
#include <QStringListModel>
#include <QDebug>

CompleteLineEdit::CompleteLineEdit(QStringList words, QWidget *parent)
  : QLineEdit(parent), words(words)
{
  listView = new QListView(this);
  model = new QStringListModel(this);

  listView->setWindowFlags(Qt::ToolTip);
  listView->setUniformItemSizes(true);

  installEventFilter(this);

  connect(this, SIGNAL(textChanged(const QString &)),
          this, SLOT(setCompleter(const QString &)));

  connect(listView, SIGNAL(clicked(const QModelIndex &)),
          this, SLOT(completeText(const QModelIndex &)));
}

void CompleteLineEdit::setStringList(const QStringList &words)
{
  this->words = words;
}

bool CompleteLineEdit::eventFilter(QObject *object, QEvent *event)
{
  if (object == this && event->type() == QEvent::KeyPress)
  {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_Tab)
    {
      if(!listView->isHidden())
      {
        QModelIndex currentIndex = listView->currentIndex();
        if (currentIndex.isValid())
        {
          QString text = listView->currentIndex().data().toString();
          setText(text);
        }
        listView->hide();
      }
    }
  }
  return false;
}

void CompleteLineEdit::hideEvent ( QHideEvent * event )
{
  Q_UNUSED(event)
  listView->hide();
}

// void CompleteLineEdit::focusOutEvent(QFocusEvent *e)
// {
//  QLineEdit::focusOutEvent(e);
//  listView->hide();
// }

void CompleteLineEdit::focusInEvent(QFocusEvent *e)
{
  setCompleter("");
  QLineEdit::focusInEvent(e);
}

void CompleteLineEdit::keyPressEvent(QKeyEvent *e)
{
  if (!listView->isHidden())
  {
    int key = e->key();
    int count = listView->model()->rowCount();
    QModelIndex currentIndex = listView->currentIndex();

    if (Qt::Key_Down == key)
    {
      int row = currentIndex.row() + 1;
      if (row >= count)
        row = 0;

      QModelIndex index = listView->model()->index(row, 0);
      listView->setCurrentIndex(index);
    }
    else if (Qt::Key_Up == key)
    {
      int row = currentIndex.row() - 1;
      if (row < 0)
      {
        row = count - 1;
      }

      QModelIndex index = listView->model()->index(row, 0);
      listView->setCurrentIndex(index);
    }
    else if (Qt::Key_Escape == key)
    {
      listView->hide();
    }
    else if (Qt::Key_Enter == key ||
             Qt::Key_Return == key )
    {
      if (currentIndex.isValid())
      {
        QString text = listView->currentIndex().data().toString();
        setText(text);
      }
      listView->hide();
    }
    else
    {
      // listView->hide();
      QLineEdit::keyPressEvent(e);
    }
  }
  else
  {
    QLineEdit::keyPressEvent(e);
  }
}

void CompleteLineEdit::setCompleter(const QString &text)
{
  /*if (text.isEmpty())
  {
    listView->hide();
    return;
  }

  if ((text.length() > 1) && (!listView->isHidden()))
    return;
  */

  QStringList sl;
  foreach(QString word, words)
  {
    if (word.toLower().contains(text.toLower()))
    {
      sl << word;
    }
  }

  model->setStringList(sl);
  listView->setModel(model);

  if (model->rowCount() == 0)
    return;

  // Position the text edit
  listView->setMinimumWidth(width());
  listView->setMaximumWidth(width());

  QPoint p(0, height());
  int x = mapToGlobal(p).x();
  int y = mapToGlobal(p).y() + 1;

  listView->move(x, y);
  listView->show();
}

void CompleteLineEdit::completeText(const QModelIndex &index)
{
  QString text = index.data().toString();
  setText(text);
  listView->hide();
}

StructuralLinkDialog::StructuralLinkDialog(QWidget* parent)
  : QDialog(parent), firstTime(true)
{
  form.setupUi(this);

  connLineEdit = new CompleteLineEdit(QStringList());
  this->form.gridLayout_2->addWidget(connLineEdit, 0, 1);

  connect(connLineEdit, SIGNAL(textChanged(QString)),
          SLOT(adjustBinds(QString)));

  changeModel = true;
}

StructuralLinkDialog::~StructuralLinkDialog()
{

}

void StructuralLinkDialog::init(QMap<QString, QVector<QString> > conditions,
                                QMap<QString, QVector<QString> > actions,
                                QMap<QString, QVector<QString> > params)
{

  _conditions = conditions;
  _actions = actions;
  _params = params;

  form.cbCondition->setEnabled(false);
  form.cbCondition->clear();

  form.cbAction->setEnabled(false);
  form.cbAction->clear();

  connLineEdit->clear();

  QStringList strConn;

  foreach (QString conn, _conditions.keys()) {
    strConn << conn;
  }

  connLineEdit->setStringList(strConn);

  if(firstTime)
  {
    setMinimumWidth(350);
    updateGeometry();

    QRect screenGeometry;

    if(this->parentWidget())
    {
      screenGeometry = QApplication::desktop()->screenGeometry(this->parentWidget());
    }
    else
    {
      screenGeometry = QApplication::desktop()->screenGeometry();
    }

    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    int x = screenGeometry.x() + (screenWidth - this->width()) / 2;
    int y = screenGeometry.y() + (screenHeight - this->height()) / 2;

    this->move(x, y);
    firstTime = false;
  }
}

void StructuralLinkDialog::adjustBinds(QString conn)
{

  form.cbCondition->setEnabled(true);
  form.cbCondition->clear();

  foreach (QString b, _conditions.value(conn)) {
    form.cbCondition->addItem(b);
  }

  form.cbAction->setEnabled(true);
  form.cbAction->clear();

  foreach (QString b, _actions.value(conn)) {
    form.cbAction->addItem(b);
  }
}

void StructuralLinkDialog::showEvent(QShowEvent *evt)
{
  Q_UNUSED(evt)
  this->connLineEdit->setText("");
  this->connLineEdit->setFocus();
}

QString StructuralLinkDialog::getCurrentConnector()
{
  return this->connLineEdit->text();
}
