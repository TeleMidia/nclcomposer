#ifndef TEXTVIEW_H
#define TEXTVIEW_H
#include<QString>
#include<QTextEdit>
#include<QFile>

/*!
 \brief Handles the creation of text objects.
*/
class textview : public QTextEdit
{
public:

    /*!
     \brief Constructor.
     \param Filename is the variable for specifying the media object location.
    */
    textview(QString Filename);
    /*!
      \brief Destructor.
     */
  ~textview();
private:

    QFile *inputFile;
};

#endif // TEXTVIEW_H
