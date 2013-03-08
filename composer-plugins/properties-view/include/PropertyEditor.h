/*
 * Copyright 2011 TeleMidia/PUC-Rio.
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
#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <QObject>
#include <QString>
#include <QWidget>

#include "ui_PropertyEditorWidget.h"
#include "QLineEditFilter.h"

/*!
 \brief PropertyEditor is a Widget that allows to edit individual properties
            (also called attributes) of any entity.

*/
class PropertyEditor : public QWidget
{
  Q_OBJECT

private:
  Ui::PropertyEditorWidget *ui; /*!< TODO */
  QMap <QString, int> propertyToLine; /*!< TODO */
  QMap <QString, QString> propertyToValue; /*!< TODO */

  bool internalPropertyChange;

  QString currentTagname, currentName;
  QString currentFilterString;

public:
  /*!
   * \brief Constructor.
   *
   * \param parent The QObject parent.
   */
  explicit PropertyEditor(QWidget *parent=0);

  /*!
   * \brief Destructor.
   */
  virtual ~PropertyEditor();
  /*!
   * \brief Set the current tagname and update the available properties
   *  according to that tagname.
   *
   * The available properties are loaded from the configuration file of the
   * current language.
   *
   * \param tagname The current tagname.
   */
  void setTagname(QString tagname, QString name);

  /*!
   * \brief Get the current tagname.
   */
  QString getTagname() { return this->currentTagname; }

  /*!
   * \brief Set the current name (id, name, ...) of the selected entity.
   */
  void setCurrentName(QString name);

  /*!
   * \brief Set the current error message (and change the color of the text) if
   *  the errorMessage is not empty.
   */
  void setErrorMessage(QString errorMessage);

  /*!
   * \brief Get the current name.
   */
  QString getCurrentName(){ return this->currentName; }

  /*!
   * \brief Set a value of an attribute.
   *
   * \param property Which property must be changed.
   * \param value The new Value of the property.
   */
  void setAttributeValue(QString property, QString value);

  /*!
   * \brief
   */
  static bool isURL(const QString &tagname, const QString &attr);

  QSize sizeHint() const { return QSize(250, 400); }

private slots:
  /*!
   * \brief This SLOT must be called when the user changes the value of an item.
   *
   * The changes will then be sent to NCL Composer core.
   */
  void updateWithItemChanges(QTableWidgetItem *item);

  /*!
   * \brief This SLOT allows filter the current properties that are been showed.
   */
  void filterProperties(const QString&);

signals:
  void propertyChanged(QString property, QString value);
};

#endif // PROPERTYEDITOR_H
