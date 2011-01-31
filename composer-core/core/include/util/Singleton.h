#ifndef __SINGLETON_HPP_
#define __SINGLETON_HPP_

#include <QObject>
#include <QDebug>

#include <assert.h>

namespace composer {
namespace core {
namespace util {


    template <class T>
    class Singleton
    {

    public:
      static T* getInstance() {
          if(!m_pInstance) {
              m_pInstance = new T;
          }
          assert(m_pInstance != NULL);
          return m_pInstance;
      }
      static void releaseInstance() {
          if (m_pInstance != NULL) {
              delete m_pInstance;
              m_pInstance = NULL;
          }
      }

    protected:
      Singleton(){}
      virtual ~Singleton(){}
    private:
      Singleton(Singleton const&) {}
      Singleton& operator=(Singleton const&){}
      static T* m_pInstance;
    };

    template <class T> T* Singleton<T>::m_pInstance=NULL;
}
}
}

#endif
