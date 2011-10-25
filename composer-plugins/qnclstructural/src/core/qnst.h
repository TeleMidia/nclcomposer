/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>. 
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
