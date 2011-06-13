#ifndef PLUGINSLAYOUTMANAGER_H
#define PLUGINSLAYOUTMANAGER_H

#include <QDialog>

namespace Ui{
    class PerpectiveManager;
};

/*!
 \brief Enum that informs what is the current behavior of the
    PerspectiveManager.
*/
enum PERSPEC_BEHAVIOR { //PERSPECTIVE BEHAVIOR
    PERSPEC_SAVE = 0,
    PERSPEC_LOAD
};

namespace composer {
    namespace gui {
/*!
 \brief A dialog that allows the user to choose or save one perspective.
*/
class PerspectiveManager: public QDialog {
    Q_OBJECT

public:
    /*!
     \brief Constructor.
     \param parent The parent of this class.
    */
    PerspectiveManager(QWidget *parent = 0);
    /*!
     \brief Destructor.
    */
    virtual ~PerspectiveManager();
    /*!
     \brief Get the name of the perspective the user has selected.

     \return QString The name of the perspective selected.
    */
    QString getSelectedName();
    /*!
     \brief Set the current behavior of the PerpectiveManager. This function
        must be called before the show event to have effect.

     \param behavior
    */
    void setBehavior(PERSPEC_BEHAVIOR behavior);

public slots:
    /*!
     \brief
    */
    void accept();
    void deleteSelectedPerspective();

protected:
    /*!
     \brief Reimplemented to update the dialog with the information of
    */
    void showEvent(QShowEvent *);
    /*!
      \brief Removes the perspective with name: name.

      \param name The name of the perspective to be removed.
     */
    void deletePerspective(QString name);

private:
    Ui::PerpectiveManager *ui; /*!< The instantiation of the qtcreator designer
                                    generated class.*/
    QString selectedName; /*!< The current selected element. */
    PERSPEC_BEHAVIOR behavior; /*!< The current behavior of the
                                    PerspectiveManager */

    void updateContent();
};
} } //end namespace
#endif // PLUGINSLAYOUTMANAGER_H
