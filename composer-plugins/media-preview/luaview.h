#ifndef LUAVIEW_H
#define LUAVIEW_H
#include<QTextEdit>
#include<QString>


/*!
 \brief Handles the creation of lua objects.
*/
class luaview : public QTextEdit
{
    Q_OBJECT
public:
    /*!
     \brief Constructor.
     \param Filename is the variable for specifying the media object location.
    */

    luaview(QString filename);

    /*!
      \brief Destructor.
     */
  ~luaview();
};

#endif // LUAVIEW_H
