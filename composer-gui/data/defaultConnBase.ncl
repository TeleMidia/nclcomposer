<?xml version="1.0" encoding="ISO-8859-1"?>
<!--

This is NCL
Copyright: 2000-2005 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.
See http://www.telemidia.puc-rio.br

Public URI: http://www.ncl.org.br/NCL3.0/connectorBases/causalConnBase.ncl
Author: TeleMidia Laboratory
Revision: 19/09/2006

-->
<ncl id="causalConnBase" xmlns="http://www.ncl.org.br/NCL3.0/CausalConnectorProfile">

<head>
	<connectorBase>

        <!-- OnBegin -->
        
        <causalConnector id="onBeginStart">
          <simpleCondition role="onBegin"/>
          <simpleAction role="start" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onBeginStart_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onBegin"/>
          <simpleAction role="start" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onBeginStop">
          <simpleCondition role="onBegin"/>
          <simpleAction role="stop" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onBeginStop_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onBegin"/>
          <simpleAction role="stop" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onBeginPause">
          <simpleCondition role="onBegin"/>
          <simpleAction role="pause" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onBeginPause_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onBegin"/>
          <simpleAction role="pause" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onBeginResume">
          <simpleCondition role="onBegin"/>
          <simpleAction role="resume" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onBeginResume_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onBegin"/>
          <simpleAction role="resume" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onBeginSet_var">
          <connectorParam name="var"/>
          <simpleCondition role="onBegin"/>
          <simpleAction role="set" value="$var"/>
        </causalConnector>
        
        <causalConnector id="onBeginSet_var_delay">
          <connectorParam name="var"/>
          <connectorParam name="delay"/>
          <simpleCondition role="onBegin"/>
          <simpleAction role="set" value="$var" delay="$delay"/>
        </causalConnector>
        
        <!-- OnEnd -->
        
        <causalConnector id="onEndStart">
          <simpleCondition role="onEnd"/>
          <simpleAction role="start" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onEndStart_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onEnd"/>
          <simpleAction role="start" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onEndStop">
          <simpleCondition role="onEnd"/>
          <simpleAction role="stop" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onEndStop_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onEnd"/>
          <simpleAction role="stop" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onEndPause">
          <simpleCondition role="onEnd"/>
          <simpleAction role="pause" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onEndPause_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onEnd"/>
          <simpleAction role="pause" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onEndResume">
          <simpleCondition role="onEnd"/>
          <simpleAction role="resume" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onEndResume_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onEnd"/>
          <simpleAction role="resume" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onEndSet_var">
          <connectorParam name="var"/>
          <simpleCondition role="onEnd"/>
          <simpleAction role="set" value="$var"/>
        </causalConnector>
        
        <causalConnector id="onEndSet_var_delay">
          <connectorParam name="var"/>
          <connectorParam name="delay"/>
          <simpleCondition role="onEnd"/>
          <simpleAction role="set" value="$var" delay="$delay"/>
        </causalConnector>
        
        
        <!-- OnMouseSelection -->
        
        <causalConnector id="onSelectionStart">
          <simpleCondition role="onSelection"/>
          <simpleAction role="start" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onSelectionStart_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onSelection"/>
          <simpleAction role="start" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onSelectionStop">
          <simpleCondition role="onSelection"/>
          <simpleAction role="stop" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onSelectionStop_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onSelection"/>
          <simpleAction role="stop" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onSelectionPause">
          <simpleCondition role="onSelection"/>
          <simpleAction role="pause" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onSelectionPause_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onSelection"/>
          <simpleAction role="pause" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onSelectionResume">
          <simpleCondition role="onSelection"/>
          <simpleAction role="resume" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onSelectionResume_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onSelection"/>
          <simpleAction role="resume" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onSelectionSet_var">
          <connectorParam name="var" />
          <simpleCondition role="onSelection"/>
          <simpleAction role="set" value="$var"/>
        </causalConnector>
        
        <causalConnector id="onSelectionStet_var_delay">
          <connectorParam name="var" />
          <connectorParam name="delay"/>
          <simpleCondition role="onSelection"/>
          <simpleAction role="set" value="$var" delay="$delay"/>
        </causalConnector>
        
        <!-- OnKeySelection -->
        
        <causalConnector id="onKeySelectionStart">
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <simpleAction role="start" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStart_delay">
          <connectorParam name="keyCode"/>
          <connectorParam name="delay"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <simpleAction role="start" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStop">
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <simpleAction role="stop" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStop_delay">
          <connectorParam name="keyCode"/>
          <connectorParam name="delay"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <simpleAction role="stop" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onKeySelectionPause">
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <simpleAction role="pause" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onKeySelectionPause_delay">
          <connectorParam name="keyCode"/>
          <connectorParam name="delay"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <simpleAction role="pause" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onKeySelectionResume">
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <simpleAction role="resume" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onKeySelectionResume_delay">
          <connectorParam name="keyCode"/>
          <connectorParam name="delay"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <simpleAction role="resume" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onKeySelectionSet_var">
          <connectorParam name="keyCode"/>
          <connectorParam name="var"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <simpleAction role="set" value="$var"/>
        </causalConnector>
        
        <causalConnector id="onKeySelectionSet_var_delay">
          <connectorParam name="keyCode"/>
          <connectorParam name="var"/>
          <connectorParam name="delay"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <simpleAction role="set" value="$var" delay="$delay"/>
        </causalConnector>
        
        <!-- OnBeginAttribution -->
        
        <causalConnector id="onBeginAttributionStart">
          <simpleCondition role="onBeginAttribution"/>
          <simpleAction role="start" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStart_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onBeginAttribution"/>
          <simpleAction role="start" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStop">
          <simpleCondition role="onBeginAttribution"/>
          <simpleAction role="stop" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStop_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onBeginAttribution"/>
          <simpleAction role="stop" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionPause">
          <simpleCondition role="onBeginAttribution"/>
          <simpleAction role="pause" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionPause_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onBeginAttribution"/>
          <simpleAction role="pause" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionResume">
          <simpleCondition role="onBeginAttribution"/>
          <simpleAction role="resume" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionResume_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onBeginAttribution"/>
          <simpleAction role="resume" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionSet_var">
          <connectorParam name="var"/>
          <simpleCondition role="onBeginAttribution"/>
          <simpleAction role="set" value="$var"/>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionSet_var_delay">
          <connectorParam name="var"/>
          <connectorParam name="delay"/>
          <simpleCondition role="onBeginAttribution"/>
          <simpleAction role="set" value="$var" delay="$delay"/>
        </causalConnector>
        
        <!-- OnEndAttribution -->
        
        <causalConnector id="onEndAttributionStart">
          <simpleCondition role="onEndAttribution"/>
          <simpleAction role="start" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStart_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onEndAttribution"/>
          <simpleAction role="start" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStop">
          <simpleCondition role="onEndAttribution"/>
          <simpleAction role="stop" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStop_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onEndAttribution"/>
          <simpleAction role="stop" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onEndAttributionPause">
          <simpleCondition role="onEndAttribution"/>
          <simpleAction role="pause" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onEndAttributionPause_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onEndAttribution"/>
          <simpleAction role="pause" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onEndAttributionResume">
          <simpleCondition role="onEndAttribution"/>
          <simpleAction role="resume" max="unbounded"/>
        </causalConnector>
        
        <causalConnector id="onEndAttributionResume_delay">
          <connectorParam name="delay"/>
          <simpleCondition role="onEndAttribution"/>
          <simpleAction role="resume" max="unbounded" delay="$delay"/>
        </causalConnector>
        
        <causalConnector id="onEndAttributionSet_var">
          <connectorParam name="var"/>
          <simpleCondition role="onEnd"/>
          <simpleAction role="set" value="$var"/>
        </causalConnector>
        
        <causalConnector id="onEndAttributionSet_var_delay">
          <connectorParam name="var"/>
          <connectorParam name="delay"/>
          <simpleCondition role="onEndAttribution"/>
          <simpleAction role="set" value="$var" delay="$delay"/>
        </causalConnector>
        
        
        <!-- OnBegin multiple actions -->
        
        <causalConnector id="onBeginStartStop">
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" />
             <simpleAction role="stop" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStart_delay1Stop_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="start" delay="$delay1" max="unbounded" />
             <simpleAction role="stop" delay="$delay2" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStartPause">
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" />
             <simpleAction role="pause" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStart_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="start" delay="$delay1" max="unbounded" />
             <simpleAction role="pause" delay="$delay2" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStartResume">
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" />
             <simpleAction role="resume" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStart_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="start" delay="$delay1" max="unbounded" />
             <simpleAction role="resume" delay="$delay2" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStartSet">
          <connectorParam name="var"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" />
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStart_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="start" delay="$delay1" max="unbounded" />
             <simpleAction role="set" value="$var" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStopStart">
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" />
             <simpleAction role="start" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStop_delay1Start_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" delay="$delay1" max="unbounded" />
             <simpleAction role="start" delay="$delay2" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStopPause">
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" />
             <simpleAction role="pause" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStop_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" delay="$delay1" max="unbounded" />
             <simpleAction role="pause" delay="$delay2" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStopResume">
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" />
             <simpleAction role="resume" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStop_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" delay="$delay1" max="unbounded" />
             <simpleAction role="resume" delay="$delay2" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStopSet">
          <connectorParam name="var"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" />
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginStop_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" delay="$delay1" max="unbounded" />
             <simpleAction role="set" delay="$delay2" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginSetStart">
          <connectorParam name="var"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="start" max="unbounded" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginSet_delay1Start_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="start" delay="$delay2" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginSetStop">
          <connectorParam name="var"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginSet_delay1Stop_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="stop" delay="$delay2" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginSetPause">
          <connectorParam name="var"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginSet_delay1Pause_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="pause" delay="$delay2" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginSetResume">
          <connectorParam name="var"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginSet_delay1Resume_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBegin"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="resume" delay="$delay2" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        
        <!-- OnEnd multiple actions -->
        
        <causalConnector id="onEndStartStop">
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStart_delay1Stop_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="stop" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStartPause">
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStart_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStartResume">
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStart_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStartSet">
          <connectorParam name="var"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStart_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="set" value="$var" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStopStart">
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStop_delay1Start_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="start" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStopPause">
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStop_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStopResume">
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStop_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStopSet">
          <connectorParam name="var"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndStop_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="set" delay="$delay2" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndSetStart">
          <connectorParam name="var"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndSet_delay1Start_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="start" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndSetStop">
          <connectorParam name="var"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndSet_delay1Stop_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="stop" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndSetPause">
          <connectorParam name="var"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndSet_delay1Pause_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndSetResume">
          <connectorParam name="var"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndSet_delay1Resume_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEnd"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        
        <!-- OnMouseSelection multiple actions -->
        
        <causalConnector id="onSelectionStartStop">
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStart_delay1Stop_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="stop" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStartPause">
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStart_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStartResume">
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStart_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStartSet">
          <connectorParam name="var"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStart_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="set" value="$var" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStopStart">
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStop_delay1Start_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="start" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStopPause">
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStop_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStopResume">
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStop_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStopSet">
          <connectorParam name="var"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionStop_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="set" delay="$delay2" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionSetStart">
          <connectorParam name="var"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionSet_delay1Start_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="start" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionSetStop">
          <connectorParam name="var"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionSet_delay1Stop_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="stop" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionSetPause">
          <connectorParam name="var"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionSet_delay1Pause_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionSetResume">
          <connectorParam name="var"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onSelectionSet_delay1Resume_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onSelection"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        
        <!-- OnKeySelection multiple actions -->
        
        <causalConnector id="onKeySelectionStartStop">
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStart_delay1Stop_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="stop" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStartPause">
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStart_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStartResume">
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStart_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStartSet">
          <connectorParam name="var"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStart_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="set" value="$var" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStopStart">
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStop_delay1Start_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="start" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStopPause">
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStop_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStopResume">
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStop_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStopSet">
          <connectorParam name="var"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionStop_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="set" delay="$delay2" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionSetStart">
          <connectorParam name="var"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionSet_delay1Start_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="start" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionSetStop">
          <connectorParam name="var"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionSet_delay1Stop_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="stop" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionSetPause">
          <connectorParam name="var"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionSet_delay1Pause_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionSetResume">
          <connectorParam name="var"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelectionSet_delay1Resume_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1" />
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        
        <!-- OnBeginAttribution multiple actions -->
        
        
        <causalConnector id="onBeginAttributionStartStop">
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStart_delay1Stop_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="stop" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStartPause">
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStart_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStartResume">
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStart_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStartSet">
          <connectorParam name="var"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStart_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="set" value="$var" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStopStart">
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStop_delay1Start_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="start" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStopPause">
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStop_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStopResume">
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStop_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStopSet">
          <connectorParam name="var"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionStop_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="set" delay="$delay2" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionSetStart">
          <connectorParam name="var"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionSet_delay1Start_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1"/>
             <simpleAction role="start" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionSetStop">
          <connectorParam name="var"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionSet_delay1Stop_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1"/>
             <simpleAction role="stop" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionSetPause">
          <connectorParam name="var"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionSet_delay1Pause_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2" />
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionSetResume">
          <connectorParam name="var"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onBeginAttributionSet_delay1Resume_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onBeginAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        
        <!-- OnEndAttribution multiple actions -->
        
        <causalConnector id="onEndAttributionStartStop">
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStart_delay1Stop_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="stop" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStartPause">
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStart_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStartResume">
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStart_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStartSet">
          <connectorParam name="var"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStart_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="start" max="unbounded" delay="$delay1"/>
             <simpleAction role="set" value="$var" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStopStart">
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStop_delay1Start_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="start" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStopPause">
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStop_delay1Pause_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStopResume">
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStop_delay1Resume_delay2">
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStopSet">
          <connectorParam name="var"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionStop_delay1Set_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded" delay="$delay1"/>
             <simpleAction role="set" delay="$delay2" value="$var"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionSetStart">
          <connectorParam name="var"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionSet_delay1Start_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1"/>
             <simpleAction role="start" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionSetStop">
          <connectorParam name="var"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="stop" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionSet_delay1Stop_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1"/>
             <simpleAction role="stop" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionSetPause">
          <connectorParam name="var"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="pause" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionSet_delay1Pause_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1"/>
             <simpleAction role="pause" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionSetResume">
          <connectorParam name="var"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var"/>
             <simpleAction role="resume" max="unbounded"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onEndAttributionSet_delay1Resume_delay2">
          <connectorParam name="var"/>
          <connectorParam name="delay1"/>
          <connectorParam name="delay2"/>
          <simpleCondition role="onEndAttribution"/>
          <compoundAction operator="seq">
             <simpleAction role="set" value="$var" delay="$delay1"/>
             <simpleAction role="resume" max="unbounded" delay="$delay2"/>
          </compoundAction>
        </causalConnector>
        
        
        <!--Miscellaneous-->
        
        <causalConnector id="onKeySelectionStopSet_varPauseStart">
          <connectorParam name="var"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode"/>
          <compoundAction operator="seq">
             <simpleAction role="stop"/>
             <simpleAction role="set" value="$var"/>
             <simpleAction role="pause"/>
             <simpleAction role="start"/>
          </compoundAction>
        </causalConnector>
        
        <causalConnector id="onKeySelection_orStopSet_varPauseStart">
          <connectorParam name="var"/>
          <connectorParam name="keyCode"/>
          <simpleCondition role="onSelection" key="$keyCode" max="unbounded"/>
          <compoundAction operator="seq">
             <simpleAction role="stop" max="unbounded"/>
             <simpleAction role="set" value="$var"/>
             <simpleAction role="pause" max="unbounded"/>
             <simpleAction role="start" max="unbounded"/>
          </compoundAction>
        </causalConnector>
	
    </connectorBase>
</head>
</ncl>

