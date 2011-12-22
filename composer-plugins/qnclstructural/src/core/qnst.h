#ifndef QNST_H
#define QNST_H

class Qnst
{
public:
    enum EntityType {
        Node,

        Content,
        Media,
        Image,
        Audio,
        Video,
        Text,
        Script,
        Settings,

        Compostion,
        Body,
        Context,
        Switch,

        Interface,
        Port,
        Area,
        Property,

        Edge,
        Reference,
        Link,
        Bind,

        NoType
    };

    enum ConditionType {
        onBegin,
        onEnd,
        onSelection,
        onResume,
        onPause,

        NoConditionType
    };

    enum ActionType {
        Start,
        Stop,
        Resume,
        Pause,
        Set,

        NoActionType
    };
};

#endif // QNST_H
