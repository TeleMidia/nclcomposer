#include "../../include/functions.h"

namespace org {
namespace isdtv {
namespace util {
	string gingaDocumentPath;
	string gingaCurrentPath;

	int strHexaToInt(string value) {
		std::stringstream str1(value);
		int number;

		if (value == "") {
			number = -1;

		} else if (value.find("0x") != string::npos) {
			if (!(str1 >> std::hex >> number)) {
				number = -1; 
			}

		} else {
			if (!(str1 >> number)) {
				number = -1; 
			}
		}
		return number;
	}

	string intToStrHexa(int value) {
		stringstream str;
		str << std::hex << std::uppercase << value;
		return "0x" + str.str();
	}

	string lowerCase(string s) {
		for (unsigned int j=0; j<s.length(); ++j) {
			s[j]=tolower(s[j]);
		}
		return s;
	}

	string upperCase(string s) {
		if (s == "") {
			return "";
		}

		for (unsigned int j=0; j<s.length(); ++j) {
			s[j]=toupper(s[j]);
		}
		return s;
	}

	bool isNumeric(void* value) {
		bool r = true;
		char* str = (char*)value;

		for (unsigned int i = 0; i < strlen(str); i++) {
			if (i == 0)
				if ((str[i] < '0' || str[i] > '9') &&
					    (str[i] != '.') &&
					    (str[i] != '+' || str[i] != '-'))
					r = false;

			if ((str[i] < '0' || str[i] > '9') && (str[i] != '.'))
				r = false;
		}

		return r;
	}

	string itos(double i) {
		ostringstream os;
		os << i;
		return os.str();
	}

	float stof(string s) {
		float f = 0.0f;
		stringstream ss;
		ss << s;
		ss >> f;
		return f;
	}

	float itof(int i) {
		string s = itos(i);
		return stof(s);
	}

	string absoluteFile(string basePath, string filename) {
		/*uri_t* uriFile = NULL;
		string protocol, path;*/

		//TODO: corrigir isso para receber os ids absolutos do XML
		if (filename.substr(0,6) == "sbtvd:"
			    || filename.substr(0,7) == "http://"
			    || filename.substr(0,6) == "ftp://"
			    || filename.substr(0,1) == "/"
			    || filename.substr(0,7) == "file://"
			    || filename.substr(0,6) == "tcp://"
			    || filename.substr(0,6) == "udp://"
			    || filename.substr(0,6) == "rtp://")
			return filename;

		filename = basePath + "/" + filename;

		/*try {
			uriFile = uri_alloc((char*)filename.c_str(), filename.length());
			protocol = uri_scheme(uriFile);
			path = uri_path(uriFile);
		}
		catch(...) {
			if(uriFile!=NULL)
				uri_free(uriFile);
			return filename;
		}

		if(protocol=="") {
			if(filename.substr(0,1)!="/") { //o que inclui . ou  ..
				filename = basePath + "/" + filename;
			}
		}*/

		//uri_free(uriFile);
		return filename;
	}

	string getPath(string filename) {
		/*string path;
		uri_t* uriFile = NULL;
		try {
			uriFile = uri_alloc((char*)filename.c_str(), filename.length());
			path = uri_all_path(uriFile);
		}
		catch(...) {
			if(uriFile!=NULL)
				uri_free(uriFile);
			return "";
		}
		path = path.substr(0, path.find_last_of('/'));
		//uri_free(uriFile);
		return path;*/
		return filename.substr(0, filename.find_last_of('/'));
	}

	string getCurrentPath() {
		return gingaCurrentPath;
	}

	vector<string>* split(string str, string delimiter) {
		vector<string>* splited;
		splited = new vector<string>;
		cout << "FUNCTIONS::SPLIT str = '" << str << "'" << endl;

		if (str == "") {
			return splited;
		}

		if (str.find_first_of(delimiter) == std::string::npos) {
			splited->push_back(str);
			return splited;
		}

		string::size_type lastPos = str.find_first_not_of(delimiter, 0);
		string::size_type pos = str.find_first_of(delimiter, lastPos);

		while (string::npos != pos || string::npos != lastPos) {
		    splited->push_back(str.substr(lastPos, pos - lastPos));
		    lastPos = str.find_first_not_of(delimiter, pos);
		    pos = str.find_first_of(delimiter, lastPos);
		}
		return splited;
	}

	string trim(string str) {
		string strR = "";

		string::size_type lastPos = str.find_first_not_of(" ", 0);
		string::size_type pos = str.find_first_of(" ", lastPos);

		while (string::npos != pos || string::npos != lastPos) {
		    strR = strR + str.substr(lastPos, pos - lastPos);
		    lastPos = str.find_first_not_of(" ", pos);
		    pos = str.find_first_of(" ", lastPos);
		}
		return strR;
	}

	float getPercentualValue(string value) {
		string actualValue;
		float floatValue;

		//retirar o caracter percentual da string
		actualValue = value.substr(0, (value.length() - 1));
		//converter para float
		floatValue = stof(actualValue);

		//se menor que zero, retornar zero
		if (floatValue < 0)
			floatValue = 0;
		else if (floatValue > 100)
			//se maior que 100, retornar 100
			floatValue = 100;

		//retornar valor percentual
		return floatValue;
	}

	/**
	 * Testa se uma string indica um valor percentual
	 * @param value string com um valor
	 * @return true se o valor e' percentual; false caso contrario.
	 */
	bool isPercentualValue(string value) {
		if (value.find_last_of("%") == (value.length() - 1))
			return true;
		else
			return false;
	}

	bool fileExists(string fileName) {
		if (fileName == "") {
			return false;
		}

		try {
			if (fileName.find_first_of("/") != 0) {
				char path[PATH_MAX] = "";
				getcwd(path, PATH_MAX);
				char* p = (char*)malloc(strlen(path) + 1);
			    if (p)
					strcpy(p, path);

				string currentPath;
				currentPath = (string)p;
				fileName = currentPath + "/" + fileName;
			}

			fstream fin;
			fin.open(fileName.c_str(), std::ios::in);

			if(fin.is_open()) {
				fin.close();
				return true;
			}
			fin.close();
			return false;

		} catch (...) {
			return false;
		}
	}

	void setDocumentPath(string docPath) {
		if (docPath.find_last_of("/") != docPath.length()) {
			docPath = docPath + "/";
		}
		gingaDocumentPath = docPath;
	}

	string getDocumentPath() {
		return gingaDocumentPath;
	}

	void initializeCurrentPath() {
		char path[PATH_MAX] = "";
		getcwd(path, PATH_MAX);
		char* p = (char*)malloc(strlen(path) + 1);
	    if (p)
			strcpy(p, path);

		gingaCurrentPath = (string)p;
		if (gingaCurrentPath.find_last_of("/") != gingaCurrentPath.length()) {
			gingaCurrentPath = gingaCurrentPath + "/";
		}
	}

numeric_limits<double> double_info;
double notANumber = double_info.quiet_NaN();
double doubleInfinity = double_info.infinity();

	double NaN() {
		return notANumber;
	}

	double infinity() {
		return doubleInfinity;
	}

	bool isNaN(double value) {
		string sval;
		sval = itos(value);
		/*cout << "functions::isNaN val = '" << value << "' ";
		cout << "sval = '" << sval << "'";*/
		if (isNumeric((void*)(sval.c_str()))) {
			return false;
		}

		if (upperCase(sval) == "NAN") {
			return true;
		}

		/*cout << "isNaN Warning! Value = '" << value << "', ";
		cout << "sval = " << sval << endl;*/
		return false;
	}

	bool isInfinity(double value) {
		string sval;
		sval = itos(value);
		if (isNumeric((void*)(sval.c_str()))) {
			return false;
		}

		if (upperCase(sval) == "INF") {
			return true;
		}

		/*cout << "isInfinity Warning! Value = '" << value << "', ";
		cout << "sval = " << sval << endl;*/
		return false;
	}

	double startTimeMills;
	bool firstCallTimeMills = true;
	
	double getCurrentTimeMillis() {
		struct timeb t;

		ftime(&t);

		if(firstCallTimeMills) {
			firstCallTimeMills = false;
			startTimeMills = (double)t.time*1000 + (double)t.millitm - 1;
			return 1;
		}

		return (double)t.time*1000 + (double)t.millitm - startTimeMills;
	}

	bool isAbsolutePath(string path) {
		/*uri_t* base_uri = NULL;
		string protocol;*/

		//if (SOFLAG == 1) {
			unsigned int pos;
			while (true) {
				pos = path.find_first_of("\\");
				if (pos == std::string::npos) {
					break;
				}
				path.replace(pos,1,"/");
			}
		//}

		if (path.substr(0,10) == "x-sbtvdts:"
			    || path.substr(0,7) == "http://"
			    || path.substr(0,6) == "ftp://"
			    || path.substr(0,1) == "/"
			    || path.substr(0,7) == "file://"
			    || path.substr(0,6) == "tcp://"
			    || path.substr(0,6) == "udp://"
			    || path.substr(0,6) == "rtp://")
			return true;

		return false;
		/*try {
			base_uri = uri_alloc((char*)path.c_str(), path.length());
			protocol = uri_scheme(base_uri);
		}
		catch(...) {
			return true;
		}

		if(protocol=="" && path.substr(0,1)!="/")
			return false;
		else
			return true;*/
	}
	
	time_t makeDateTime(int yyyy, int mm, int dd, int hh, int nn, int ss) {
		struct tm t;
		
		t.tm_year = yyyy - 1900;
		t.tm_mon = mm - 1;
		t.tm_mday = dd;
		t.tm_hour = hh;
		t.tm_min = nn;
		t.tm_sec = ss;
		t.tm_isdst = 0;

		return mktime(&t);
	}
  }
 }
}
