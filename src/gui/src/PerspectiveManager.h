/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef PLUGINSLAYOUTMANAGER_H
#define PLUGINSLAYOUTMANAGER_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui{
    class PerpectiveManager;
}

/*!
 \brief Enum that informs what is the current behavior of the
    PerspectiveManager.
*/
enum PERSPEC_BEHAVIOR //PERSPECTIVE BEHAVIOR
{
    PERSPEC_SAVE = 0,
    PERSPEC_LOAD
};

namespace composer {
  namespace gui {
/*!
 * \brief A dialog that allows the user to choose or save one perspective.
 */
class PerspectiveManager: public QDialog
{
  Q_OBJECT

public:
  /*!
   * \brief Constructor.
   *
   * \param parent The parent of this class.
   */
  PerspectiveManager(QWidget *parent = 0);

  /*!
   * \brief Destructor.
   */
  virtual ~PerspectiveManager();

  /*!
   * \brief Get the name of the perspective the user has selected.
   * \return QString The name of the perspective selected.
   */
  QString getSelectedName();

  /*!
   * \brief Get the name of the default perspective the user has selected.
   *
   * \return QString The name of the default perspective selected.
   */
  QString getDefaultPerspective();

  /*!
   * \brief Set the current behavior of the PerpectiveManager. This function
   * must be called before the show event to have effect.
   *
   * \param behavior
   */
  void setBehavior(PERSPEC_BEHAVIOR behavior);

public slots:
  /*!
   * \brief Called when the user clicks OK button
   */
  void accept();

protected:
  /*!
   *  \brief Reimplemented to update the dialog with the information of
   */
  void showEvent(QShowEvent *);

  /*!
   * \brief Removes the perspective with name: name.
   *
   * \param name The name of the perspective to be removed.
   */
  void deletePerspective(QString name);

private:
    Ui::PerpectiveManager *ui; /*!< The instantiation of the qtcreator designer
                                    generated class.*/
    QString selectedName; /*!< The current selected element. */
    QString defaultPerspective;
    PERSPEC_BEHAVIOR behavior; /*!< The current behavior of the
                                    PerspectiveManager */

    bool internalChange;
    void updateContent();

private slots:
  /*!
   * \brief Called when the user select the delete button.
   */
  void deleteSelectedPerspective();

  /*!
   * \brief Called when the user edit an item.
   */
  void itemChanged(QTreeWidgetItem *, int);
};
} } //end namespace
#endif // PLUGINSLAYOUTMANAGER_H
