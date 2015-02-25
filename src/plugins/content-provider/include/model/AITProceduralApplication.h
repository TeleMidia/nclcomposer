#ifndef AITPROCEDURALAPPLICATION_H_
#define AITPROCEDURALAPPLICATION_H_

#include <string>
#include "AITApplication.h"

using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
	class AITProceduralApplication : public AITApplication {
		private:
			string parameter;
			string baseDirectory;
			string classpathExtension;
			string initialClass;
		
		public:
			AITProceduralApplication();
			virtual ~AITProceduralApplication();
			
			virtual string getParameter();
			virtual void	setParameter(string newParameter);
			
			virtual string getBaseDirectory();
			virtual void	setBaseDirectory(string newBaseDirectory);
			
			virtual string getClasspathExtension();
			virtual void	setClasspathExtension(string newClasspathExtension);
			
			virtual string getInitialClass();
			virtual void	setInitialClass(string newInitialClass);
	};
   }
  }
 }
}

#endif /*AITPROCEDURALAPPLICATION_H_*/
