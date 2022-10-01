
import subprocess
import sys
import os

fix = False

args = sys.argv[1:]
index = 0

while index != len(args):
    arg = args[index]

    if arg == "--fix":
        fix = True
    else:
        print(f"unknown argument:{arg}")
        exit(1)
    
    index += 1

root = os.path.dirname(os.path.realpath(__file__))

format_args = ["python","check-format.py","--style","file","-r",f"{root}/utopia-client",f"{root}/utopia-core",f"{root}/utopia-server"]

if fix:
    format_args.append("--in-place")

subprocess.run(format_args,check=True)
subprocess.run(["python","check-header.py"],check=True)
