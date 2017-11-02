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
#define ST_ATTR_ENT_ID ":ent:id"
#define ST_ATTR_ENT_UID ":ent:uid"
#define ST_ATTR_ENT_CATEGORY ":ent:category"
#define ST_ATTR_ENT_TYPE ":ent:type"

#define ST_ATTR_ENT_TOP ":ent:top"
#define ST_ATTR_ENT_LEFT ":ent:left"
#define ST_ATTR_ENT_WIDTH ":ent:width"
#define ST_ATTR_ENT_HEIGHT ":ent:height"
#define ST_ATTR_ENT_ZINDEX ":ent:zindex"
#define ST_ATTR_ENT_UNCOLLAPSED_TOP ":ent:uncollapsed:top"
#define ST_ATTR_ENT_UNCOLLAPSED_LEFT ":ent:uncollapsed:left"
#define ST_ATTR_ENT_UNCOLLAPSED_WIDTH ":ent:uncollapsed:width"
#define ST_ATTR_ENT_UNCOLLAPSED_HEIGHT ":ent:uncollapsed:height"

#define ST_ATTR_ENT_AUTOSTART ":ent:autostart"
#define ST_ATTR_ENT_HIDDEN ":ent:hidden"
#define ST_ATTR_ENT_REFERENCE ":ent:reference"
#define ST_ATTR_ENT_COLLAPSED ":ent:collapsed"

// Node defines
#define ST_ATTR_NODE_SRC ":node:src"
#define ST_ATTR_NODE_TYPE ":node:type"
#define ST_ATTR_NODE_MIMETYPE ":node:mimetype"
#define ST_ATTR_NODE_INSTANCE ":node:instance"

// Link defines
#define ST_ATTR_LINKPARAM_NAME ":linkparam:name"
#define ST_ATTR_LINKPARAM_VALUE ":linkparam:value"

// Edge defines
#define ST_ATTR_EDGE_ORIG ":edge:orig"
#define ST_ATTR_EDGE_DEST ":edge:dest"
#define ST_ATTR_EDGE_ANGLE ":edge:angle"

#define ST_ATTR_BIND_ROLE ":bind:role"

#define ST_ATTR_BINDPARAM_NAME ":bindparam:name"
#define ST_ATTR_BINDPARAM_VALUE ":bindparam:value"

// Interface defines
// nothing...

// Reference defines
#define ST_ATTR_REFERENCE_LINK_ID ":ref:link:id"
#define ST_ATTR_REFERENCE_LINK_UID ":ref:link:uid"

#define ST_ATTR_REFERENCE_COMPONENT_ID ":ref:component:id"
#define ST_ATTR_REFERENCE_COMPONENT_UID ":ref:component:uid"

#define ST_ATTR_REFERENCE_INTERFACE_ID ":ref:interface:id"
#define ST_ATTR_REFERENCE_INTERFACE_UID ":ref:interface:uid"

#define ST_ATTR_REFERENCE_XCONNECTOR_ID ":ref:xconnector:id"
#define ST_ATTR_REFERENCE_XCONNECTOR_UID ":ref:xconnector:uid"

#define ST_ATTR_REFERENCE_REFER_ID ":ref:refer:id"
#define ST_ATTR_REFERENCE_REFER_UID ":ref:refer:uid"

//
// Settings
//
#define ST_SETTINGS_UNDO ":settings:undo"
#define ST_SETTINGS_UNDO_TRACE ":settings:undo:trace"
#define ST_SETTINGS_NOTIFY ":settings:notify"
#define ST_SETTINGS_CODE ":settings:code"
#define ST_SETTINGS_ADJUST_REFERS ":settings:adjust:refers"

//
// Values
//
#define ST_VALUE_TRUE "1"
#define ST_VALUE_FALSE "0"

//
// Defaults
//

// Minimap defines
#define ST_DEFAULT_MINIMAP_W 240
#define ST_DEFAULT_MINIMAP_H 160

// Scene defines
#define ST_DEFAULT_SCENE_W 3600
#define ST_DEFAULT_SCENE_H 2400

// Zoom values are in %
#define ST_DEFAULT_SCENE_ZOOM_MIN 25
#define ST_DEFAULT_SCENE_ZOOM_MAX 150
#define ST_DEFAULT_SCENE_ZOOM_ORIGINAL 100
#define ST_DEFAULT_SCENE_ZOOM_STEP 5

// Entities defines
#define ST_DEFAULT_ENTITY_PADDING 4

#define ST_DEFAULT_ENTITY_ANCHOR_W 8
#define ST_DEFAULT_ENTITY_ANCHOR_H 8

// Nodes defines
#define ST_DEFAULT_COMPOSITION_W 250
#define ST_DEFAULT_COMPOSITION_H 200

#define ST_DEFAULT_BODY_W 750
#define ST_DEFAULT_BODY_H 500

#define ST_DEFAULT_CONTENT_PADDING 4

#define ST_DEFAULT_CONTENT_W 48
#define ST_DEFAULT_CONTENT_H 64

#define ST_DEFAULT_CONTENT_TEXT_H 16
#define ST_DEFAULT_CONTENT_TEXT_W 16

#define ST_DEFAULT_LINK_PADDING 4

#define ST_DEFAULT_LINK_W 14
#define ST_DEFAULT_LINK_H 14

// Interfaces defines
#define ST_DEFAULT_INTERFACE_PADDING 1

#define ST_DEFAULT_INTERFACE_W 18
#define ST_DEFAULT_INTERFACE_H 18

// Edges defines
#define ST_DEFAULT_EDGE_PADDING 8

#define ST_DEFAULT_EDGE_TAIL_W 2
#define ST_DEFAULT_EDGE_TAIL_H 2

#define ST_DEFAULT_EDGE_HEAD_W 12
#define ST_DEFAULT_EDGE_HEAD_H 12

#define ST_DEFAULT_BIND_PADDING 0

#define ST_DEFAULT_BIND_ROLE_W 16
#define ST_DEFAULT_BIND_ROLE_H 16

// Others defines
#define ST_DEFAULT_ALERT_ICON_W 16
#define ST_DEFAULT_ALERT_ICON_H 16

#define ST_DEFAULT_ALERT_ERROR_ICON ":/icon/error"
#define ST_DEFAULT_ALERT_ERROR_COLOR "#BD0300"

#define ST_DEFAULT_ALERT_WARNING_ICON ":/icon/warning"
#define ST_DEFAULT_ALERT_WARNING_COLOR "#E5E500"

#define ST_OPT_WITH_BODY 1
#define ST_OPT_SHOW_INTERFACES 1
#define ST_OPT_USE_FLOATING_INTERFACES 0
#define ST_OPT_COMPOSITIONS_AS_RECT 0

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

#include <QMap>
typedef QMap<QString, QString> QStrMap;

#endif // STRUCTURAL_H
