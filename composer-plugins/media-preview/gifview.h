#ifndef GIFVIEW_H
#define GIFVIEW_H
#include<QLabel>
#include<QString>
#include<QMovie>
#include<QtTest>

/*!
 \brief Handles the creation of image objects, but gif type.
*/

class gifview : public QLabel
{
    Q_OBJECT
public: 

    /*!
    * \brief Constructor.
    * \param Filename is the variable for specifying the media object location.
    */
    gifview(QString filename);

    /*!
      \brief Destructor.
     */
  ~gifview();
private:
    QMovie *movie;
};

#endif // GIFVIEW_H
