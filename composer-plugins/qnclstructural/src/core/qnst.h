/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNST_H
#define QNST_H

class Qnst
{
public:
    enum EntityType {
        Body,
        Port,
        Link,
        Switch,
        Context,
        Media,
        Property,
        Area,
        Refer
    };

    enum ConditionType {
        OnBegin,
        OnEnd,
        OnSelection,
        OnKeySelection,
        OnPause,
        OnResume,
        OnSet,
        OnAbort,
        NoCondition
    };

    enum ActionType {
        Start,
        StartDelay,
        Stop,
        Pause,
        Resume,
        Set,
        Abort,
        NoAction
    };
};

#endif // QNST_H
