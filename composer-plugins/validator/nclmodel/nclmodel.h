/*
 * nclmodel.h
 *
 *  Created on: 12/06/2011
 *      Author: José Rios
 */

#ifndef NCLMODEL_H_
#define NCLMODEL_H_

#include <vector>
#include <map>
#include <set>
#include <list>
#include <limits>
#include <sstream>
#include <fstream>
#include "textualparser.h"
#include "connectorparser.h"
#include "../definitions/definitions.h"
#include "../langstruct/langstruct.h"

namespace nclValidator {

class Model;
class ConnectorParser;

class ModelElement {
public:
	explicit ModelElement (virtualId parentId = "");
	~ModelElement ();


	void setElementName (string);
	void setParent (virtualId);
	void addAttribute (Attribute&);
	void addAttribute (string, string);
        void setAttributes (vector <Attribute>&);

	inline const vector<virtualId> children () const { return _children; }
	inline const vector <virtualId> references () const { return _referencesToMyself; }
	inline const virtualId & id () const { return _id; }
	inline const virtualId & parent () const { return _parentId; }
	inline const string & elementName () const { return _elementName; }
	inline const string & scope () const { return _scope; }

        inline void setData (void * data) { _data = data; }
        inline void * data () const { return _data; }

	inline vector <Attribute> attributes () const { return _attributes; }

	Attribute attribute (string) const;

	friend class Model;

private:
	inline void setScope (string scope) { _scope = scope; }
	inline void addReference (virtualId id) { _referencesToMyself.push_back(id); }

        bool removeReference (virtualId);
	void addChild (virtualId);
	void removeChild (virtualId);

	virtualId _id;
	virtualId _parentId;
	string _elementName;
	vector <Attribute> _attributes;
	string _scope;
	vector <virtualId> _children;
	vector <virtualId> _referencesToMyself;
        void * _data;
};


class Model{
public:
	explicit Model ();
	~Model ();

        void clear ();

	virtualId addChild (virtualId parentId, string elementName, vector<Attribute> attributes);
	void addChild (virtualId parent, virtualId child);
	virtualId addElement (string elementName, vector<Attribute> attributes);
	vector <ModelElement *> elementByIdentifier (string identifier);
        vector <ModelElement *> elementByPropertyName (string component, string propertyName);


        inline void addElementWithErrorInLastPass (virtualId id){_elementsWithErrorInLastPass.insert(id);}
        inline void clearElementsWithErrorInLastPass () { _elementsWithErrorInLastPass.clear(); }

        set <virtualId> elementsWithErrorInLastPass ();

	bool removeElement (virtualId &);
        bool editElement (virtualId &, vector<Attribute> &);
	ModelElement * element (const virtualId &);
	set <virtualId> affectedElements ();


	inline bool hasElement (virtualId &id) { return _modelElements.count (id) > 0; }
        inline void clearMarkedElements () {_markedElements.clear(); _affectedEllements.clear();}
        inline const set <virtualId> & markedElements () { return _markedElements; }

        inline void setRelativePath (const string & relativePath) { _relativePath = relativePath; }
        map<string, pair<int, int> > getConnectorSetRoles (string identifier);
        void parseConnectorChild (ModelElement *child, map <string, pair <int, int> > &roles);

	void print ();

private:
	virtualId randomId ();
	void setElementId (virtualId, ModelElement &);
	void adjustScope (virtualId parent, virtualId child);
        void adjustReference (string, ModelElement &, string &);
	void addChild (ModelElement *parentElement, ModelElement *childElement);
        void parseAllConnectorChildren (ModelElement *, ModelElement *, map <string, pair <int, int> > &);

        void findElementInImportedFile (string, QXmlContentHandler *);

	int _seed;
	map <virtualId, ModelElement> _modelElements;

        set <virtualId> _nonIncrementalMarkedElements;

        set <virtualId> _markedElements;
        set <virtualId> _affectedEllements;
        set <virtualId> _elementsWithErrorInLastPass;

	multimap <string, virtualId> _idToElement;
	multimap <string, virtualId> _elementsNotYetInserted;

//        vector <pair<string, string> > _importedDocuments;
        vector <ModelElement> _importedElements; //provisorio

        string _relativePath;
};

}
#endif /* NCLMODEL_H_ */
