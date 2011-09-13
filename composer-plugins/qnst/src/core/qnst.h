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
        Area
    };

    enum ConditionType {
        OnBegin,
        OnEnd,
        OnSelection,
        OnPause,
        OnResume,
        OnSet,
        OnAbort,
        NoCondition
    };

    enum ActionType {
        Start,
        Stop,
        Pause,
        Resume,
        Set,
        Abort,
        NoAction
    };
};

#endif // QNST_H
