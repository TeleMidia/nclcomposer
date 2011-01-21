#ifndef PARENTNOTFOUND_H
#define PARENTNOTFOUND_H

#include <QString>

#include <exception>
using namespace std;

namespace composer{
    namespace core {
        namespace util {

class ParentNotFound : public exception
{
    private:
        QString element;
        QString parent;
        QString id;
    public:
        ParentNotFound(QString _element, QString _parent ,QString _id) :
                element(_element), parent(_parent), id(_id) {}
        ~ParentNotFound() throw() {}
        const char* what() const throw() {
            QString ret = "Entity (" + parent + ") parent of "
                          + element + "(" + id + ") could not be found!";
            return ret.toStdString().c_str();
        }
};
}}}
#endif // PARENTNOTFOUND_H
