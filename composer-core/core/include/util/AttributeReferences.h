#ifndef ATTRIBUTEREFERENCES_H
#define ATTRIBUTEREFERENCES_H

#include <QString>
/**
  */

namespace composer {
namespace util {

class AttributeReferences {
private:
    QString element, attr,
    ref_element, ref_attr;

public:
    AttributeReferences ( QString element,
                          QString attr,
                          QString ref_element,
                          QString ref_attr);

    QString getElement();
    QString getAttribute();
    QString getRefElement();
    QString getRefAttribute();
};

}} //end namespace
#endif
