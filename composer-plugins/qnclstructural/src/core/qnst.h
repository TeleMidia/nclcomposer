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
  enum EntityType
  {
    Node = 0,

    /* MEDIAS */
    Media,
    Image,
    Audio,
    Video,
    Html,
    NCL,
    Text,
    NCLua,
    Settings,
    /* END MEDIAS */

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

  enum BindType
  {
    /* Conditions */
    onBegin,
    onEnd,
    onSelection,
    onResume,
    onPause,

    /* Actions */
    Start,
    Stop,
    Resume,
    Pause,
    Set,

    NoBindType
  };
};

#endif // QNST_H
