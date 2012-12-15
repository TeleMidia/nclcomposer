#ifndef HYPERTEXTVIEW_H
#define HYPERTEXTVIEW_H
#include<QtWebKit/QWebView>
#include<QString>

/*!
 \brief Handles the creation of hypertext objects.
*/

class hypertextview :public QWebView
{
public:

    /*!
     \brief Constructor.
     \param Filename is the variable for specifying the media object location.
    */
    hypertextview(QString filename);

    /*!
      \brief Destructor.
     */
  ~hypertextview();
};

#endif // HYPERTEXTVIEW_H
