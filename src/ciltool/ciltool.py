# use python3
import re
import shlex
import subprocess
import sys


# remove_comments function from https://stackoverflow.com/questions/2319019/using-regex-to-remove-comments-from-source-files
# posted by user msanders Feb 23, 2010
def remove_comments(text):
    text = re.sub(re.compile("/\*.*?\*/", re.DOTALL), "", text) # remove all streamed comments (/*COMMENT */) from string
    text = re.sub(re.compile("//.*?\n" ), "", text) # remove all single-line comments (//COMMENT\n ) from string
    return text
  

command = "dotnet build %s" % sys.argv[1]
arg = shlex.split(command)
subprocess.call(arg)

build_path = input("Build File Path: ")
output_path = input("Output File Path: ")

command = "dotnet-ildasm %s -f -o %s" % (build_path, output_path)
arg = shlex.split(command)
subprocess.call(arg)

translated_path = input("\nTranslated File Path: ")


with open(output_path) as il_file, open(translated_path, "w") as tr_file:
    for line in il_file:
       tr_file.write(remove_comments(line))
