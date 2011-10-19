/*
 * ReferenceStructure.h
 *
 *  Created on: 09/08/2011
 *      Author: Mária Gonçalves
 */

#ifndef REFERENCESTRUCTURE_H_
#define REFERENCESTRUCTURE_H_

#include <string>

namespace nclValidator {

using namespace std;

class ReferenceStructure {
	private:
            string from;
            string fromAtt;
            string to;
            string toAtt;
            string perspective;
            string perspectiveAtt;

	public:
            ReferenceStructure(string, string, string, string, string, string);
            virtual ~ReferenceStructure();

            string getFrom() const;
            string getFromAtt() const;
            string getTo() const;
            string getToAtt() const;            
            string getPerspective() const;
            string getPerspectiveAtt() const;
};

}

#endif /* REFERENCESTRUCTURE_H_ */
