#!/usr/bin/python3.5

import sys
import runpy

sys.argv.append('serve')
runpy.run_module('mkdocs', run_name="__main__", alter_sys=True)

