#ifndef ENTITYNOTFOUND_H
#define ENTITYNOTFOUND_H

#include <QString>

#include <exception>
using namespace std;

namespace composer{
    namespace core {
        namespace util {
class EntityNotFound : public exception
{
    private:
        QString element;
        QString id;
    public:
        EntityNotFound(QString _element, QString _id) :
                element(_element), id(_id) {}
        ~EntityNotFound() throw() {}
        const char* what() const throw() {
            QString ret = "Entity (" + element + ") with id (" + id + ") not found!";
            return ret.toStdString().c_str();
        }
};

}}}
#endif // ENTITYNOTFOUND_H
