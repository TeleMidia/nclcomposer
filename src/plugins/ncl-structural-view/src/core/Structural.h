#ifndef STRUCTURAL_H
#define STRUCTURAL_H

/*
 *
 * STRUCTURAL
 *
 */

//
// Properties
//

// Entity defines
#define STR_ATTR_ENT_ID ":ent:id"
#define STR_ATTR_ENT_UID ":ent:uid"
#define STR_ATTR_ENT_CATEGORY ":ent:category"
#define STR_ATTR_ENT_TYPE ":ent:type"

#define STR_ATTR_ENT_TOP ":ent:top"
#define STR_ATTR_ENT_LEFT ":ent:left"
#define STR_ATTR_ENT_WIDTH ":ent:width"
#define STR_ATTR_ENT_HEIGHT ":ent:height"
#define STR_ATTR_ENT_ZINDEX ":ent:zindex"
#define STR_ATTR_ENT_UNCOLLAPSED_TOP ":ent:uncollapsed:top"
#define STR_ATTR_ENT_UNCOLLAPSED_LEFT ":ent:uncollapsed:left"
#define STR_ATTR_ENT_UNCOLLAPSED_WIDTH ":ent:uncollapsed:width"
#define STR_ATTR_ENT_UNCOLLAPSED_HEIGHT ":ent:uncollapsed:height"

#define STR_ATTR_ENT_AUTOSTART ":ent:autostart"
#define STR_ATTR_ENT_HIDDEN ":ent:hidden"
#define STR_ATTR_ENT_REFERENCE ":ent:reference"
#define STR_ATTR_ENT_UNCOLLAPSED ":ent:uncollapsed"

// Node defines
#define STR_ATTR_NODE_SRC ":node:src"
#define STR_ATTR_NODE_TYPE ":node:type"
#define STR_ATTR_NODE_MIMETYPE ":node:mimetype"
#define STR_ATTR_NODE_INSTANCE ":node:instance"

// Link defines
#define STR_ATTR_LINKPARAM_NAME ":linkparam:name"
#define STR_ATTR_LINKPARAM_VALUE ":linkparam:value"

// Edge defines
#define STR_ATTR_EDGE_TAIL ":edge:tail"
#define STR_ATTR_EDGE_HEAD ":edge:head"
#define STR_ATTR_EDGE_ANGLE ":edge:angle"

#define STR_ATTR_BIND_ROLE ":bind:role"

#define STR_ATTR_BINDPARAM_NAME ":bindparam:name"
#define STR_ATTR_BINDPARAM_VALUE ":bindparam:value"

// Interface defines
// nothing...

// Reference defines
#define STR_ATTR_REFERENCE_LINK_ID ":ref:link:id"
#define STR_ATTR_REFERENCE_LINK_UID ":ref:link:uid"

#define STR_ATTR_REFERENCE_COMPONENT_ID ":ref:component:id"
#define STR_ATTR_REFERENCE_COMPONENT_UID ":ref:component:uid"

#define STR_ATTR_REFERENCE_INTERFACE_ID ":ref:interface:id"
#define STR_ATTR_REFERENCE_INTERFACE_UID ":ref:interface:uid"

#define STR_ATTR_REFERENCE_XCONNECTOR_ID ":ref:xconnector:id"
#define STR_ATTR_REFERENCE_XCONNECTOR_UID ":ref:xconnector:uid"

#define STR_ATTR_REFERENCE_REFER_ID ":ref:refer:id"
#define STR_ATTR_REFERENCE_REFER_UID ":ref:refer:uid"

//
// Settings
//
#define STR_SETTINGS_UNDO ":settings:undo"
#define STR_SETTINGS_UNDO_TRACE ":settings:undo:trace"
#define STR_SETTINGS_NOTIFY ":settings:notify"
#define STR_SETTINGS_CODE ":settings:code"
#define STR_SETTINGS_ADJUST_REFERENCE ":settings:adjust:reference"

//
// Values
//
#define STR_VALUE_TRUE "1"
#define STR_VALUE_FALSE "0"

//
// Defaults
//

// Minimap defines
#define STR_DEFAULT_MINIMAP_W 240
#define STR_DEFAULT_MINIMAP_H 160

// Scene defines
#define STR_DEFAULT_SCENE_W 3600
#define STR_DEFAULT_SCENE_H 2400

// Entities defines
#define STR_DEFAULT_ENTITY_PADDING 4

#define STR_DEFAULT_ENTITY_ANCHOR_W 8
#define STR_DEFAULT_ENTITY_ANCHOR_H 8

// Nodes defines
#define STR_DEFAULT_COMPOSITION_W 250
#define STR_DEFAULT_COMPOSITION_H 200

#define STR_DEFAULT_BODY_W 750
#define STR_DEFAULT_BODY_H 500

#define STR_DEFAULT_CONTENT_PADDING 4

#define STR_DEFAULT_CONTENT_W 48
#define STR_DEFAULT_CONTENT_H 64

#define STR_DEFAULT_CONTENT_TEXT_H 16
#define STR_DEFAULT_CONTENT_TEXT_W 16

#define STR_DEFAULT_LINK_PADDING 4

#define STR_DEFAULT_LINK_W 14
#define STR_DEFAULT_LINK_H 14

// Interfaces defines
#define STR_DEFAULT_INTERFACE_PADDING 1

#define STR_DEFAULT_INTERFACE_W 18
#define STR_DEFAULT_INTERFACE_H 18

// Edges defines
#define STR_DEFAULT_EDGE_PADDING 8

#define STR_DEFAULT_EDGE_TAIL_W 2
#define STR_DEFAULT_EDGE_TAIL_H 2

#define STR_DEFAULT_EDGE_HEAD_W 12
#define STR_DEFAULT_EDGE_HEAD_H 12

#define STR_DEFAULT_BIND_PADDING 0

#define STR_DEFAULT_BIND_ROLE_W 16
#define STR_DEFAULT_BIND_ROLE_H 16

// Others defines
#define STR_DEFAULT_ALERT_ICON_W 16
#define STR_DEFAULT_ALERT_ICON_H 16

#define STR_DEFAULT_ALERT_ERROR_ICON ":/icon/error"
#define STR_DEFAULT_ALERT_ERROR_COLOR "#BD0300"

#define STR_DEFAULT_ALERT_WARNING_ICON ":/icon/warning"
#define STR_DEFAULT_ALERT_WARNING_COLOR "#E5E500"

#define STR_DEFAULT_WITH_BODY 0
#define STR_DEFAULT_WITH_INTERFACES 0
#define STR_DEFAULT_WITH_FLOATING_INTERFACES 0

/*
 *
 * NESTED CONTEXT LANGUAGE
 *
 */

//
// Attributes
//

// Entities defines
#define NCL_ATTR_ID "id"
#define NCL_ATTR_REFER "refer"
#define NCL_ATTR_SRC "src"
#define NCL_ATTR_INSTANCE "instance"
#define NCL_ATTR_TYPE "type"
#define NCL_ATTR_DESCRIPTOR "descriptor"
#define NCL_ATTR_COMPONENT "component"
#define NCL_ATTR_INTERFACE "interface"
#define NCL_ATTR_NAME "name"
#define NCL_ATTR_VALUE "value"
#define NCL_ATTR_EXTERNABLE "externable"
#define NCL_ATTR_XCONNECTOR "xconnector"
#define NCL_ATTR_ROLE "role"
#define NCL_ATTR_COORDS "coords"
#define NCL_ATTR_BEGIN "begin"
#define NCL_ATTR_END "end"
#define NCL_ATTR_BEGINTEXT "beginText"
#define NCL_ATTR_ENDTEXT "endText"
#define NCL_ATTR_BEGINPOSITION "beginPosition"
#define NCL_ATTR_ENDPOSITION "endPosition"
#define NCL_ATTR_FIRST "first"
#define NCL_ATTR_LAST "last"
#define NCL_ATTR_LABEL "label"
#define NCL_ATTR_CLIP "clip"

class Structural
{
public:
  enum Category
  {
    Node = 1,
    Edge = 2,
    Interface = 3,

    NoCategory = 0
  };

  enum Type
  {
    Media = 1,
    Body = 2,
    Context = 3,
    Switch = 4,
    Link = 5,
    Port = 6,
    SwitchPort = 7,
    Area = 8,
    Property = 9,
    Bind = 10,
    Reference = 11,
    Mapping = 12,

    NoType = 0
  };

  enum Role
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

  enum MimeType
  {
    Image = 1,
    Audio = 2,
    Video = 3,
    Text = 4,
    HTML = 5,
    NCL = 6,
    NCLua = 7,
    Settings = 9,
    Time = 10,

    NoMimeType = 0
  };

  enum Resize
  {
    Top = 1,
    TopRight = 2,
    Right = 3,
    BottomRight = 4,
    Bottom = 5,
    BottomLeft = 6,
    Left = 7,
    TopLeft = 8,

    NoResize = 0
  };

  enum Mode
  {
    Pointing = 1,
    Linking = 2,

    NoMode = 0
  };
};

typedef Structural::Category StructuralCategory;
typedef Structural::Type StructuralType;
typedef Structural::Resize StructuralResize;
typedef Structural::MimeType StructuralMimeType;
typedef Structural::Role StructuralRole;
typedef Structural::Mode StructuralMode;

#endif // STRUCTURAL_H
