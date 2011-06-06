#ifndef ENTITYNOTFOUND_H
#define ENTITYNOTFOUND_H

#include <QString>

#include <exception>
using namespace std;

namespace composer{
    namespace core {
        namespace util {

/*!
 \brief Exception to inform that the desired entity wasn't find (e.g. in the
            model).

*/
class EntityNotFound : public exception
{
    private:
        QString element; /*!< TODO */
        QString id; /*!< TODO */
    public:
        /*!
         \brief Contructor.

         \param _element
         \param _id
        */
        EntityNotFound(QString _element, QString _id) :
                element(_element), id(_id) {}
        /*!
            \brief Destructor.
        */
        ~EntityNotFound() throw() {}

        /*!
          \brief return a description of what the exception represents.
        */
        const char* what() const throw() {
            QString ret = "Entity (" + element + ") with id (" + id + ") not \
                            found!";
            return ret.toStdString().c_str();
        }
};

}}}
#endif // ENTITYNOTFOUND_H
