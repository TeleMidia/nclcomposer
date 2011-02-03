#ifndef __SINGLETON_HPP_
#define __SINGLETON_HPP_

#include <QObject>
#include <QDebug>

#include <assert.h>


#define SINGLETON(T) \
private: static T* m_pInstance; \
public: \
static T* getInstance(){ \
        if(m_pInstance == NULL) return m_pInstance = new T(); \
        return m_pInstance; \
 } \
static void releaseInstance(){ \
        if(m_pInstance != NULL) delete m_pInstance; \
        m_pInstance = NULL; \
 }

#define INIT_SINGLETON(T) \
    T* T::m_pInstance = NULL;

#endif
