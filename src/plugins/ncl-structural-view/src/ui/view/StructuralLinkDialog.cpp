#include "StructuralLinkDialog.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QStringList>

#include <QKeyEvent>
#include <QListView>
#include <QStringListModel>
#include <QDebug>

#include <QStandardItemModel>
#include <QTimer>

#include "StructuralUtil.h"

CompleteLineEdit::CompleteLineEdit(const QStringList &words,
                                   QWidget *parent)
  : QLineEdit(parent), _words(words)
{
  _listView = new QListView (this);
  _model = new QStringListModel (this);
  _listView->setModel(_model);

  _listView->setWindowFlags(Qt::ToolTip);
  _listView->setUniformItemSizes(true);
  _listView->setHidden(true);

  installEventFilter(this);

  connect(this,
          SIGNAL(textEdited(const QString &)),
          this,
          SLOT(setCompleter(const QString &)));

  connect(_listView,
          SIGNAL(pressed(const QModelIndex &)),
          this,
          SLOT(completeText(const QModelIndex &)));
}

void CompleteLineEdit::setStringList(const QStringList &words)
{
  this->_words = words;
  setCompleter(text());
}

bool CompleteLineEdit::eventFilter(QObject *object, QEvent *event)
{
  if ( object == this &&
       event->type() == QEvent::KeyPress )
  {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_Tab)
    {
      if(!_listView->isHidden())
      {
        QModelIndex currentIndex = _listView->currentIndex();
        if (currentIndex.isValid())
        {
          QString text = _listView->currentIndex().data().toString();
          setText(text);
        }

        _listView->hide();
      }
    }
  }

  return false;
}

void CompleteLineEdit::updateListPos()
{
  QPoint p(0, height());
  int x = mapToGlobal(p).x();
  int y = mapToGlobal(p).y() + 1;

  _listView->move(x, y);
}

void CompleteLineEdit::updateListSize()
{
  setMinimumWidth(_listView->sizeHintForColumn(0));
  setMaximumWidth(_listView->sizeHintForColumn(0));
  _listView->setMinimumWidth(_listView->sizeHintForColumn(0));
  _listView->setMaximumWidth(_listView->sizeHintForColumn(0));
}

void CompleteLineEdit::focusOutEvent(QFocusEvent *e)
{
  QLineEdit::focusOutEvent(e);
  _listView->hide();
}

void CompleteLineEdit::focusInEvent(QFocusEvent *e)
{
  QLineEdit::focusInEvent(e);

  setCompleter(text());
  updateListPos();

  _listView->show();
}

void CompleteLineEdit::keyPressEvent(QKeyEvent *e)
{
  if (!_listView->isHidden())
  {
    int key = e->key();
    int count = _listView->model()->rowCount();
    QModelIndex currentIndex = _listView->currentIndex();

    if (Qt::Key_Down == key)
    {
      int row = currentIndex.row() + 1;
      if (row >= count)
        row = 0;

      QModelIndex index = _listView->model()->index(row, 0);
      _listView->setCurrentIndex(index);
    }
    else if (Qt::Key_Up == key)
    {
      int row = currentIndex.row() - 1;
      if (row < 0)
      {
        row = count - 1;
      }

      QModelIndex index = _listView->model()->index(row, 0);
      _listView->setCurrentIndex(index);
    }
    else if (Qt::Key_Escape == key)
    {
      _listView->hide();
    }
    else if (Qt::Key_Enter == key ||
             Qt::Key_Return == key )
    {
      if (currentIndex.isValid())
      {
        QString text = _listView->currentIndex().data().toString();
        setText(text);
      }

      _listView->hide();
    }
    else
    {
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
  QString txt = text.isEmpty() ? "" : text;
  QStringList sl;

  for (const QString &word: _words)
  {
    if (word.toLower().contains(txt.toLower()))
      sl << word;
  }

  _model->setStringList(sl);
  _listView->show();
}

void CompleteLineEdit::completeText(const QModelIndex &index)
{
  QString text = index.data().toString();
  setText(text);
  _listView->hide();
  nextInFocusChain()->setFocus();
}

StructuralLinkDialog::StructuralLinkDialog(QWidget* parent)
  : QDialog(parent), firstTime(true)
{
  form.setupUi(this);

  connLineEdit = new CompleteLineEdit(QStringList(), this);
  this->form.gridLayout_2->addWidget(connLineEdit, 0, 1);

  connect(connLineEdit, SIGNAL(textChanged(QString)),
          SLOT(updateForm(QString)));

  connect(form.kbLinkParams, SIGNAL(stateChanged(int)), this, SLOT(changeLinkParamState(int)));
  connect(form.kbConditionParams, SIGNAL(stateChanged(int)), this, SLOT(changeConditionParamState(int)));
  connect(form.kbActionParams, SIGNAL(stateChanged(int)), this, SLOT(changeActionParamState(int)));

  changeModel = true;

  form.tbLinkParams->hide();
  form.tbConditionParams->hide();
  form.tbActionParams->hide();

  form.gridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
  form.gridLayout_2->setSizeConstraint(QLayout::SetMinAndMaxSize);
  form.gridLayout_3->setSizeConstraint(QLayout::SetMinAndMaxSize);

  _currentMode = CreateLink;
}

StructuralLinkDialog::~StructuralLinkDialog()
{

}

void StructuralLinkDialog::setData(const QMap<QString, QVector<QString> > &conditions,
                                   const QMap<QString, QVector<QString> > &actions,
                                   const QMap<QString, QVector<QString> > &params)
{
  _conditions = conditions;
  _actions = actions;
  _params = params;
}

void StructuralLinkDialog::init( const QString &connName,
                                 const QString &condName,
                                 const QString &actionName,
                                 LinkDialogMode mode)
{
  _currentMode = mode;

  connLineEdit->clear();
  connLineEdit->setStringList(_conditions.keys());

  switch (_currentMode)
  {
    case CreateLink:
      setWindowTitle(tr("Create Link"));

      connLineEdit->setEnabled(true);

      form.tbLinkParams->setEnabled(true);
      form.kbLinkParams->setEnabled(true);
      form.kbLinkParams->setChecked(false);

      form.cbCondition->setEnabled(true);
      form.tbConditionParams->setEnabled(true);
      form.kbConditionParams->setEnabled(true);
      form.kbConditionParams->setChecked(false);

      form.cbAction->setEnabled(true);
      form.tbActionParams->setEnabled(true);
      form.kbActionParams->setEnabled(true);
      form.kbActionParams->setChecked(false);

      break;

    case EditLink:
      setWindowTitle(tr("Edit Link"));

      // Disable editing of xconnector property
      // for now.
      connLineEdit->setEnabled(false);

      form.tbLinkParams->setEnabled(true);
      form.kbLinkParams->setEnabled(true);
      form.kbLinkParams->setChecked(true);

      form.cbCondition->setEnabled(false);
      form.tbConditionParams->setEnabled(false);
      form.kbConditionParams->setEnabled(false);
      form.kbConditionParams->setChecked(false);

      form.cbAction->setEnabled(false);
      form.tbActionParams->setEnabled(false);
      form.kbActionParams->setEnabled(false);
      form.kbActionParams->setChecked(false);

      break;

    case CreateCondition:
      setWindowTitle(tr("Create Bind (Condition)"));

      connLineEdit->setEnabled(false);

      form.tbLinkParams->setEnabled(false);
      form.kbLinkParams->setEnabled(false);
      form.kbLinkParams->setChecked(false);

      form.cbCondition->setEnabled(true);
      form.tbConditionParams->setEnabled(true);
      form.kbConditionParams->setEnabled(true);
      form.kbConditionParams->setChecked(false);

      form.cbAction->setEnabled(false);
      form.tbActionParams->setEnabled(false);
      form.kbActionParams->setEnabled(false);
      form.kbActionParams->setChecked(false);

      break;

    case EditCondition:
      setWindowTitle(tr("Edit Bind (Condition)"));

      connLineEdit->setEnabled(false);

      form.tbLinkParams->setEnabled(false);
      form.kbLinkParams->setEnabled(false);
      form.kbLinkParams->setChecked(false);

      form.cbCondition->setEnabled(true);
      form.tbConditionParams->setEnabled(true);
      form.kbConditionParams->setEnabled(true);
      form.kbConditionParams->setChecked(true);

      form.cbAction->setEnabled(false);
      form.tbActionParams->setEnabled(false);
      form.kbActionParams->setEnabled(false);
      form.kbActionParams->setChecked(false);

      break;

    case CreateAction:
      setWindowTitle(tr("Create Bind (Action)"));

      connLineEdit->setEnabled(false);

      form.tbLinkParams->setEnabled(false);
      form.kbLinkParams->setEnabled(false);
      form.kbLinkParams->setChecked(false);

      form.cbCondition->setEnabled(false);
      form.tbConditionParams->setEnabled(false);
      form.kbConditionParams->setEnabled(false);
      form.kbConditionParams->setChecked(false);

      form.cbAction->setEnabled(true);
      form.tbActionParams->setEnabled(true);
      form.kbActionParams->setEnabled(true);
      form.kbActionParams->setChecked(true);

      break;

    case EditAction:
      setWindowTitle(tr("Edit Bind (Action)"));

      connLineEdit->setEnabled(false);

      form.tbLinkParams->setEnabled(false);
      form.kbLinkParams->setEnabled(false);
      form.kbLinkParams->setChecked(false);

      form.cbCondition->setEnabled(false);
      form.tbConditionParams->setEnabled(false);
      form.kbConditionParams->setEnabled(false);
      form.kbConditionParams->setChecked(false);

      form.cbAction->setEnabled(true);
      form.tbActionParams->setEnabled(true);
      form.kbActionParams->setEnabled(true);
      form.kbActionParams->setChecked(true);

      break;

    default:
      break;
  }

  if (!connName.isEmpty() && _conditions.contains(connName))
  {
    connLineEdit->setText(connName);

    if (!condName.isEmpty() && _conditions.value(connName).contains(condName))
    {
      int index = form.cbCondition->findText(condName);
      form.cbCondition->setCurrentIndex(index);
    }

    if (!actionName.isEmpty() && _actions.value(connName).contains(actionName))
    {
      int index = form.cbCondition->findText(actionName);
      form.cbAction->setCurrentIndex(index);
    }
  }

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

void StructuralLinkDialog::updateForm(QString conn)
{
  form.cbCondition->clear();

  if (_currentMode == CreateCondition ||
      _currentMode == EditCondition ||
      _currentMode == CreateLink)
  {
    foreach (QString b, _conditions.value(conn))
    {
      QString icon = ":/icon/bind-unknow-condition";

      if (StructuralUtil::isCondition(b))
          icon = ":/icon/bind-"+b.toLower();

      if (icon.contains("attribution"))
        icon = ":/icon/bind-unknow-condition";

      form.cbCondition->addItem(QIcon(icon), b);
    }
  }

  if (!form.cbCondition->count())
  {
    form.cbCondition->addItem(tr("Not available"));
  }

  form.cbAction->clear();

  if (_currentMode == CreateAction ||
      _currentMode == EditAction ||
      _currentMode == CreateLink)
  {
    foreach (QString b, _actions.value(conn))
    {
      QString icon = ":/icon/bind-unknow-action";

      if (StructuralUtil::isAction(b))
          icon = ":/icon/bind-"+b.toLower();

      if (icon.contains("attribution"))
        icon = ":/icon/bind-unknow-action";

      form.cbAction->addItem(QIcon(icon),b);
    }
  }

  if (!form.cbAction->count())
    form.cbAction->addItem(tr("Not available"));

  int ncol = 2;
  int nrow = _params.value(conn).size();

  form.tbLinkParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  form.tbConditionParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  form.tbActionParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  form.tbLinkParams->verticalHeader()->hide();
  form.tbConditionParams->verticalHeader()->hide();
  form.tbActionParams->verticalHeader()->hide();

  QStandardItemModel* mdLinkParams = new QStandardItemModel(nrow, ncol);

  mdLinkParams->setHorizontalHeaderItem(0, new QStandardItem("Name"));
  mdLinkParams->setHorizontalHeaderItem(1, new QStandardItem("Value"));

  if (form.tbLinkParams->model() != NULL)
  {
    QAbstractItemModel* m = form.tbLinkParams->model();
    delete m;
  }

  form.tbLinkParams->setModel(mdLinkParams);

  QStandardItemModel* mdConditionParams = new QStandardItemModel(nrow, ncol);

  mdConditionParams->setHorizontalHeaderItem(0, new QStandardItem("Name"));
  mdConditionParams->setHorizontalHeaderItem(1, new QStandardItem("Value"));

  if (form.tbConditionParams->model() != NULL)
  {
    QAbstractItemModel* m = form.tbConditionParams->model();
    delete m;
  }

  form.tbConditionParams->setModel(mdConditionParams);

  QStandardItemModel* mdActionParams = new QStandardItemModel(nrow, ncol);

  mdActionParams->setHorizontalHeaderItem(0, new QStandardItem("Name"));
  mdActionParams->setHorizontalHeaderItem(1, new QStandardItem("Value"));

  if (form.tbActionParams->model() != NULL)
  {
    QAbstractItemModel* m = form.tbActionParams->model();
    delete m;
  }

  form.tbActionParams->setModel(mdActionParams);

  int i = 0;

  foreach(QString name, _params.value(conn))
  {
    QStandardItem* linkItem = new QStandardItem(name);
    linkItem->setEditable(false);

    QStandardItem* conditionItem = new QStandardItem(name);
    conditionItem->setEditable(false);

    QStandardItem* actionItem = new QStandardItem(name);
    actionItem->setEditable(false);

    mdLinkParams->setItem(i, 0, linkItem);
    mdLinkParams->setItem(i, 1, new QStandardItem());

    mdConditionParams->setItem(i, 0, conditionItem);
    mdConditionParams->setItem(i, 1, new QStandardItem());

    mdActionParams->setItem(i, 0, actionItem);
    mdActionParams->setItem(i, 1, new QStandardItem());

    ++i;
  }
}

void StructuralLinkDialog::moveEvent(QMoveEvent *event)
{
  QDialog::moveEvent(event);

  connLineEdit->updateListPos();
}

void StructuralLinkDialog::resizeEvent(QResizeEvent *event)
{
  QDialog::resizeEvent(event);

  connLineEdit->updateListSize();
}

bool StructuralLinkDialog::event(QEvent *evt)
{
  int ret = QDialog::event(evt);

  if(evt->type() == QEvent::Show)
  {
    QTimer::singleShot(0, this->connLineEdit, SLOT(setFocus()));
  }

  return ret;
}

QString StructuralLinkDialog::getCurrentConnector()
{
  return this->connLineEdit->text();
}

void StructuralLinkDialog::changeLinkParamState(int state)
{
  if (!state)
    form.tbLinkParams->hide();
  else
    form.tbLinkParams->show();
}

void StructuralLinkDialog::changeConditionParamState(int state)
{
  if (!state)
    form.tbConditionParams->hide();
  else
    form.tbConditionParams->show();
}

void StructuralLinkDialog::changeActionParamState(int state)
{
  if (!state)
    form.tbActionParams->hide();
  else
    form.tbActionParams->show();
}

QMap<QString, QString> StructuralLinkDialog::getLinkParams()
{
  return getParams(form.tbLinkParams);
}

QMap<QString, QString> StructuralLinkDialog::getConditionParams()
{
  return getParams(form.tbConditionParams);
}

QMap<QString, QString> StructuralLinkDialog::getActionParams()
{
  return getParams(form.tbActionParams);
}

QMap<QString, QString> StructuralLinkDialog::getParams(QTableView* table)
{
  QMap<QString, QString> p;

  QAbstractItemModel* m = table->model();

  int nrow = m->rowCount();

  for (int i=0; i<nrow; ++i)
    p[m->data(m->index(i,0)).toString()] = m->data(m->index(i,1)).toString();

  return p;
}

void StructuralLinkDialog::updateCurrentLinkParam(const QMap<QString, QString> &params)
{
  updateCurrentParams(form.tbLinkParams, params);
}

void StructuralLinkDialog::updateCurrentConditionParam(const QMap<QString, QString> &params)
{
  updateCurrentParams(form.tbConditionParams, params);
}

void StructuralLinkDialog::updateCurrentActionParam(const QMap<QString, QString> &params)
{
  updateCurrentParams(form.tbActionParams, params);
}

void StructuralLinkDialog::updateCurrentParams(QTableView* table, const QMap<QString, QString> &params)
{
  QAbstractItemModel* m = table->model();

  if (m != NULL)
  {
    int nrow = m->rowCount();

    for (int i=0; i<nrow; ++i)
    {
      QString key = m->data(m->index(i,0)).toString();

      if (params.contains(key))
      {
        m->setData(m->index(i,1),params.value(key));
      }
    }
  }
}
