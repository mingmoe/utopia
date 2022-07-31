

import difflib
from genericpath import isfile
from posixpath import abspath
import sys
import os
import glob
import math

root = os.path.dirname(os.path.realpath(__file__))

directories = ["/utopia-core/**","/utopia-client/**","/utopia-server/**"]

directories = map(lambda x: os.path.abspath(root + x),directories)

error = False

def check_file(path):
    with open(path,mode="r",encoding="utf-8") as fs:
        lines = fs.readlines()

        if len(lines) <= 7:
            comment_line = lines[0:]
        else:
            comment_line = lines[0:7]

        n = os.path.basename(path)

        left_space = 26 - math.floor(len(n) / 2)
        right_space = 26 - math.ceil(len(n) / 2)
        first_line =  "//===" + '-' * left_space + " " + n + " " + "-" * right_space + "===//\n"
        
        for it in difflib.unified_diff(
            a = comment_line,
            b = [
                first_line,
                "//\n",
                "// this file is under the MIT License\n",
                "// See https://opensource.org/licenses/MIT for license information.\n",
                "// Copyright(c) 2020-2022 moe-org All rights reserved.\n",
                "//\n",
                "//===" + '-' * (len(first_line) - 1 - 10) + "===//\n"],
            fromfile=path,
            tofile=path
        ):
            error=True
            print(it,end="")

        pass


for d in directories:
    for file in glob.iglob(d, recursive=True):
        if os.path.isfile(file) and (file.endswith(".cpp") or file.endswith(".hpp")):
            check_file(file)

if error:
    exit(1)

