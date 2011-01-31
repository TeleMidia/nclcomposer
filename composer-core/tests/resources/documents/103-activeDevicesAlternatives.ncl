<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- Exemplo de uso de múltiplos dispositivos em classe ativa com opção de uso apenas  do dispositivo base-->
<ncl id="activeDevicesAlternatives" xmlns="http://www.ncl.org.br/NCL3.0/EDTVProfile">
	<head>
		<ruleBase>
			<rule id="single" var="system.devNumber(2)" value="0" comparator="eq"/>
		</ruleBase>
		<regionBase>
			<region id="screenReg" width="100%" height="100%" zIndex="1">
				<region id="frameReg" left="5%" top="6.7%" width="18.5%" height="18.5%" zIndex="2"/>
				<region id="secIconReg" left="87.5%" top="11.7%" width="8.45%" height="6.7%" zIndex="2"/>
				<region id="NCLAdvertSingleReg" left="5%" top="5%" width="28.125%" height="88.888%" zIndex="2"/>
			</region>
		</regionBase>
		<regionBase device="systemScreen(2)"> 
			<region id="NCLAdvertMultiReg" width="100%" height="100%" zIndex="1"/>
		</regionBase>

		<descriptorBase>
			<descriptor id="screenDesc" region="screenReg"/>
			<descriptor id="photoDesc" region="frameReg" explicitDur="5s"/>
			<descriptor id="audioDesc"/>
			<descriptor id="dribleDesc" region="frameReg"/>
			<descriptor id="secIconDesc" region="secIconReg" explicitDur="6s"/>
			<descriptorSwitch id="NCLAdvertDesc">
				<bindRule constituent="NCLAdvertSingleDesc" rule="single"/>
				<defaultDescriptor descriptor="NCLAdvertMultiDesc"/>
				<descriptor id="NCLAdvertSingleDesc" region="NCLAdvertSingleReg" focusIndex="2"/>
				<descriptor id="NCLAdvertMultiDesc" region="NCLAdvertMultiReg"/>
			</descriptorSwitch>
		</descriptorBase>
		<connectorBase>
			<importBase documentURI="../causalConnBase.ncl" alias="conEx"/>
		</connectorBase>
	</head>
	<body>
		<port id="entry" component="animation"/>
		<media id="animation" src="../media/animGar.mp4" descriptor="screenDesc">
			<area id="segDrible" begin="11.5s"/>
			<area id="segPhoto" begin="41s"/>
			<area id="segIcon" begin="45s" end="51s"/>
		</media>
		<media id="globalVar" type="application/x-ginga-settings">
			<property name="service.currentKeyMaster"/>
		</media>	
		<media id="choro" src="../media/choro.mp3" descriptor="audioDesc"/>
		<media id="drible" src="../media/drible.mp4" descriptor="dribleDesc"/>
		<media id="photo" src="../media/photo.png" descriptor="photoDesc"/>
		<media id="secIcon" src="../media/secIcon.png" descriptor="secIconDesc"/>
		<media id="NCLAdvert" src="NCLApplications/advert/advert.ncl" descriptor="NCLAdvertDesc"/>
		<link id="lMusic" xconnector="conEx#onBeginStartDelay">
			<bind role="onBegin" component="animation"/>
			<bind role="start" component="choro">
				<bindParam name="delay" value="5s"/>
			</bind>				
		</link>
		<link id="lDrible" xconnector="conEx#onBeginStart"> 
			<bind role="onBegin" component="animation" interface="segDrible"/>
			<bind role="start" component="drible"/>
		</link>	
		<link id="lPhoto" xconnector="conEx#onBeginStart"> 
			<bind role="onBegin" component="animation" interface="segPhoto"/>
			<bind role="start" component="photo"/>
		</link>	
		<link id="lEnd" xconnector="conEx#onEndStop">
			<bind role="onEnd" component="animation"/>
			<bind role="stop" component="choro"/>
			<bind role="stop" component="NCLAdvert"/>
		</link>
		<link id="lBeginAdvert" xconnector="conEx#onBeginSetStart">
			<bind role="onBegin" component="animation" interface="segIcon"/>
			<bind role="start" component="secIcon"/>
			<bind role="start" component="NCLAdvert"/>
			<bind role="set" component="globalVar" interface="service.currentKeyMaster">
				<bindParam name="var" value="NCLAdvert"/>
			</bind>	
		</link>	
	</body>
</ncl>
