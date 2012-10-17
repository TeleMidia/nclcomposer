#ifndef QNST_H
#define QNST_H

#define DEFAULT_BODY_WIDTH  750
#define DEFAULT_BODY_HEIGHT 500

#define DEFAULT_CONTEXT_WIDTH  250
#define DEFAULT_CONTEXT_HEIGHT 200

#define DEFAULT_MEDIA_WIDTH  48
#define DEFAULT_MEDIA_HEIGHT 64

#define DEFAULT_INTERFACE_WIDTH  18
#define DEFAULT_INTERFACE_HEIGHT 18

#define DEFAULT_AGGREGATOR_WIDTH  14
#define DEFAULT_AGGREGATOR_HEIGHT 14

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
        Html,
        NCL,
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
        SwitchPort,
        Property,

        Edge,
        Reference,
        Link,
        Mapping,
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
