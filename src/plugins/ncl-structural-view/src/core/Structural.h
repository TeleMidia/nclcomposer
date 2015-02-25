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

class Structural
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
    Stream,
    /* END MEDIAS */


    Aggregator, // \fixme this is here only for compatibility with versions
                // prior to 0.1.3

    Composition,
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
    BindParam, /* Warning: This is not an Entity */

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
    onBeginAttribution,
    onEndAttribution,
    onPauseAttribution,
    onResumeAttribution,

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
