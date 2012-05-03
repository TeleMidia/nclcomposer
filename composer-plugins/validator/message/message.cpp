/*
 * message.cpp
 *
 *  Created on: Aug 9, 2011
 *      Author: Rodrigo Costa
 */

#include "message.h"

namespace nclValidator {

Message::Message (string language){
	_language = language;

	string fileName = "";
        if (_language == "pt_BR"){
                fileName = ":/config/pt_BR_messages.txt";
	}
        if (_language == "es"){
                fileName = ":/config/es_messages.txt";
        }
	else { // default: en
                fileName = ":/config/en_messages.txt";
	}

//	ifstream messageFile;

//	messageFile.open(fileName.c_str(), ifstream::in);
        QFile messageFile (QString::fromStdString(fileName));

        messageFile.open(QIODevice::ReadOnly | QIODevice::Text);

//	char line [256];
        std::string line;
        while (!messageFile.atEnd()){
//		messageFile.getline(line, 256);
                line = QString (messageFile.readLine()).toStdString();

		int i;
                for (i = 0; i < line.size(); i++){
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

		if (message.size()){
			_messages [index] = message;
//			cout << message << endl;
		}
	}

        messageFile.close();
}

string Message::createMessage(int messageId, int num_args, const char * first, ...){

	va_list argList;
        const char * str = first;
	vector <string> args;

	va_start(argList, str);

	for (int i=0; i < num_args; i++){
		args.push_back(str);
		str = va_arg(argList, char *);
	}
	va_end(argList);

	if (_messages.count(messageId)){
		string message = _messages[messageId];

		for (int i=0; i < args.size(); i++)
			message.replace (message.find("%s"), 2, args[i]);

		return message;
	}

	return "";


}

}
