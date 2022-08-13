

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

try_fix = False

def check_file(path):
    with open(path,mode="r",encoding="utf-8") as fs:
        lines = fs.readlines()

        if len(lines) <= 7:
            comment_line = lines[0:]
            rest_lines = []
        else:
            comment_line = lines[0:7]
            rest_lines = lines[7:]

        n = os.path.basename(path)

        left_space = 30 - math.floor(len(n) / 2)
        right_space = 30 - math.ceil(len(n) / 2)
        first_line =  "//===" + '-' * left_space + " " + n + " " + "-" * right_space + "===//\n"
        last_line = "//===" + '-' * (len(first_line) - 1 - 10) + "===//\n"

        comments =  \
"""//   Copyright (C) 2021-2022 mingmoe(me@kawayi.moe)(https://blog.kawayi.moe)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU Affero General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License
"""

        comments = comments.splitlines(keepends=True)

        all_lines = [first_line]
        all_lines.extend(comments)
        all_lines.append(last_line)
        
        for it in difflib.unified_diff(
            a = comment_line,
            b = all_lines,
            fromfile=path,
            tofile=path
        ):
            if try_fix:
                fs.truncate(0)
                fs.write(all_lines)
                fs.wrtie('\n'.join(rest_lines))
            else:
                error=True
                print(it,end="")

        pass


for d in directories:
    for file in glob.iglob(d, recursive=True):
        if os.path.isfile(file) and (file.endswith(".cpp") or file.endswith(".hpp")):
            check_file(file)

if error:
    exit(1)

