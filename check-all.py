
import subprocess
import os

root = os.path.dirname(os.path.realpath(__file__))

subprocess.run(["python","check-format.py","--style","file","-r",f"{root}/utopia-client",f"{root}/utopia-core",f"{root}/utopia-server"],check=True)
subprocess.run(["python","check-header.py"],check=True)
