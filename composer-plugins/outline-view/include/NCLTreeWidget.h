/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef NCLTREEWIDGET_H
#define NCLTREEWIDGET_H

#include <map>
using namespace std;

#include <QDebug>
#include <QtXml>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QTreeWidget>
#include <QShortcut>
#include <QKeyEvent>
#include <QWheelEvent>

#include "NCLParser.h"

/**
 * @brief Handles a NCL document as a tree. OutlineView plugin is just
 *          a wrapper over this class to send and receive messsages from
 *          Composer Core.
 *
 */
class NCLTreeWidget: public QTreeWidget {
    Q_OBJECT

private:
    QAction *insertNodeAct; /**< TODO */
    QAction *removeNodeAct; /**< TODO */
    QAction *expandAllAct; /**< TODO */
    QMenu   *elementMenu; /**< TODO */

    QShortcut *shortcut_zoomout; /**< TODO */
    QShortcut *shortcut_zoomin; /**< TODO */
    bool isExpandedAll;

    /**
     * @brief Creates the QActions
     *
    */
    void createActions();
    /**
     * @brief Creates the menus and bind to previous created QActions.
     *
    */
    void createMenus();

    /* User events */
    /**
     * @brief Handles wheel event (mainly to perform zoom actions). This function
     *          is a callback called by Qt.
     *
     * @param event The QWheelEvent.
    */
    void wheelEvent(QWheelEvent * event);
    /**
     * @brief Handles keypress event. This callback is called internally by Qt.
     *
     * @param event The QKeyEvent.
    */
    void keyPressEvent(QKeyEvent *event);

public:
    /**
     * @brief Constructs a ncl tree widget with the given parent.
     *
     * @param parent The
    */
    NCLTreeWidget(QWidget *parent);
    /**
     * @brief Destroys the tree widget and all its items.
     *
    */
    virtual ~NCLTreeWidget();

    /**
     * @brief Updates the tree representation based on the XML text parameter.
     *
     * @param text The source of the document should be represented as a tree.
     * @return bool Return false if the
    */
    bool updateFromText(QString text);

    /**
     * @brief Add an element as child of father and return this element
     *
     * @param father
     * @param pos
     * @param tagname
     * @param id
     * @param attrs
     * @param line_in_text
     * @param column_in_text
     * @return QTreeWidgetItem *
    */
    QTreeWidgetItem* addElement ( QTreeWidgetItem *father,
                                  int pos,
                                  QString tagname,
                                  QString id,
                                  QMap <QString, QString> &attrs,
                                  int line_in_text = -1,
                                  int column_in_text = -1);

    /**
     * @brief Returns a QTreeWidgetItem with identifier equal to itemId.
     *
     * @param itemId The identifier of the element.
     * @return QTreeWidgetItem* The element with identifier itemId.
    */
    QTreeWidgetItem* getItemById(QString itemId);
    /**
     * @brief Remove the element
     *
     * @param itemId
    */
    void removeItem(QString itemId);
    /**
     * @brief
     *
     * @param item
     * @param tagname
     * @param attrs
    */
    void updateItem(QTreeWidgetItem* item, QString tagname,
                    QMap <QString, QString> &attrs);

public slots:
    /**
     * @brief
     *
     * @param message
     * @param filename
     * @param line
     * @param column
     * @param severity
    */
    void errorNotification( QString message,
                            QString filename,
                            int line,
                            int column,
                            int severity);
    void expandAll();

private slots:
    /**
     * @brief
     *
    */
    void userAddNewElement();
    /**
     * @brief
     *
    */
    void userRemoveElement();
    /**
     * @brief
     *
    */
    void DecreaseFont();
    /**
     * @brief
     *
    */
    void IncreaseFont();

signals:
    /**
     * @brief
     *
     * @param QString
     * @param QString
     * @param
     * @param bool
    */
    void elementAddedByUser(QString, QString, QMap<QString,QString>&, bool);
    /**
     * @brief
     *
     * @param QString
    */
    void elementRemovedByUser (QString);
    /**
     * @brief
     *
     * @param message
     * @param filename
     * @param line
     * @param column
     * @param severity
    */
    void parserErrorNotify( QString message,
                            QString filename,
                            int line,
                            int column,
                            int severity);

};

#endif
