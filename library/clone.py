#!/usr/bin/python3

import subprocess

subprocess.run(['git','submodule','update','--init','--recursive'],check=True)
