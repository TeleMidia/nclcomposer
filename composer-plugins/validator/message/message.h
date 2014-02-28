/*
 * Copyright 2011-2013 Laws/UFMA.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string.h>
#include <sstream>
#include <iostream>
#include <QFile>
#include <QDir>
#include <map>
#include <stdarg.h>
#include <vector>

using namespace std;

class Message
{
public:
  Message (string language="en");
  string createMessage (int, int, const char *, ...);

private:
  string _language;
  map <int, string> _messages;
};


#endif /* MESSAGE_H_ */
