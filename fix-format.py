
import sys
import os
import glob
import subprocess

root = os.path.dirname(os.path.realpath(__file__))

directories = ["/utopia-core/**","/utopia-client/**","/utopia-server/**"]

directories = map(lambda x: os.path.abspath(root + x),directories)



for d in directories:
    for file in glob.iglob(d, recursive=True):
        if os.path.isfile(file) and (file.endswith(".cpp") or file.endswith(".hpp")):
            print(f"format {file}...")
            subprocess.run(["clang-format","-i","--style=file",file],check=True,cwd=root,encoding='utf-8')


