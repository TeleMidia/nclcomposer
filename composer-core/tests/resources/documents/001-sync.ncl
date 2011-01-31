<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- Exemplo de sincronismo sem a interacao do usuario -->
<ncl id="syncEx" xmlns="http://www.ncl.org.br/NCL3.0/EDTVProfile">
	<head>
		<regionBase>
			<region id="backgroundReg" width="100%" height="100%" zIndex="1"/>
			<region id="screenReg" width="100%" height="100%" zIndex="2">
				<region id="frameReg" left="5%" top="6.7%" width="18.5%" height="18.5%" zIndex="3"/>
			</region>
		</regionBase>
		<descriptorBase>
			<descriptor id="backgroundDesc" region="backgroundReg"/>
			<descriptor id="screenDesc" region="screenReg"/>
			<descriptor id="photoDesc" region="frameReg" explicitDur="5s"/>
			<descriptor id="audioDesc"/>
			<descriptor id="dribleDesc" region="frameReg"/>			
		</descriptorBase>
		<connectorBase>
			<importBase documentURI="../causalConnBase.ncl" alias="conEx"/>
		</connectorBase>
	</head>

	<body>
		<port id="entry" component="animation"/>
		<media id="background" src="../media/background.png" descriptor="backgroundDesc"/>
		<media id="animation" src="../media/animGar.mp4" descriptor="screenDesc">
			<area id="segDrible" begin="11.5s"/>
			<area id="segPhoto" begin="41s"/>
		</media>
		<media id="choro" src="../media/choro.mp3" descriptor="audioDesc"/>
		<media id="drible" src="../media/drible.mp4" descriptor="dribleDesc"/>
		<media id="photo" src="../media/photo.png" descriptor="photoDesc"/>
		<link  id="lMusic" xconnector="conEx#onBeginStartDelay">
			<bind role="onBegin" component="animation"/>
			<bind role="start" component="background">			
				<bindParam name="delay" value="5s"/>
			</bind>				
			<bind role="start" component="choro">
				<bindParam name="delay" value="5s"/>
			</bind>				
		</link>
		<link  id="lDrible" xconnector="conEx#onBeginStart">
			<bind role="onBegin" component="animation" interface="segDrible"/>
			<bind role="start" component="drible"/>			
		</link>	
		<link  id="lPhoto" xconnector="conEx#onBeginStart">
			<bind role="onBegin" component="animation" interface="segPhoto"/>
			<bind role="start" component="photo"/>			
		</link>	
		<link  id="lEnd" xconnector="conEx#onEndStop">
			<bind role="onEnd" component="animation"/>
			<bind role="stop" component="choro"/>			
			<bind role="stop" component="background"/>
		</link>
	</body>
</ncl>