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
        Aggregator,

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
        Condition,
        Action,
        Bind,

        NoType
    };

    enum ConditionType {
        onBegin,
        onSelectiononBegin,
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
