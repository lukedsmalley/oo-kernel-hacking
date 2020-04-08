# use python3
import shlex
import subprocess
import sys


command = "dotnet build %s" % sys.argv[1]
arg = shlex.split(command)
subprocess.call(arg)

build_path = input("")
output_path = input("")

command = "dotnet-ildasm %s -f -o %s" % (build_path, output_path)
arg = shlex.split(command)
subprocess.call(arg)
