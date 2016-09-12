#ifndef STRUCTURAL_H
#define STRUCTURAL_H

#define PLG_PROPERTY_ID ":plugin:property:id"
#define PLG_PROPERTY_UID ":plugin:property:uid"
#define PLG_PROPERTY_CATEGORY ":plugin:property:category"
#define PLG_PROPERTY_TYPE ":plugin:property:type"
#define PLG_PROPERTY_MIMETYPE ":plugin:property:mimetype"

#define PLG_PROPERTY_TOP ":plugin:property:top"
#define PLG_PROPERTY_LEFT ":plugin:property:left"
#define PLG_PROPERTY_WIDTH ":plugin:property:width"
#define PLG_PROPERTY_HEIGHT ":plugin:property:height"
#define PLG_PROPERTY_ZINDEX ":plugin:property:zindex"

#define PLG_PROPERTY_HIDDEN ":plugin:property:hidden"

#define PLG_ENTITY_UNCOLLAPSED ":plugin:property:uncollapsed"
#define PLG_ENTITY_UNCOLLAPSED_TOP ":plugin:property:uncollaped:top"
#define PLG_ENTITY_UNCOLLAPSED_LEFT ":plugin:property:uncollaped:left"
#define PLG_ENTITY_UNCOLLAPSED_WIDTH ":plugin:property:uncollaped:width"
#define PLG_ENTITY_UNCOLLAPSED_HEIGHT ":plugin:property:uncollaped:height"

#define PLG_PROPERTY_LOCATION ":plugin:property:location"

#define PLG_ENTITY_START_UID ":plugin:property:start:uid"
#define PLG_ENTITY_END_UID ":plugin:property:end:uid"
#define PLG_ENTITY_ROLE ":plugin:property:role"
#define PLG_ENTITY_LINK_ID ":plugin:property:link:id"
#define PLG_ENTITY_LINK_UID ":plugin:property:link:uid"
#define PLG_ENTITY_XCONNECTOR_ID ":plugin:property:xconnector:id"
#define PLG_ENTITY_COMPONENT_ID ":plugin:property:component:id"
#define PLG_ENTITY_COMPONENT_UID ":plugin:property:component:uid"
#define PLG_ENTITY_INTERFACE_ID ":plugin:property:interface:id"
#define PLG_ENTITY_INTERFACE_UID ":plugin:property:interface:uid"
#define PLG_ENTITY_ANGLE ":plugin:property:angle"
#define PLG_ENTITY_ROLE ":plugin:property:role"

#define PLG_ENTITY_LINKPARAM_NAME ":plugin:property:linkparam:name"
#define PLG_ENTITY_LINKPARAM_VALUE ":plugin:property:linkparam:value"
#define PLG_ENTITY_BINDPARAM_NAME ":plugin:property:bindparam:name"
#define PLG_ENTITY_BINDPARAM_VALUE ":plugin:property:bindparam:value"

#define PLG_SETTING_UNDO_CHILDREN ":plugin:setting:undo:children"
#define PLG_SETTING_UNDO ":plugin:setting:undo"
#define PLG_SETTING_NOTIFY ":plugin:setting:notify"
#define PLG_SETTING_CODE ":plugin:setting:code"

#define PLG_VALUE_TRUE "1"
#define PLG_VALUE_FALSE "0"

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
#define NCL_ENTITY_ROLE "role"

#define NCL_ENTITY_COORDS "coords"
#define NCL_ENTITY_BEGIN "begin"
#define NCL_ENTITY_END "end"
#define NCL_ENTITY_BEGINTEXT "beginText"
#define NCL_ENTITY_ENDTEXT "endText"
#define NCL_ENTITY_BEGINPOSITION "beginPosition"
#define NCL_ENTITY_ENDPOSITION "endPosition"
#define NCL_ENTITY_FIRST "first"
#define NCL_ENTITY_LAST "last"
#define NCL_ENTITY_LABEL "label"
#define NCL_ENTITY_CLIP "clip"



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
    SwitchPort  = 6,
    Area        = 7,
    Property    = 8,
    Link        = 9,
    Bind        = 10,
    Reference   = 11,
    Mapping     = 12,

    NoType      = 0
  };

  enum StructuralRole
  {
    onBegin,
    onEnd,
    onSelection,
    onResume,
    onPause,
    onBeginAttribution,
    onEndAttribution,
    onPauseAttribution,
    onResumeAttribution,

    Start,
    Stop,
    Resume,
    Pause,
    Set,

    NoRole
  };

  enum StructuralMimeType {
    Image       = 1,
    Audio       = 2,
    Video       = 3,
    Text        = 4,
    HTML        = 5,
    NCL         = 6,
    NCLua       = 7,
    Settings    = 9,

    NoMimeType  = 0
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
};

typedef Structural::StructuralCategory StructualCategory;
typedef Structural::StructuralType StructuralType;
typedef Structural::StructuralResize StructuralResize;
typedef Structural::StructuralMimeType StructuralMimeType;
typedef Structural::StructuralRole StructuralRole;

#endif // STRUCTURAL_H
