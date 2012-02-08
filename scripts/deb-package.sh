#!/bin/bash
set -e

debuild -us -uc
debuild -S -us -uc
pbuilder build ../composer_0.1.0.dsc
