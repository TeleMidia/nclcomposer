<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- Exemplo de uso de multiplos dispositivos em classe passiva -->
<ncl id="passiveDevicesEx" xmlns="http://www.ncl.org.br/NCL3.0/EDTVProfile">
	<head>
		<regionBase>
			<region id="screenReg" width="100%" height="100%" zIndex="1">
				<region id="frameReg" left="5%" top="6.7%" width="18.5%" height="18.5%" zIndex="2"/>
				<region id="secIconReg" left="87.5%" top="11.7%" width="8.45%" height="6.7%" zIndex="2"/>
			</region>
		</regionBase>
		<regionBase device="systemScreen(1)">
			<region id="backgroundReg" width="100%" height="100%" zIndex="1">
				<region id="iconReg" width="100%" height="100%" zIndex="2"/> 
				<region id="shoesReg" left="5%" top="30%" width="40%" height="40%" zIndex="2"/>
				<region id="chutReg" left="50%" top="5%" width="45%" height="90%" zIndex="2"/>
			</region>
		</regionBase>

		<descriptorBase>
			<descriptor id="backgroundDesc" region="backgroundReg"/>
			<descriptor id="screenDesc" region="screenReg"/>
			<descriptor id="photoDesc" region="frameReg" explicitDur="3s"/>
			<descriptor id="audioDesc"/>
			<descriptor id="dribleDesc" region="frameReg"/>
			<descriptor id="iconDesc" region="iconReg" explicitDur="6s" focusIndex="1"/>
			<descriptor id="secIconDesc" region="secIconReg" explicitDur="6s"/>
			<descriptor id="shoesDesc" region="shoesReg"/>
			<descriptor id="chutDesc" region="chutReg" focusIndex="2"/>
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
		<media id="choro" src="../media/choro.mp3" descriptor="audioDesc"/>
		<media id="drible" src="../media/drible.mp4" descriptor="dribleDesc"/>
		<media id="photo" src="../media/photo.png" descriptor="photoDesc"/>
		<media id="secIcon" src="../media/secIcon.png" descriptor="secIconDesc"/>
		<context id="advert">
			<port id="pIcon" component="icon"/>
			<media id="icon" src="../media/iconPassive.png" descriptor="iconDesc"/>
			<media id="background" src="../media/backgroundPassive.png" descriptor="backgroundDesc"/>
			<media id="shoes" src="../media/shoes.mp4" descriptor="shoesDesc"/>
			<media id="chuteira" src="../media/chut.png" descriptor="chutDesc"/> 
			<link id="lBegingAdvert" xconnector="conEx#onSelectionStopStart"> 
				<bind role="onSelection" component="icon"/>
				<bind role="stop" component="icon"/>
				<bind role="start" component="background"/>
				<bind role="start" component="chuteira"/>
				<bind role="start" component="shoes"/>
			</link>	
		</context>
		<link id="lMusic" xconnector="conEx#onBeginStartDelay">
			<bind role="onBegin" component="animation"/>
			<bind role="start" component="choro">
				<bindParam name="delay" value="5s"/>
			</bind>				
		</link>
		<link id="lDrible" xconnector="conEx#onBeginStart">
			<bind role="onBegin" component="animation" 	interface="segDrible"/>
			<bind role="start" component="drible"/>
		</link>	
		<link id="lPhoto" xconnector="conEx#onBeginStart">
			<bind role="onBegin" component="animation" interface="segPhoto"/>
			<bind role="start" component="photo"/>
		</link>	
		<link id="lEnd" xconnector="conEx#onEndStop">
			<bind role="onEnd" component="animation"/>
			<bind role="stop" component="advert"/>
			<bind role="stop" component="choro"/>
		</link>
		<link id="lIcon" xconnector="conEx#onBeginStart">
			<bind role="onBegin" component="animation" interface="segIcon"/>
			<bind role="start" component="secIcon"/>
			<bind role="start" component="advert" interface="pIcon"/>
		</link>	
	</body>
</ncl>
