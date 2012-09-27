#ifndef QNSTGRAPHICSLINKDIALOG_H
#define QNSTGRAPHICSLINKDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QCompleter>
#include <QStringListModel>
#include <QSortFilterProxyModel>

#include  "qnstconncetor.h"

#include "ui_QnstGraphicsLinkForm.h"

#include <QtGui/QLineEdit>
#include <QStringList>

class QListView;
class QStringListModel;
class QModelIndex;

class CompleteLineEdit : public QLineEdit {
    Q_OBJECT
public:
    CompleteLineEdit(QStringList words, QWidget *parent = 0);
    void setStringList(const QStringList &words);

public slots:
    void setCompleter(const QString &text);
    void completeText(const QModelIndex &index);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
    virtual bool eventFilter(QObject *object, QEvent *event);

private:
    QStringList words;
    QListView *listView;
    QStringListModel *model;
};

class MySortFilterProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  MySortFilterProxyModel(QObject *parent = 0) :
    QSortFilterProxyModel(parent)
  {

  }

protected:
  virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
  {
    qDebug() << "aq";
    // QString key = d->model->data(source_index, d->filter_role).toString();
    // qDebug(key);

    return true;
  }
};

class MyCompleter : public QCompleter
{
    Q_OBJECT

public:
    inline MyCompleter(const QStringList& words, QObject * parent) :
            QCompleter(parent), m_list(words), m_model()
    {
        setModel(&m_model);
        setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        connect(completionModel(), SIGNAL(layoutChanged()), SLOT(completionModelChanged()));
    }


    inline void update(QString word)
    {
      // Do any filtering you like.
      // Here we just include all items that contain word.
      QStringList filtered;
      foreach (QString str, m_list)
      {
        if(str.contains(word))
          filtered << str;
      }

      m_model.setStringList(filtered);
      m_word = word;
      complete();
    }

    inline QString word()
    {
        return m_word;
    }

private:
    QStringList m_list;
    QStringListModel m_model;
    QString m_word;
};

class QnstGraphicsLinkDialog : public QDialog
{
    Q_OBJECT

public:
    QnstGraphicsLinkDialog(QWidget* parent = 0);

    ~QnstGraphicsLinkDialog();

    void init(QMap<QString, QnstConncetor*> connectors);

protected slots:
    void adjustBinds(QString conn);

public:
    //TODO: This shouldn't be public
    Ui::QnstGraphicsLinkForm form;

    QMap<QString, QnstConncetor*> connectors;
    QString getCurrentConnector();

private:
    bool firstTime, changeModel;
    CompleteLineEdit *connLineEdit;
};

#endif // QNSTGRAPHICSLINKDIALOG_H
