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
    Node        = 1,
    Edge        = 2,
    Interface   = 3,

    NoType      = 0
  };

  enum EntityName
  {
    Media       = 1,
    Body        = 2,
    Context     = 3,
    Switch      = 4,

    Port        = 5,
    Area        = 6,
    SwitchPort  = 7,
    Property    = 8,

    Link        = 9,
    Bind        = 10,
    Reference   = 11,

    NoName   = 0
  };

  enum EntityResize {
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left,
    TopLeft,

    NoResize
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

typedef Structural::EntityType QnstType;
typedef Structural::EntityName QnstName;

typedef Structural::EntityResize QnstResizeType;

#endif // QNST_H
