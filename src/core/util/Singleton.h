/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef __SINGLETON_HPP_
#define __SINGLETON_HPP_

#include <QObject>
#include <assert.h>

#define SINGLETON(T) \
private: static T* m_pInstance; \
public: \
static T* getInstance(){ \
        if(m_pInstance == nullptr) return m_pInstance = new T(); \
        return m_pInstance; \
 } \
static void releaseInstance(){ \
        if(m_pInstance != nullptr) delete m_pInstance; \
        m_pInstance = nullptr; \
 }

#define INIT_SINGLETON(T) \
    T* T::m_pInstance = nullptr;

#endif
