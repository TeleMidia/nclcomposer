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
          SLOT(updateCompleter(const QString &)));

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

void CompleteLineEdit::updateCompleter(const QString &text)
{
  setCompleter(text);
  _listView->show();
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
}

void CompleteLineEdit::completeText(const QModelIndex &index)
{
  QString text = index.data().toString();
  setText(text);
  _listView->hide();
  nextInFocusChain()->setFocus();
}

StructuralLinkDialog::StructuralLinkDialog(QWidget* parent)
  : QDialog(parent), _aligned(false)
{
  _form.setupUi(this);

  _form.tbLinkParams->hide();
  _form.tbConditionParams->hide();
  _form.tbActionParams->hide();

  _form.lyDialog->setSizeConstraint(QLayout::SetMinAndMaxSize);
  _form.lyLink->setSizeConstraint(QLayout::SetMinAndMaxSize);
  _form.lyBinds->setSizeConstraint(QLayout::SetMinAndMaxSize);

  _form.lyLink->addWidget((_search = new CompleteLineEdit(QStringList(), this)), 0, 1);

  _mode = CreateLink;

  connect(_search,SIGNAL(textChanged(QString)), SLOT(changeContent(QString)));

  connect(_form.kbLinkParams, SIGNAL(stateChanged(int)),this, SLOT(switchConnectorParam(int)));
  connect(_form.kbConditionParams, SIGNAL(stateChanged(int)), this, SLOT(switchConditionParam(int)));
  connect(_form.kbActionParams, SIGNAL(stateChanged(int)), this, SLOT(switchActionParam(int)));

  setMode();
}

StructuralLinkDialog::~StructuralLinkDialog()
{
  // TODO
}

QString StructuralLinkDialog::getConnector()
{
  return _search->text();
}

void StructuralLinkDialog::setConnector(const QString &connector)
{
  _search->setText(connector);
}

QMap<QString, QString> StructuralLinkDialog::getConnectorParams()
{
  return getParams(_form.tbLinkParams);
}

void StructuralLinkDialog::setConnectorParams(const QMap<QString, QString> &params)
{
  setParams(_form.tbLinkParams, params);
}

QString StructuralLinkDialog::getCondition()
{
  return _form.cbCondition->currentText();
}

void StructuralLinkDialog::setCondition(const QString &condition)
{
  _form.cbCondition->setCurrentText(condition);
}

QMap<QString, QString> StructuralLinkDialog::getConditionParams()
{
  return getParams(_form.tbConditionParams);
}

void StructuralLinkDialog::setConditionParams(const QMap<QString, QString> &params)
{
  setParams(_form.tbConditionParams, params);
}

QString StructuralLinkDialog::getConditionInterface()
{
  return getInterface(_form.cbConditionInterface);
}

void StructuralLinkDialog::setConditionInterface(const QString &name)
{
  setInterface(_form.cbConditionInterface, name);
}

QMap<QString, QString> StructuralLinkDialog::getConditonInterfaces()
{
  return getInterfaces(_form.cbConditionInterface);
}

void StructuralLinkDialog::addConditionInterface(const QString &uid, const QString &name, const QString &icon)
{
  addInterface(_form.cbConditionInterface, uid, name, icon);
}

void StructuralLinkDialog::removeConditionInterface(const QString& name)
{
  removeInterface(_form.cbConditionInterface, name);
}

QString StructuralLinkDialog::getAction()
{
  return _form.cbAction->currentText();
}

void StructuralLinkDialog::setAction(const QString &action)
{
  _form.cbAction->setCurrentText(action);
}

QMap<QString, QString> StructuralLinkDialog::getActionParams()
{
  return getParams(_form.tbActionParams);
}

void StructuralLinkDialog::setActionParams(const QMap<QString, QString> &params)
{
  setParams(_form.tbActionParams, params);
}

QString StructuralLinkDialog::getActionInterface()
{
  return getInterface(_form.cbActionInterface);
}

void StructuralLinkDialog::setActionInterface(const QString &name)
{
  setInterface(_form.cbActionInterface, name);
}

QMap<QString, QString> StructuralLinkDialog::getActionInterfaces()
{
  return getInterfaces(_form.cbActionInterface);
}

void StructuralLinkDialog::addActionInterface(const QString &uid, const QString &name, const QString &icon)
{
  addInterface(_form.cbActionInterface, uid, name, icon);
}

void StructuralLinkDialog::removeActionInterface(const QString& name)
{
  removeInterface(_form.cbActionInterface, name);
}

void StructuralLinkDialog::setBase(const QMap<QString, QVector<QString> > &conditions,
                                   const QMap<QString, QVector<QString> > &actions,
                                   const QMap<QString, QVector<QString> > &params)
{
  // Cleaning data...
  _form.cbConditionInterface->clear();
  _form.cbConditionInterface->addItem("");

  _form.cbActionInterface->clear();
  _form.cbActionInterface->addItem("");

  // Setting data...
  _conditions = conditions;
  _actions = actions;
  _params = params;
}

void StructuralLinkDialog::setMode(const QString &connector,
                                   const QString &condition,
                                   const QString &action,
                                   Mode mode)
{
  _mode = mode;

  _search->clear();
  _search->setStringList(_conditions.keys());

  // Setting mode...
  switch (_mode)
  {
    case CreateLink:
      setWindowTitle(tr("Create Link"));

      _search->setEnabled(true);

      _form.tbLinkParams->setEnabled(true);
      _form.kbLinkParams->setEnabled(true);
      _form.kbLinkParams->setChecked(false);

      _form.cbCondition->setEnabled(true);
      _form.cbConditionInterface->setEnabled(true);
      _form.tbConditionParams->setEnabled(true);
      _form.kbConditionParams->setEnabled(true);
      _form.kbConditionParams->setChecked(false);

      _form.cbAction->setEnabled(true);
      _form.cbActionInterface->setEnabled(true);
      _form.tbActionParams->setEnabled(true);
      _form.kbActionParams->setEnabled(true);
      _form.kbActionParams->setChecked(false);

      break;

    case EditLink:
      setWindowTitle(tr("Edit Link"));

      _search->setEnabled(false);

      _form.tbLinkParams->setEnabled(true);
      _form.kbLinkParams->setEnabled(true);
      _form.kbLinkParams->setChecked(true);

      _form.cbCondition->setEnabled(false);
      _form.cbConditionInterface->setEnabled(false);
      _form.tbConditionParams->setEnabled(false);
      _form.kbConditionParams->setEnabled(false);
      _form.kbConditionParams->setChecked(false);

      _form.cbAction->setEnabled(false);
      _form.cbActionInterface->setEnabled(false);
      _form.tbActionParams->setEnabled(false);
      _form.kbActionParams->setEnabled(false);
      _form.kbActionParams->setChecked(false);

      break;

    case CreateCondition:
      setWindowTitle(tr("Create Bind (Condition)"));

      _search->setEnabled(false);

      _form.tbLinkParams->setEnabled(false);
      _form.kbLinkParams->setEnabled(false);
      _form.kbLinkParams->setChecked(false);

      _form.cbCondition->setEnabled(true);
      _form.cbConditionInterface->setEnabled(true);
      _form.tbConditionParams->setEnabled(true);
      _form.kbConditionParams->setEnabled(true);
      _form.kbConditionParams->setChecked(false);

      _form.cbAction->setEnabled(false);
      _form.cbActionInterface->setEnabled(false);
      _form.tbActionParams->setEnabled(false);
      _form.kbActionParams->setEnabled(false);
      _form.kbActionParams->setChecked(false);

      break;

    case EditCondition:
      setWindowTitle(tr("Edit Bind (Condition)"));

      _search->setEnabled(false);

      _form.tbLinkParams->setEnabled(false);
      _form.kbLinkParams->setEnabled(false);
      _form.kbLinkParams->setChecked(false);

      _form.cbCondition->setEnabled(true);
      _form.cbConditionInterface->setEnabled(true);
      _form.tbConditionParams->setEnabled(true);
      _form.kbConditionParams->setEnabled(true);
      _form.kbConditionParams->setChecked(false);

      _form.cbAction->setEnabled(false);
      _form.cbActionInterface->setEnabled(false);
      _form.tbActionParams->setEnabled(false);
      _form.kbActionParams->setEnabled(false);
      _form.kbActionParams->setChecked(false);

      break;

    case CreateAction:
      setWindowTitle(tr("Create Bind (Action)"));

      _search->setEnabled(false);

      _form.tbLinkParams->setEnabled(false);
      _form.kbLinkParams->setEnabled(false);
      _form.kbLinkParams->setChecked(false);

      _form.cbCondition->setEnabled(false);
      _form.cbConditionInterface->setEnabled(false);
      _form.tbConditionParams->setEnabled(false);
      _form.kbConditionParams->setEnabled(false);
      _form.kbConditionParams->setChecked(false);

      _form.cbAction->setEnabled(true);
      _form.cbActionInterface->setEnabled(true);
      _form.tbActionParams->setEnabled(true);
      _form.kbActionParams->setEnabled(true);
      _form.kbActionParams->setChecked(true);

      break;

    case EditAction:
      setWindowTitle(tr("Edit Bind (Action)"));

      _search->setEnabled(false);

      _form.tbLinkParams->setEnabled(false);
      _form.kbLinkParams->setEnabled(false);
      _form.kbLinkParams->setChecked(false);

      _form.cbCondition->setEnabled(false);
      _form.cbConditionInterface->setEnabled(false);
      _form.tbConditionParams->setEnabled(false);
      _form.kbConditionParams->setEnabled(false);
      _form.kbConditionParams->setChecked(false);

      _form.cbAction->setEnabled(true);
      _form.cbActionInterface->setEnabled(true);
      _form.tbActionParams->setEnabled(true);
      _form.kbActionParams->setEnabled(true);
      _form.kbActionParams->setChecked(true);

      break;

    default:
      break;
  }

  // Setting content...
  if (!connector.isEmpty() &&
      _conditions.contains(connector))
  {
    // Setting 'Connector'...
    _search->setText(connector);

    // Setting 'Condition'...
    if (!condition.isEmpty() &&
        _conditions.value(connector).contains(condition))
    {
      int index = _form.cbCondition->findText(condition);
      _form.cbCondition->setCurrentIndex(index);
    }

    // Setting 'Action'...
    if (!action.isEmpty() &&
        _actions.value(connector).contains(action))
    {
      int index = _form.cbCondition->findText(action);
      _form.cbAction->setCurrentIndex(index);
    }
  }

  // Adjusting position...
  if(!_aligned)
  {
    setMinimumWidth(350);
    updateGeometry();

    QRect screenGeometry;

    if (this->parentWidget())
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

    _aligned = true;
  }
}

void StructuralLinkDialog::changeContent(const QString &connector)
{
  // Updating 'Condition'...
  _form.cbCondition->clear();

  if (_mode == CreateCondition ||
      _mode == EditCondition ||
      _mode == CreateLink)
  {
    foreach (const QString &condition, _conditions.value(connector))
    {
      QString icon = ":/icon/bind-unknow-condition";

      if (StructuralUtil::isCondition(condition))
          icon = ":/icon/bind-"+condition.toLower();

      if (icon.contains("attribution"))
        icon = ":/icon/bind-unknow-condition";

      _form.cbCondition->addItem(QIcon(icon), condition);
    }
  }

  if (!_form.cbCondition->count())
  {
    _form.cbCondition->addItem(tr("Not available"));
  }

  // Updating 'Action'...
  _form.cbAction->clear();

  if (_mode == CreateAction ||
      _mode == EditAction ||
      _mode == CreateLink)
  {
    foreach (const QString &action, _actions.value(connector))
    {
      QString icon = ":/icon/bind-unknow-action";

      if (StructuralUtil::isAction(action))
          icon = ":/icon/bind-"+action.toLower();

      if (icon.contains("attribution"))
        icon = ":/icon/bind-unknow-action";

      _form.cbAction->addItem(QIcon(icon),action);
    }
  }

  if (!_form.cbAction->count())
  {
    _form.cbAction->addItem(tr("Not available"));
  }

  int ncol = 2;
  int nrow = _params.value(connector).size();

  _form.tbLinkParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  _form.tbConditionParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  _form.tbActionParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  _form.tbLinkParams->verticalHeader()->hide();
  _form.tbConditionParams->verticalHeader()->hide();
  _form.tbActionParams->verticalHeader()->hide();

  QStandardItemModel* mdLinkParams = new QStandardItemModel(nrow, ncol);

  mdLinkParams->setHorizontalHeaderItem(0, new QStandardItem("Name"));
  mdLinkParams->setHorizontalHeaderItem(1, new QStandardItem("Value"));

  if (_form.tbLinkParams->model() != NULL)
  {
    QAbstractItemModel* m = _form.tbLinkParams->model();
    delete m;
  }

  _form.tbLinkParams->setModel(mdLinkParams);

  QStandardItemModel* mdConditionParams = new QStandardItemModel(nrow, ncol);

  mdConditionParams->setHorizontalHeaderItem(0, new QStandardItem("Name"));
  mdConditionParams->setHorizontalHeaderItem(1, new QStandardItem("Value"));

  if (_form.tbConditionParams->model() != NULL)
  {
    QAbstractItemModel* m = _form.tbConditionParams->model();
    delete m;
  }

  _form.tbConditionParams->setModel(mdConditionParams);

  QStandardItemModel* mdActionParams = new QStandardItemModel(nrow, ncol);

  mdActionParams->setHorizontalHeaderItem(0, new QStandardItem("Name"));
  mdActionParams->setHorizontalHeaderItem(1, new QStandardItem("Value"));

  if (_form.tbActionParams->model() != NULL)
  {
    QAbstractItemModel* m = _form.tbActionParams->model();
    delete m;
  }

  _form.tbActionParams->setModel(mdActionParams);

  int i = 0;

  foreach (const QString &name, _params.value(connector))
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

void StructuralLinkDialog::switchConnectorParam(int state)
{
  if (!state)
    _form.tbLinkParams->hide();
  else
    _form.tbLinkParams->show();
}

void StructuralLinkDialog::switchConditionParam(int state)
{
  if (!state)
    _form.tbConditionParams->hide();
  else
    _form.tbConditionParams->show();
}

void StructuralLinkDialog::switchActionParam(int state)
{
  if (!state)
    _form.tbActionParams->hide();
  else
    _form.tbActionParams->show();
}

void StructuralLinkDialog::moveEvent(QMoveEvent *event)
{
  QDialog::moveEvent(event);

  _search->updateListPos();
}

void StructuralLinkDialog::resizeEvent(QResizeEvent *event)
{
  QDialog::resizeEvent(event);

  _search->updateListSize();
}

bool StructuralLinkDialog::event(QEvent *event)
{
  int ret = QDialog::event(event);

  if(event->type() == QEvent::Show)
  {
    QTimer::singleShot(0, this->_search, SLOT(setFocus()));
  }

  return ret;
}

QMap<QString, QString> StructuralLinkDialog::getParams(QTableView* widget)
{
  QMap<QString, QString> p;

  QAbstractItemModel* m = widget->model();

  int nrow = m->rowCount();

  for (int i=0; i<nrow; ++i)
    p[m->data(m->index(i,0)).toString()] = m->data(m->index(i,1)).toString();

  return p;
}

QString StructuralLinkDialog::getInterface(const QComboBox* widget)
{
  return widget->currentData().toString();
}

void StructuralLinkDialog::setInterface(QComboBox* widget, const QString &name)
{
  widget->setCurrentText(name);
}

QMap<QString, QString> StructuralLinkDialog::getInterfaces(const QComboBox* widget)
{
  QMap<QString, QString> interfaces;

  for (auto i = 0; i < widget->count(); ++i)
  {
    interfaces[widget->itemData(i).toString()] = widget->itemText(i);
  }

  return interfaces;
}

void StructuralLinkDialog::addInterface(QComboBox* widget, const QString &uid, const QString &name, const QString &icon)
{
  widget->addItem(QIcon(icon), name, uid);
}

void StructuralLinkDialog::removeInterface(QComboBox* widget, const QString &name)
{
  int index = widget->findText(name);

  if (index >= 0)
    widget->removeItem(index);
}

void StructuralLinkDialog::setParams(QTableView* widget, const QMap<QString, QString> &params)
{
  QAbstractItemModel* m = widget->model();

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
