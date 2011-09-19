# maint.mk -- composer maintainer's makefile.

ME:= maint.mk
MAKEME:= $(MAKE) -f $(ME)

TARGET:= Composer

DMG:= $(TARGET).dmg
BUNDLE:= $(TARGET).app

PLUGINS:= /Library/Application\ Support/Composer

.PHONY: all
all:
	@echo "usage: make -f maint.mk [macx-deploy]"
	@exit 0

.PHONY: macx-deploy
macx-deploy:
	mkdir $(BUNDLE)/Contents/PlugIns
	mkdir $(BUNDLE)/Contents/PlugIns/composer
	
	cp $(PLUGINS)/* $(BUNDLE)/Contents/PlugIns/composer

	./macx-plugins-deploy.sh
	
	macdeployqt $(BUNDLE) -dmg
