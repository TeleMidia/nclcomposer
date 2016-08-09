#ifndef STRUCTURAL_H
#define STRUCTURAL_H

#define PLG_ENTITY_PREFIX ":plugin:entity"

#define PLG_ENTITY_ID ":plugin:entity:id"
#define PLG_ENTITY_UID ":plugin:entity:uid"
#define PLG_ENTITY_CATEGORY ":plugin:entity:category"
#define PLG_ENTITY_TYPE ":plugin:entity:type"
#define PLG_ENTITY_MEDIA ":plugin:entity:media"

#define PLG_ENTITY_TOP ":plugin:entity:top"
#define PLG_ENTITY_LEFT ":plugin:entity:left"
#define PLG_ENTITY_WIDTH ":plugin:entity:width"
#define PLG_ENTITY_HEIGHT ":plugin:entity:height"
#define PLG_ENTITY_ZINDEX ":plugin:entity:zindex"

#define PLG_ENTITY_UNCOLLAPSED_TOP ":plugin:entity:uncollaped:top"
#define PLG_ENTITY_UNCOLLAPSED_LEFT ":plugin:entity:uncollaped:left"
#define PLG_ENTITY_UNCOLLAPSED_WIDTH ":plugin:entity:uncollaped:width"
#define PLG_ENTITY_UNCOLLAPSED_HEIGHT ":plugin:entity:uncollaped:height"

#define PLG_ENTITY_SELECTED ":plugin:entity:selected"
#define PLG_ENTITY_HIDDEN ":plugin:entity:hidden"

#define PLG_ENTITY_MOVEABLE ":plugin:entity:moveable"
#define PLG_ENTITY_SELECTABLE ":plugin:entity:selectable"
#define PLG_ENTITY_HOVERABLE ":plugin:entity:hoverable"
#define PLG_ENTITY_RESIZABLE ":plugin:entity:resizable"
#define PLG_ENTITY_DRAGGABLE ":plugin:entity:draggable"

#define PLG_ENTITY_SRC ":plugin:entity:src"
#define PLG_ENTITY_COLLAPSED ":plugin:entity:collapsed"

#define PLG_ENTITY_START_UID ":plugin:entity:start:uid"
#define PLG_ENTITY_END_UID ":plugin:entity:end:uid"
#define PLG_ENTITY_ROLE ":plugin:entity:role"
#define PLG_ENTITY_LINK_ID ":plugin:entity:link:id"
#define PLG_ENTITY_LINK_UID ":plugin:entity:link:uid"
#define PLG_ENTITY_XCONNECTOR_ID ":plugin:entity:xconnector:id"
#define PLG_ENTITY_COMPONENT_ID ":plugin:entity:component:id"
#define PLG_ENTITY_COMPONENT_UID ":plugin:entity:component:uid"
#define PLG_ENTITY_INTERFACE_ID ":plugin:entity:interface:id"
#define PLG_ENTITY_INTERFACE_UID ":plugin:entity:interface:uid"
#define PLG_ENTITY_ANGLE ":plugin:entity:angle"

#define PLG_SETTING_UNDO_CHILDREN ":plugin:setting:undo:children"
#define PLG_SETTING_UNDO ":plugin:setting:undo"
#define PLG_SETTING_NOTIFY ":plugin:setting:notify"
#define PLG_SETTING_CODE ":plugin:setting:code"

#define NCL_ENTITY_ID "id"
#define NCL_ENTITY_REFER "refer"
#define NCL_ENTITY_SRC "src"
#define NCL_ENTITY_INSTANCE "instance"
#define NCL_ENTITY_TYPE "type"
#define NCL_ENTITY_DESCRIPTOR "descriptor"
#define NCL_ENTITY_COMPONENT "component"
#define NCL_ENTITY_INTERFACE "interface"
#define NCL_ENTITY_NAME "name"
#define NCL_ENTITY_VALUE "value"
#define NCL_ENTITY_EXTERNABLE "externable"
#define NCL_ENTITY_XCONNECTOR "xconnector"

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

#define DEFAULT_ANCHOR_WIDTH  8
#define DEFAULT_ANCHOR_HEIGHT 8

#define DEFAULT_BODY_ENABLE 1

class Structural
{
public:
  enum StructuralCategory
  {
    Node        = 1,
    Edge        = 2,
    Interface   = 3,

    NoCategory  = 0
  };

  enum StructuralType
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

    NoType      = 0
  };

  enum StructuralResize {
    Top         = 1,
    TopRight    = 2,
    Right       = 3,
    BottomRight = 4,
    Bottom      = 5,
    BottomLeft  = 6,
    Left        = 7,
    TopLeft     = 8,

    NoResize    = 0
  };

  enum StructuralMedia {
    Image       = 1,
    Audio       = 2,
    Video       = 3,
    Text        = 4,
    HTML        = 5,
    NCL         = 6,
    NCLua       = 7,
    Settings    = 9,

    NoContent   = 0
  };


  enum StructuralRole
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

    NoRole
  };

};

typedef Structural::StructuralCategory StructualCategory;
typedef Structural::StructuralType StructuralType;
typedef Structural::StructuralResize StructuralResize;
typedef Structural::StructuralMedia StructuralMedia;
typedef Structural::StructuralRole StructuralRole;

#endif // STRUCTURAL_H
