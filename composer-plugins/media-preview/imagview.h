#ifndef IMAGVIEW_H
#define IMAGVIEW_H
#include<QLabel>
#include<QString>

/*!
 \brief Handles the creation of image objects.
*/

class imagview :public QLabel
{
 Q_OBJECT

public:

    /*!
     \brief Constructor.
     \param Filename is the variable for specifying the media object location.
    */

    imagview(QString filename);
    /*!
      \brief Destructor.
     */
  ~imagview();
};

#endif // IMAGVIEW_H
