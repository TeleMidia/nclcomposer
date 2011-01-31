<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- $Id: main.ncl,v 1.3 2009/01/16 15:49:25 root Exp $ -->
<!--
    main.ncl
    14jan2009, gflima@telemidia.puc-rio.br

    Jogo da velha (tic-tac-toe) em NCL
  -->
<ncl id="velha" xmlns="http://www.ncl.org.br/NCL3.0/EDTVProfile">
  <head>
    <regionBase>
      <region id="rgScreen" width="62.5%" height="92.7%"
              top="2.5%" left="18.75%" zIndex="0">

        <!-- messages and options -->
        <region id="rgTitle" width="100%" height="18.24%"
                top="0" left="0" zIndex="2"/>
        <region id="rgPlayAgain" width="50%" height="15.82%"
                top="84.17%" left="0" zIndex="2"/>
        <region id="rgExit" width="50%" height="15.82%"
                top="84.17%" left="50%" zIndex="2"/>
        <region id="rgMessages" width="75%" height="65.93%"
                top="18.24%" left="12.5%" zIndex="4"/>

        <!-- board -->
        <region id="rgGrid" width="75%" height="65.93%"
                top="18.24%" left="12.5%" zIndex="2">
          <!-- 1st line -->
          <region id="rgPos0" width="31.66%" height="31.66%"
                  top="0" left="0" zIndex="4"/>
          <region id="rgPos1" width="31.66%" height="31.66%"
                  top="0" left="34%" zIndex="4"/>
          <region id="rgPos2" width="31.66%" height="31.66%"
                  top="0" left="68%" zIndex="4"/>
          <!-- 2nd line -->
          <region id="rgPos3" width="31.66%" height="31.66%"
                  top="34%" left="0" zIndex="4"/>
          <region id="rgPos4" width="31.66%" height="31.66%"
                  top="34%" left="34%" zIndex="4"/>
          <region id="rgPos5" width="31.66%" height="31.66%"
                  top="34%" left="68%" zIndex="4"/>
          <!-- 3rd line -->
          <region id="rgPos6" width="31.66%" height="31.66%"
                  top="68%" left="0" zIndex="4"/>
          <region id="rgPos7" width="31.66%" height="31.66%"
                  top="68%" left="34%" zIndex="4"/>
          <region id="rgPos8" width="31.66%" height="31.66%"
                  top="68%" left="68%" zIndex="4"/>
        </region>
      </region>
    </regionBase>

    <descriptorBase>
      <descriptor id="dTitle" region="rgTitle"/>
      <descriptor id="dMessages" region="rgMessages"/>

      <descriptor id="dPlayAgain" region="rgPlayAgain"
                  focusIndex="ixPlayAgain"
                  moveUp="ixExit" moveDown="ixExit"
                  moveRight="ixExit" moveLeft="ixExit"
                  focusSrc="media/play_again_on.png"
                  focusBorderWidth="0"/>

      <descriptor id="dExit" region="rgExit"
                  focusIndex="ixExit"
                  moveUp="ixPlayAgain" moveDown="ixPlayAgain"
                  moveRight="ixPlayAgain" moveLeft="ixPlayAgain"
                  focusSrc="media/exit_on.png"
                  focusBorderWidth="0"/>

      <descriptor id="dGrid" region="rgGrid"/>
      <!-- 1st line -->
      <descriptor id="dPos0" region="rgPos0" focusIndex="0"
                  moveUp="6" moveDown="3" moveRight="1" moveLeft="2"
                  focusBorderWidth="3" focusBorderColor="yellow"
                  selBorderColor="red" />
      <descriptor id="dPos1" region="rgPos1" focusIndex="1"
                  moveUp="7" moveDown="4" moveRight="2" moveLeft="0"
                  focusBorderWidth="3" focusBorderColor="yellow"
                  selBorderColor="red" />
      <descriptor id="dPos2" region="rgPos2" focusIndex="2"
                  moveUp="8" moveDown="5" moveRight="0" moveLeft="1"
                  focusBorderWidth="3" focusBorderColor="yellow"
                  selBorderColor="red" />
      <!-- 2nd line -->
      <descriptor id="dPos3" region="rgPos3" focusIndex="3"
                  moveUp="0" moveDown="6" moveRight="4" moveLeft="5"
                  focusBorderWidth="3" focusBorderColor="yellow"
                  selBorderColor="red"/>
      <descriptor id="dPos4" region="rgPos4" focusIndex="4"
                  moveUp="1" moveDown="7" moveRight="5" moveLeft="3"
                  focusBorderWidth="3" focusBorderColor="yellow"
                  selBorderColor="red"/>
      <descriptor id="dPos5" region="rgPos5" focusIndex="5"
                  moveUp="2" moveDown="8" moveRight="3" moveLeft="4"
                  focusBorderWidth="3" focusBorderColor="yellow"
                  selBorderColor="red"/>
      <!-- 3rd line -->
      <descriptor id="dPos6" region="rgPos6" focusIndex="6"
                  moveUp="3" moveDown="0" moveRight="7" moveLeft="8"
                  focusBorderWidth="3" focusBorderColor="yellow"
                  selBorderColor="red"/>
      <descriptor id="dPos7" region="rgPos7" focusIndex="7"
                  moveUp="4" moveDown="1" moveRight="8" moveLeft="6"
                  focusBorderWidth="3" focusBorderColor="yellow"
                  selBorderColor="red"/>
      <descriptor id="dPos8" region="rgPos8" focusIndex="8"
                  moveUp="5" moveDown="2" moveRight="6" moveLeft="7"
                  focusBorderWidth="3" focusBorderColor="yellow"
                  selBorderColor="red"/>
    </descriptorBase>

    <ruleBase>

      <!-- x wins -->
      <compositeRule id="rXWin" operator="or">
        <!-- x wins line -->
        <compositeRule id="rXWinLine" operator="or">
          <compositeRule id="rXWinLine0" operator="and">
            <rule id="rXWinLine0_pos0" var="_pos0" comparator="eq"
                  value="x"/>
            <rule id="rXWinLine0_pos1" var="_pos1" comparator="eq"
                  value="x"/>
            <rule id="rXWinLine0_pos2" var="_pos2" comparator="eq"
                  value="x"/>
          </compositeRule>
          <compositeRule id="rXWinLine1" operator="and">
            <rule id="rXWinLine1_pos3" var="_pos3" comparator="eq"
                  value="x"/>
            <rule id="rXWinLine1_pos4" var="_pos4" comparator="eq"
                  value="x"/>
            <rule id="rXWinLine1_pos5" var="_pos5" comparator="eq"
                  value="x"/>
          </compositeRule>
          <compositeRule id="rXWinLine2" operator="and">
            <rule id="rXWinLine2_pos6" var="_pos6" comparator="eq"
                  value="x"/>
            <rule id="rXWinLine2_pos7" var="_pos7" comparator="eq"
                  value="x"/>
            <rule id="rXWinLine2_pos8" var="_pos8" comparator="eq"
                  value="x"/>
          </compositeRule>
        </compositeRule>
        <!-- x wins column -->
        <compositeRule id="rXWinCol" operator="or">
          <compositeRule id="rXWinCol0" operator="and">
            <rule id="rXWinCol0_pos0" var="_pos0" comparator="eq"
                  value="x"/>
            <rule id="rXWinCol0_pos3" var="_pos3" comparator="eq"
                  value="x"/>
            <rule id="rXWinCol0_pos6" var="_pos6" comparator="eq"
                  value="x"/>
          </compositeRule>
          <compositeRule id="rXWinCol1" operator="and">
            <rule id="rXWinCol1_pos1" var="_pos1" comparator="eq"
                  value="x"/>
            <rule id="rXWinCol1_pos4" var="_pos4" comparator="eq"
                  value="x"/>
            <rule id="rXWinCol1_pos7" var="_pos7" comparator="eq"
                  value="x"/>
          </compositeRule>
          <compositeRule id="rXWinCol2" operator="and">
            <rule id="rXWinCol2_pos2" var="_pos2" comparator="eq"
                  value="x"/>
            <rule id="rXWinCol2_pos5" var="_pos5" comparator="eq"
                  value="x"/>
            <rule id="rXWinCol2_pos8" var="_pos8" comparator="eq"
                  value="x"/>
          </compositeRule>
        </compositeRule>
        <!-- x wins diagonal -->
        <compositeRule id="rXWinDia" operator="or">
          <compositeRule id="rXWinDia0" operator="and">
            <rule id="rXWinDia0_pos0" var="_pos0" comparator="eq"
                  value="x"/>
            <rule id="rXWinDia0_pos4" var="_pos4" comparator="eq"
                  value="x"/>
            <rule id="rXWinDia0_pos8" var="_pos8" comparator="eq"
                  value="x"/>
          </compositeRule>
          <compositeRule id="rXWinDia1" operator="and">
            <rule id="rXWinDia1_pos2" var="_pos2" comparator="eq"
                  value="x"/>
            <rule id="rXWinDia1_pos4" var="_pos4" comparator="eq"
                  value="x"/>
            <rule id="rXWinDia1_pos6" var="_pos6" comparator="eq"
                  value="x"/>
          </compositeRule>
        </compositeRule>
      </compositeRule>

      <!-- o wins -->
      <compositeRule id="rOWin" operator="or">
        <!-- o wins line -->
        <compositeRule id="rOWinLine" operator="or">
          <compositeRule id="rOWinLine0" operator="and">
            <rule id="rOWinLine0_pos0" var="_pos0" comparator="eq"
                  value="o"/>
            <rule id="rOWinLine0_pos1" var="_pos1" comparator="eq"
                  value="o"/>
            <rule id="rOWinLine0_pos2" var="_pos2" comparator="eq"
                  value="o"/>
          </compositeRule>
          <compositeRule id="rOWinLine1" operator="and">
            <rule id="rOWinLine1_pos3" var="_pos3" comparator="eq"
                  value="o"/>
            <rule id="rOWinLine1_pos4" var="_pos4" comparator="eq"
                  value="o"/>
            <rule id="rOWinLine1_pos5" var="_pos5" comparator="eq"
                  value="o"/>
          </compositeRule>
          <compositeRule id="rOWinLine2" operator="and">
            <rule id="rOWinLine2_pos6" var="_pos6" comparator="eq"
                  value="o"/>
            <rule id="rOWinLine2_pos7" var="_pos7" comparator="eq"
                  value="o"/>
            <rule id="rOWinLine2_pos8" var="_pos8" comparator="eq"
                  value="o"/>
          </compositeRule>
        </compositeRule>
        <!-- o wins column -->
        <compositeRule id="rOWinCol" operator="or">
          <compositeRule id="rOWinCol0" operator="and">
            <rule id="rOWinCol0_pos0" var="_pos0" comparator="eq"
                  value="o"/>
            <rule id="rOWinCol0_pos3" var="_pos3" comparator="eq"
                  value="o"/>
            <rule id="rOWinCol0_pos6" var="_pos6" comparator="eq"
                  value="o"/>
          </compositeRule>
          <compositeRule id="rOWinCol1" operator="and">
            <rule id="rOWinCol1_pos1" var="_pos1" comparator="eq"
                  value="o"/>
            <rule id="rOWinCol1_pos4" var="_pos4" comparator="eq"
                  value="o"/>
            <rule id="rOWinCol1_pos7" var="_pos7" comparator="eq"
                  value="o"/>
          </compositeRule>
          <compositeRule id="rOWinCol2" operator="and">
            <rule id="rOWinCol2_pos2" var="_pos2" comparator="eq"
                  value="o"/>
            <rule id="rOWinCol2_pos5" var="_pos5" comparator="eq"
                  value="o"/>
            <rule id="rOWinCol2_pos8" var="_pos8" comparator="eq"
                  value="o"/>
          </compositeRule>
        </compositeRule>
        <!-- o wins diagonal -->
        <compositeRule id="rOWinDia" operator="or">
          <compositeRule id="rOWinDia0" operator="and">
            <rule id="rOWinDia0_pos0" var="_pos0" comparator="eq"
                  value="o"/>
            <rule id="rOWinDia0_pos4" var="_pos4" comparator="eq"
                  value="o"/>
            <rule id="rOWinDia0_pos8" var="_pos8" comparator="eq"
                  value="o"/>
          </compositeRule>
          <compositeRule id="rOWinDia1" operator="and">
            <rule id="rOWinDia1_pos2" var="_pos2" comparator="eq"
                  value="o"/>
            <rule id="rOWinDia1_pos4" var="_pos4" comparator="eq"
                  value="o"/>
            <rule id="rOWinDia1_pos6" var="_pos6" comparator="eq"
                  value="o"/>
          </compositeRule>
        </compositeRule>
      </compositeRule>

      <!-- tie, no more moves and
           x (always makes the last move) loses -->
      <compositeRule id="rTie" operator="and">
        <compositeRule id="rTieNoMoves" operator="and">
          <rule id="rTie0" var="_pos0" comparator="ne" value="empty"/>
          <rule id="rTie1" var="_pos1" comparator="ne" value="empty"/>
          <rule id="rTie2" var="_pos2" comparator="ne" value="empty"/>
          <rule id="rTie3" var="_pos3" comparator="ne" value="empty"/>
          <rule id="rTie4" var="_pos4" comparator="ne" value="empty"/>
          <rule id="rTie5" var="_pos5" comparator="ne" value="empty"/>
          <rule id="rTie6" var="_pos6" comparator="ne" value="empty"/>
          <rule id="rTie7" var="_pos7" comparator="ne" value="empty"/>
          <rule id="rTie8" var="_pos8" comparator="ne" value="empty"/>
        </compositeRule>

        <!-- TODO: there must be a better way to do this... -->
        <!-- x lose -->
        <compositeRule id="rXLose" operator="and">
          <!-- x lose line -->
          <compositeRule id="rXLoseLine" operator="and">
            <compositeRule id="rXLoseLine0" operator="or">
              <rule id="rXLoseLine0_pos0" var="_pos0" comparator="ne"
                    value="x"/>
              <rule id="rXLoseLine0_pos1" var="_pos1" comparator="ne"
                    value="x"/>
              <rule id="rXLoseLine0_pos2" var="_pos2" comparator="ne"
                    value="x"/>
            </compositeRule>
            <compositeRule id="rXLoseLine1" operator="or">
              <rule id="rXLoseLine1_pos3" var="_pos3" comparator="ne"
                    value="x"/>
              <rule id="rXLoseLine1_pos4" var="_pos4" comparator="ne"
                    value="x"/>
              <rule id="rXLoseLine1_pos5" var="_pos5" comparator="ne"
                    value="x"/>
            </compositeRule>
            <compositeRule id="rXLoseLine2" operator="or">
              <rule id="rXLoseLine2_pos6" var="_pos6" comparator="ne"
                    value="x"/>
              <rule id="rXLoseLine2_pos7" var="_pos7" comparator="ne"
                    value="x"/>
              <rule id="rXLoseLine2_pos8" var="_pos8" comparator="ne"
                    value="x"/>
            </compositeRule>
          </compositeRule>
          <!-- x lose col -->
          <compositeRule id="rXLoseCol" operator="and">
            <compositeRule id="rXLoseCol0" operator="or">
              <rule id="rXLoseCol0_pos0" var="_pos0" comparator="ne"
                    value="x"/>
              <rule id="rXLoseCol0_pos3" var="_pos3" comparator="ne"
                    value="x"/>
              <rule id="rXLoseCol0_pos6" var="_pos6" comparator="ne"
                    value="x"/>
            </compositeRule>
            <compositeRule id="rXLoseCol1" operator="or">
              <rule id="rXLoseCol1_pos1" var="_pos1" comparator="ne"
                    value="x"/>
              <rule id="rXLoseCol1_pos4" var="_pos4" comparator="ne"
                    value="x"/>
              <rule id="rXLoseCol1_pos7" var="_pos7" comparator="ne"
                    value="x"/>
            </compositeRule>
            <compositeRule id="rXLoseCol2" operator="or">
              <rule id="rXLoseCol2_pos2" var="_pos2" comparator="ne"
                    value="x"/>
              <rule id="rXLoseCol2_pos5" var="_pos5" comparator="ne"
                    value="x"/>
              <rule id="rXLoseCol2_pos8" var="_pos8" comparator="ne"
                    value="x"/>
            </compositeRule>
          </compositeRule>
          <!-- x lose diagonal -->
          <compositeRule id="RXLoseDia" operator="and">
            <compositeRule id="RXLoseDia1" operator="or">
              <rule id="rXLoseDia1_pos0" var="_pos0" comparator="ne"
                    value="x" />
              <rule id="rXLoseDia1_pos4" var="_pos4" comparator="ne"
                    value="x" />
              <rule id="rXLoseDia1_pos8" var="_pos8" comparator="ne"
                    value="x" />
            </compositeRule>
            <compositeRule id="RXLoseDia2" operator="or">
              <rule id="rXLoseDia1_pos2" var="_pos2" comparator="ne"
                    value="x" />
              <rule id="rXLoseDia1_pos4" var="_pos4" comparator="ne"
                    value="x" />
              <rule id="rXLoseDia1_pos6" var="_pos6" comparator="ne"
                    value="x" />
            </compositeRule>
          </compositeRule>
        </compositeRule>
      </compositeRule>
    </ruleBase>

    <connectorBase>
      <importBase alias="conn" documentURI="base.conn"/>
    </connectorBase>
  </head>


  <body>
    <port id="pStart" component="board"/>

    <!-- red: quit -->
    <link xconnector="conn#onKeySelectionAbort">
      <bind component="board" interface="pExit" role="onSelection"/>
      <bind component="board" interface="pExit" role="onKeySelection">
        <bindParam name="key" value="RED"/>
      </bind>
      <bind component="board" role="abort"/>
    </link>

    <!-- green: restart game -->
    <link xconnector="conn#onKeySelectionStopStart">
      <bind component="board" interface="pPlayAgain"
            role="onSelection"/>
      <bind component="board" interface="pPlayAgain"
            role="onKeySelection">
        <bindParam name="key" value="GREEN"/>
      </bind>
      <bind component="board" role="stop"/>
      <bind component="board" role="start"/>
    </link>

    <!-- board -->
    <context id="board">
      <port id="pTitle" component="title"/>
      <port id="pExit" component="exit"/>
      <port id="pPlayAgain" component="play_again"/>
      <port id="pGrid" component="grid"/>
      <port id="pPos0" component="empty0"/>
      <port id="pPos1" component="empty1"/>
      <port id="pPos2" component="empty2"/>
      <port id="pPos3" component="empty3"/>
      <port id="pPos4" component="empty4"/>
      <port id="pPos5" component="empty5"/>
      <port id="pPos6" component="empty6"/>
      <port id="pPos7" component="empty7"/>
      <port id="pPos8" component="empty8"/>

      <media id="exit" src="media/exit.png" descriptor="dExit"/>
      <media id="play_again" src="media/play_again.png"
             descriptor="dPlayAgain"/>
      <media id="title" src="media/title.png" descriptor="dTitle"/>

      <!-- empty board -->
      <media id="grid" src="media/grid.png" descriptor="dGrid"/>
      <media id="empty0" src="media/empty.png" descriptor="dPos0"/>
      <media id="empty1" src="media/empty.png" descriptor="dPos1"/>
      <media id="empty2" src="media/empty.png" descriptor="dPos2"/>
      <media id="empty3" src="media/empty.png" descriptor="dPos3"/>
      <media id="empty4" src="media/empty.png" descriptor="dPos4"/>
      <media id="empty5" src="media/empty.png" descriptor="dPos5"/>
      <media id="empty6" src="media/empty.png" descriptor="dPos6"/>
      <media id="empty7" src="media/empty.png" descriptor="dPos7"/>
      <media id="empty8" src="media/empty.png" descriptor="dPos8"/>

      <!-- symbols -->
      <media id="x" src="media/x.png"/>
      <media id="o" src="media/o.png"/>

      <!-- settings -->
      <media id="noSettings" type="application/x-ginga-settings">
        <property name="service.currentFocus" value="0"/>
        <property name="_pos0" value="empty"/>
        <property name="_pos1" value="empty"/>
        <property name="_pos2" value="empty"/>
        <property name="_pos3" value="empty"/>
        <property name="_pos4" value="empty"/>
        <property name="_pos5" value="empty"/>
        <property name="_pos6" value="empty"/>
        <property name="_pos7" value="empty"/>
        <property name="_pos8" value="empty"/>

        <!-- x plays first -->
        <property name="turn" value="x"/>
      </media>

      <!-- search for a winner after each move -->
      <switch id="search_winner">

        <!-- x wins -->
        <bindRule rule="rXWin" constituent="xwin"/>
        <media id="xwin" src="media/xwin.png" descriptor="dMessages"/>

        <!-- o wins -->
        <bindRule rule="rOWin" constituent="owin"/>
        <media id="owin" src="media/owin.png" descriptor="dMessages"/>

        <!-- tie -->
        <bindRule rule="rTie" constituent="tie"/>
        <media id="tie"  src="media/tie.png" descriptor="dMessages"/>
      </switch>

      <!-- LINKS -->
      <!-- context initialization -->
      <link xconnector="conn#onBeginSet">
        <bind component="empty0" role="onBegin"/>
        <bind component="noSettings" interface="service.currentFocus"
              role="set">
          <bindParam name="val" value="0"/>
        </bind>
        <bind component="noSettings" interface="_pos0" role="set">
          <bindParam name="val" value="empty"/>
        </bind>
        <bind component="noSettings" interface="_pos1" role="set">
          <bindParam name="val" value="empty"/>
        </bind>
        <bind component="noSettings" interface="_pos2" role="set">
          <bindParam name="val" value="empty"/>
        </bind>
        <bind component="noSettings" interface="_pos3" role="set">
          <bindParam name="val" value="empty"/>
        </bind>
        <bind component="noSettings" interface="_pos4" role="set">
          <bindParam name="val" value="empty"/>
        </bind>
        <bind component="noSettings" interface="_pos5" role="set">
          <bindParam name="val" value="empty"/>
        </bind>
        <bind component="noSettings" interface="_pos6" role="set">
          <bindParam name="val" value="empty"/>
        </bind>
        <bind component="noSettings" interface="_pos7" role="set">
          <bindParam name="val" value="empty"/>
        </bind>
        <bind component="noSettings" interface="_pos8" role="set">
          <bindParam name="val" value="empty"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="x"/>
        </bind>
      </link>

      <!-- game over, change focus to play_again -->
      <link xconnector="conn#onBeginSet">
        <bind component="search_winner" role="onBegin"/>
        <bind component="noSettings" interface="service.currentFocus"
              role="set">
          <bindParam name="val" value="ixPlayAgain"/>
        </bind>
      </link>

      <!-- pos0 > x -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty0" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="x"/>
        </bind>

        <bind component="noSettings" interface="_pos0" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="x" descriptor="dPos0" role="start"/>
        <bind component="search_winner" role="start"/>
        <bind component="empty0" descriptor="dPos0" role="stop"/>
      </link>

      <!-- pos0 > o -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty0" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="o"/>
        </bind>

        <bind component="noSettings" interface="_pos0" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="o" descriptor="dPos0" role="start"/>
       <bind component="search_winner" role="start"/>
        <bind component="empty0" descriptor="dPos0" role="stop"/>
      </link>

      <!-- pos1 > x -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty1" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="x"/>
        </bind>

        <bind component="noSettings" interface="_pos1" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="x" descriptor="dPos1" role="start"/>
        <bind component="search_winner" role="start"/>
        <bind component="empty1" descriptor="dPos1" role="stop"/>
      </link>

      <!-- pos1 > o -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty1" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="o"/>
        </bind>

        <bind component="noSettings" interface="_pos1" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="o" descriptor="dPos1" role="start"/>
       <bind component="search_winner" role="start"/>
        <bind component="empty1" descriptor="dPos1" role="stop"/>
      </link>

      <!-- pos2 > x -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty2" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="x"/>
        </bind>

        <bind component="noSettings" interface="_pos2" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="x" descriptor="dPos2" role="start"/>
        <bind component="search_winner" role="start"/>
        <bind component="empty2" descriptor="dPos2" role="stop"/>
      </link>

      <!-- pos2 > o -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty2" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="o"/>
        </bind>

        <bind component="noSettings" interface="_pos2" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="o" descriptor="dPos2" role="start"/>
       <bind component="search_winner" role="start"/>
        <bind component="empty2" descriptor="dPos2" role="stop"/>
      </link>

      <!-- pos3 > x -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty3" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="x"/>
        </bind>

        <bind component="noSettings" interface="_pos3" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="x" descriptor="dPos3" role="start"/>
        <bind component="search_winner" role="start"/>
        <bind component="empty3" descriptor="dPos3" role="stop"/>
      </link>

      <!-- pos3 > o -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty3" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="o"/>
        </bind>

        <bind component="noSettings" interface="_pos3" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="o" descriptor="dPos3" role="start"/>
       <bind component="search_winner" role="start"/>
        <bind component="empty3" descriptor="dPos3" role="stop"/>
      </link>

      <!-- pos4 > x -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty4" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="x"/>
        </bind>

        <bind component="noSettings" interface="_pos4" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="x" descriptor="dPos4" role="start"/>
        <bind component="search_winner" role="start"/>
        <bind component="empty4" descriptor="dPos4" role="stop"/>
      </link>

      <!-- pos4 > o -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty4" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="o"/>
        </bind>

        <bind component="noSettings" interface="_pos4" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="o" descriptor="dPos4" role="start"/>
       <bind component="search_winner" role="start"/>
        <bind component="empty4" descriptor="dPos4" role="stop"/>
      </link>

      <!-- pos5 > x -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty5" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="x"/>
        </bind>

        <bind component="noSettings" interface="_pos5" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="x" descriptor="dPos5" role="start"/>
        <bind component="search_winner" role="start"/>
        <bind component="empty5" descriptor="dPos5" role="stop"/>
      </link>

      <!-- pos5 > o -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty5" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="o"/>
        </bind>

        <bind component="noSettings" interface="_pos5" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="o" descriptor="dPos5" role="start"/>
       <bind component="search_winner" role="start"/>
        <bind component="empty5" descriptor="dPos5" role="stop"/>
      </link>

      <!-- pos6 > x -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty6" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="x"/>
        </bind>

        <bind component="noSettings" interface="_pos6" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="x" descriptor="dPos6" role="start"/>
        <bind component="search_winner" role="start"/>
        <bind component="empty6" descriptor="dPos6" role="stop"/>
      </link>

      <!-- pos6 > o -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty6" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="o"/>
        </bind>

        <bind component="noSettings" interface="_pos6" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="o" descriptor="dPos6" role="start"/>
       <bind component="search_winner" role="start"/>
        <bind component="empty6" descriptor="dPos6" role="stop"/>
      </link>

      <!-- pos7 > x -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty7" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="x"/>
        </bind>

        <bind component="noSettings" interface="_pos7" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="x" descriptor="dPos7" role="start"/>
        <bind component="search_winner" role="start"/>
        <bind component="empty7" descriptor="dPos7" role="stop"/>
      </link>

      <!-- pos7 > o -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty7" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="o"/>
        </bind>

        <bind component="noSettings" interface="_pos7" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="o" descriptor="dPos7" role="start"/>
       <bind component="search_winner" role="start"/>
        <bind component="empty7" descriptor="dPos7" role="stop"/>
      </link>

      <!-- pos8 > x -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty8" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="x"/>
        </bind>

        <bind component="noSettings" interface="_pos8" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="x" descriptor="dPos8" role="start"/>
        <bind component="search_winner" role="start"/>
        <bind component="empty8" descriptor="dPos8" role="stop"/>
      </link>

      <!-- pos8 > o -->
      <link xconnector="conn#onKeySelectionPropertyTestStopSetStart">
        <bind component="empty8" role="onSelection"/>
        <bind component="noSettings" interface="turn" role="propertyTest">
          <bindParam name="val" value="o"/>
        </bind>

        <bind component="noSettings" interface="_pos8" role="set">
          <bindParam name="val" value="o"/>
        </bind>
        <bind component="noSettings" interface="turn" role="set">
          <bindParam name="val" value="x"/>
        </bind>
        <bind component="o" descriptor="dPos8" role="start"/>
       <bind component="search_winner" role="start"/>
        <bind component="empty8" descriptor="dPos8" role="stop"/>
      </link>

    </context>
  </body>
</ncl>
