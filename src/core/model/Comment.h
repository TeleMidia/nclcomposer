/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef COMMENT_H
#define COMMENT_H

#include "util/ComposerCore_global.h"

#include "model/Node.h"

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief TODO.
 */
class COMPOSERCORESHARED_EXPORT Comment : public Node
{
  Q_OBJECT

  friend class Project;
  friend class MessageControl;
  friend class ProjectReader;

public:
  QString
  get ()
  {
    return this->_content;
  }

  Comment *
  cloneComment ()
  {
    return new Comment (this->_content, this->_doc, this->_parent);
  }

  virtual QString
  toString (int ntabs, bool writeuid) override
  {
    QString out = "";
    for (int i = 0; i < ntabs; i++)
      out += "\t";

    out += "<cpr_comment";
    if (writeuid)
    {
      out += " uniqueEntityId=\"";
      out += getUniqueId ();
      out += "\"";
    }

    out += " content=\"";
    out += this->_content;
    out += "\"/>\n";

    return out;
  }

protected:
  explicit Comment (QString &uid, const QString &content, QDomDocument &doc,
                    Node *parent = 0)
      : Node (uid, doc, parent)
  {
    this->_content = content;
  }

  explicit Comment (const QString &content, QDomDocument &doc,
                    Node *parent = 0)
      : Node (doc, parent)
  {
    this->_content = content;
  }

  void
  set (const QString &content)
  {
    this->_content = content;
  }

private:
  QString _content;
};

CPR_CORE_END_NAMESPACE

#endif // COMMENT_H
