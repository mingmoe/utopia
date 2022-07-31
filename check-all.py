
import subprocess

subprocess.run(["python","check-format.py","--style","file","-r","./utopia-client","./utopia-core","./utopia-server"],check=True)
subprocess.run(["python","check-header.py"],check=True)
