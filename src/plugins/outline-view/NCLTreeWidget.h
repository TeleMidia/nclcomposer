/*
 * Copyright 2011-2012 TeleMidia/PUC-Rio.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef NCLTREEWIDGET_H
#define NCLTREEWIDGET_H

#include <map>
using namespace std;

#include <QAction>
#include <QDebug>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMenu>
#include <QShortcut>
#include <QTreeWidget>
#include <QWheelEvent>
#include <QtXml>

#define KEEP_ELEMENTS_ORDER 1

#ifdef KEEP_ELEMENTS_ORDER
#include <QVector>
#endif

#include "NCLParser.h"

/*!
 * \brief Handles a NCL document as a tree. OutlineView plugin is just
 *  a wrapper over this class to send and receive messsages from
 *  Composer Core.
 *
 */
class NCLTreeWidget : public QTreeWidget
{
  Q_OBJECT

public:
  /*!
   * \brief Constructs a ncl tree widget with the given parent.
   *
   * \param parent The
   */
  explicit NCLTreeWidget (QWidget *parent = 0);

  /*!
   * \brief Destroys the tree widget and all its items.
   */
  virtual ~NCLTreeWidget ();

  /*!
   * \brief Sets the default font of the TreeWidget.
   */
  void setDefaultFont (const QFont &_defaultFont);

  /*!
   * Reimplementation of mouseMoveEvent.
   */
  void mouseMoveEvent (QMouseEvent *event);

  QSize
  sizeHint () const
  {
    return QSize (250, 400);
  }

public slots:
  /**
     * \brief Updates the tree representation based on the XML text parameter.
     *
     * \param text The source of the document should be represented as a tree.
     * \return bool Return false if the
    */
  bool updateFromText (QString text);

  /*!
     * \brief Add an element as child of father and return this element
     *
     * \param father
     * \param pos
     * \param tagname
     * \param id
     * \param attrs
     * \param line_in_text
     * \param column_in_text
     *
     * \return QTreeWidgetItem *
     */
  QTreeWidgetItem *addElement (QTreeWidgetItem *father, int pos,
                               QString tagname, QString id,
                               QMap<QString, QString> &attrs,
                               int line_in_text = -1, int column_in_text = -1);

  /*!
   * \brief Returns a QTreeWidgetItem with identifier equal to itemId.
   *
   * \param itemId The identifier of the element.
   *
   * \return QTreeWidgetItem* The element with identifier itemId.
   */
  QTreeWidgetItem *getItemById (QString itemId);
  /**
     * \brief Remove the element
     *
     * \param itemId
    */
  void removeItem (QString itemId);
  /**
     * \brief
     *
     * \param item
     * \param tagname
     * \param attrs
    */
  void updateItem (QTreeWidgetItem *item, QString tagname,
                   QMap<QString, QString> &attrs);

  /*!
   * \brief
   *
   * \param message
   * \param filename
   * \param line
   * \param column
   * \param severity
   */
  void errorNotification (QString message, QString filename, int line,
                          int column, int severity);
  void expandAll ();

  /*!
   * \brief Reset font to its default family and size.
   */
  void resetFont ();
  /*!
   * \brief Increases the font and icon size.
   */
  void zoomIn ();
  /*!
   * \brief Decreases the font and icon sizes.
   */
  void zoomOut ();
  /*!
   * \brief Reset font and icon sizes to defaults.
   */
  void resetZoom ();

signals:
  /*!
   * \brief
   *
   * \param QString
   * \param QString
   * \param
   * \param bool
   */
  void elementAddedByUser (QString, QString, QMap<QString, QString> &);
  /*!
   * \brief
   *
   * \param QString
   */
  void elementRemovedByUser (QString);
  /*!
   * \brief
   *
   * \param message
   * \param filename
   * \param line
   * \param column
   * \param severity
   */
  void parserErrorNotify (QString message, QString filename, int line,
                          int column, int severity);

  void userAskedToOpenWithDefaultSystemEditor (QString itemId);

private:
#ifdef KEEP_ELEMENTS_ORDER
  static QVector<QString> _entitiesOrder;
#endif

  QAction *_insertNodeAct;
  QAction *_removeNodeAct;
  QAction *_expandAllAct;
  QAction *_openWithDefaultEditorAct;
  QMenu *_elementMenu;

  QShortcut *_shortcutZoomOut;
  QShortcut *_shortcutZoomIn;
  bool _isExpandedAll;

  int _fontSize;
  QFont _defaultFont;

  QMap<QString, QString> _entitiesIcons;
  QMap<QString, QString> _mediaIcons;
  QMap<QString, QString> _extIcons;
  QMap<QString, QString> _bindIcons;

  /*!
   * \brief Creates the QActions
   */
  void createActions ();
  /*!
   * \brief Creates the menus and bind to previous created QActions.
   */
  void createMenus ();

  /* User events */
  /*!
   * \brief Handles wheel event (mainly to perform zoom actions). This function
   *          is a callback called by Qt.
   *
   * \param event The QWheelEvent.
   */
  void wheelEvent (QWheelEvent *event);
  /*!
   * \brief Handles keypress event. This callback is called internally by Qt.
   *
   * \param event The QKeyEvent.
   */
  void keyPressEvent (QKeyEvent *event);

private slots:
  /*!
   * \brief
   */
  void userAddNewElement ();
  /*!
   * \brief
   */
  void userRemoveElement ();
  /*!
   * \brief
   */
  void decreaseFont ();
  /*!
   * \brief
   */
  void increaseFont ();

  void handleSelectionChanged ();
  void openWithDefaultSystemEditor ();
};

#endif
