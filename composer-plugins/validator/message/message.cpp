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
#include "message.h"

Message::Message (string language)
{
  _language = language;

  string fileName = ":/config/" + language + "_messages.txt";
  //	ifstream messageFile;

  //	messageFile.open(fileName.c_str(), ifstream::in);
  QFile messageFile (QString::fromStdString(fileName));
  if (!messageFile.exists())
    messageFile.setFileName(":/config/en_messages.txt");

  messageFile.open(QIODevice::ReadOnly | QIODevice::Text);

  std::string line;
  while (!messageFile.atEnd())
  {
    line = QString (messageFile.readLine()).toStdString();

    uint i;
    for (i = 0; i < line.size(); i++)
    {
      if (line[i] != ' ')
        break;
    }

    if (line[i] == '#')
      continue;

    stringstream s (&line[0] + i);

    int index;
    string _;
    string message = "";
    string str;

    s >> index >> _;

    while (s >> str)
      message += str + " ";

    if (message.size())
    {
      _messages [index] = message;
      //			cout << message << endl;
    }
  }

  messageFile.close();
}

string Message::createMessage( int messageId,
                               size_t num_args,
                               const char * first,
                               ... )
{

	va_list argList;
	const char * str = first;
	vector <string> args;

    va_start(argList, first);

    for (size_t i = 0; i < num_args; i++)
	{
		args.push_back(str);
        str = va_arg(argList, const char *);
	}
	va_end(argList);

	if (_messages.count(messageId))
	{
		string message = _messages[messageId];

        for (size_t i = 0; i < args.size(); i++)
			message.replace (message.find("%s"), 2, args[i]);

		return message;
	}

	return "";
}
