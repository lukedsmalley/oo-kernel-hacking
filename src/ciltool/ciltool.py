import shlex
import subprocess
import sys


command = "dotnet build %s" % sys.argv[1]
arg = shlex.split(command)
subprocess.call(arg)

build_path = raw_input("")
output_path = raw_input("")

command = "dotnet-ildasm %s -f -o %s" % (build_path, output_path)
arg = shlex.split(command)
subprocess.call(arg)
