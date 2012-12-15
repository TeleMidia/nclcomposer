#ifndef NCLVIEW_H
#define NCLVIEW_H
#include<QTextEdit>

/*!
 \brief Handles the creation of NCL objects.
*/

class nclview :public QTextEdit
{
public:
    /*!
     \brief Constructor.
     \param Filename is the variable for specifying the media object location.
    */

    nclview(QString filename);
    /*!
      \brief Destructor.
     */
  ~nclview();
};

#endif // NCLVIEW_H
