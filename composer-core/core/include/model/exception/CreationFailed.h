#ifndef CREATIONFAILED_H
#define CREATIONFAILED_H

#include <QString>

#include <exception>
using namespace std;

namespace composer{
    namespace core {
        namespace util {
class CreationFailed : public exception
{
    private:
        QString element;
        QString id;
    public:
        CreationFailed(QString _element, QString _id) :
                element(_element), id(_id) {}
        ~CreationFailed() throw() {}
        const char* what() const throw() {
            QString ret = "Entity (" + element + ") with id (" + id + ") could "
            + "not be created!";
            return ret.toStdString().c_str();
        }
};
}}}

#endif // CREATIONFAILED_H
