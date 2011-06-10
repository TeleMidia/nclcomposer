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

public Q_SLOTS:
    /*!
     \brief
    */
    void accept();

protected:
    /*!
     \brief Reimplemented to update the dialog with the information of
    */
    void showEvent(QShowEvent *);

private:
    Ui::PerpectiveManager *ui; /*!< The instantiation of the qtcreator designer
                                    generated class.*/
    QString selectedName; /*!< The current selected element. */
    PERSPEC_BEHAVIOR behavior; /*!< The current behavior of the
                                    PerspectiveManager */
};
} } //end namespace
#endif // PLUGINSLAYOUTMANAGER_H
