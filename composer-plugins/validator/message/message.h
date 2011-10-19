/*
 * message.h
 *
 *  Created on: Aug 9, 2011
 *      Author: Rodrigo Costa
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string.h>
#include <sstream>
#include <iostream>
#include <QFile>
#include <map>
#include <stdarg.h>
#include <vector>


namespace nclValidator {

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

}
#endif /* MESSAGE_H_ */
