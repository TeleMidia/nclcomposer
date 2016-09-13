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
#define STR_PROPERTY_ENTITY_ID ":structural:property:entity:id"
#define STR_PROPERTY_ENTITY_UID ":structural:property:entity:uid"
#define STR_PROPERTY_ENTITY_CATEGORY ":structural:property:entity:category"
#define STR_PROPERTY_ENTITY_TYPE ":structural:property:entity:type"

#define STR_PROPERTY_ENTITY_TOP ":structural:property:entity:top"
#define STR_PROPERTY_ENTITY_LEFT ":structural:property:entity:left"
#define STR_PROPERTY_ENTITY_WIDTH ":structural:property:entity:width"
#define STR_PROPERTY_ENTITY_HEIGHT ":structural:property:entity:height"
#define STR_PROPERTY_ENTITY_ZINDEX ":structural:property:entity:zindex"
#define STR_PROPERTY_ENTITY_UNCOLLAPSED_TOP ":structural:property:entity:uncollaped:top"
#define STR_PROPERTY_ENTITY_UNCOLLAPSED_LEFT ":structural:property:entity:uncollaped:left"
#define STR_PROPERTY_ENTITY_UNCOLLAPSED_WIDTH ":structural:property:entity:uncollaped:width"
#define STR_PROPERTY_ENTITY_UNCOLLAPSED_HEIGHT ":structural:property:entity:uncollaped:height"

#define STR_PROPERTY_ENTITY_HIDDEN ":structural:property:entity:hidden"
#define STR_PROPERTY_ENTITY_UNCOLLAPSED ":structural:property:entity:uncollapsed"

// Node defines
#define STR_PROPERTY_CONTENT_LOCATION ":structural:property:content:location"
#define STR_PROPERTY_CONTENT_MIMETYPE ":structural:property:content:mimetype"

#define STR_PROPERTY_LINKPARAM_NAME ":structural:property:linkparam:name"
#define STR_PROPERTY_LINKPARAM_VALUE ":structural:propertylinkparam:value"

// Edge defines
#define STR_PROPERTY_EDGE_TAIL ":structural:property:edge:tail"
#define STR_PROPERTY_EDGE_HEAD ":structural:property:edge:head"
#define STR_PROPERTY_EDGE_ANGLE ":structural:property:edge:angle"

#define STR_PROPERTY_BIND_ROLE ":structural:property:bind:role"

#define STR_PROPERTY_BINDPARAM_NAME ":structural:property:bindparam:name"
#define STR_PROPERTY_BINDPARAM_VALUE ":structural:property:bindparam:value"

// Interface defines
// nothing...

// Reference defines
#define STR_PROPERTY_REFERENCE_LINK_ID ":structural:property:reference:link:id"
#define STR_PROPERTY_REFERENCE_LINK_UID ":structural:property:reference:link:uid"

#define STR_PROPERTY_REFERENCE_COMPONENT_ID ":structural:property:reference:component:id"
#define STR_PROPERTY_REFERENCE_COMPONENT_UID ":structural:property:reference:component:uid"

#define STR_PROPERTY_REFERENCE_INTERFACE_ID ":structural:property:reference:interface:id"
#define STR_PROPERTY_REFERENCE_INTERFACE_UID ":structural:property:reference:interface:uid"

#define STR_PROPERTY_REFERENCE_XCONNECTOR_ID ":structural:property:reference:xconnector:id"
#define STR_PROPERTY_REFERENCE_XCONNECTOR_UID ":structural:property:reference:xconnector:uid"

//
// Settings
//

#define STR_SETTING_UNDO ":structural:setting:undo"
#define STR_SETTING_UNDO_TRACE ":structural:setting:undo:trace"
#define STR_SETTING_NOTIFY ":structural:setting:notify"
#define STR_SETTING_CODE ":structural:setting:code"

//
// Values
//

#define STR_VALUE_TRUE "1"
#define STR_VALUE_FALSE "0"

//
// Defaults
//

// Entities defines
#define STR_DEFAULT_ANCHOR_W 8
#define STR_DEFAULT_ANCHOR_H 8

// Nodes defines
#define STR_DEFAULT_BODY_W 750
#define STR_DEFAULT_BODY_H 500

#define STR_DEFAULT_CONTEXT_W 250
#define STR_DEFAULT_CONTEXT_H 200

#define STR_DEFAULT_MEDIA_W 48
#define STR_DEFAULT_MEDIA_H 64

#define STR_DEFAULT_LINK_W 14
#define STR_DEFAULT_LINK_H 14

// Interfaces defines
#define STR_DEFAULT_INTERFACE_W 18
#define STR_DEFAULT_INTERFACE_H 18

// Edges defines
// nothing...

// Others defines
#define STR_DEFAULT_WITH_BODY 1                     // not used yet

/*
 *
 * NESTED CONTEXT LANGUAGE
 *
 */

//
// Attributes
//

// Entities defines
#define NCL_ATTRIBUTE_ID "id"
#define NCL_ATTRIBUTE_REFER "refer"
#define NCL_ATTRIBUTE_SRC "src"
#define NCL_ATTRIBUTE_INSTANCE "instance"
#define NCL_ATTRIBUTE_TYPE "type"
#define NCL_ATTRIBUTE_DESCRIPTOR "descriptor"
#define NCL_ATTRIBUTE_COMPONENT "component"
#define NCL_ATTRIBUTE_INTERFACE "interface"
#define NCL_ATTRIBUTE_NAME "name"
#define NCL_ATTRIBUTE_VALUE "value"
#define NCL_ATTRIBUTE_EXTERNABLE "externable"
#define NCL_ATTRIBUTE_XCONNECTOR "xconnector"
#define NCL_ATTRIBUTE_ROLE "role"
#define NCL_ATTRIBUTE_COORDS "coords"
#define NCL_ATTRIBUTE_BEGIN "begin"
#define NCL_ATTRIBUTE_END "end"
#define NCL_ATTRIBUTE_BEGINTEXT "beginText"
#define NCL_ATTRIBUTE_ENDTEXT "endText"
#define NCL_ATTRIBUTE_BEGINPOSITION "beginPosition"
#define NCL_ATTRIBUTE_ENDPOSITION "endPosition"
#define NCL_ATTRIBUTE_FIRST "first"
#define NCL_ATTRIBUTE_LAST "last"
#define NCL_ATTRIBUTE_LABEL "label"
#define NCL_ATTRIBUTE_CLIP "clip"

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
