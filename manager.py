# Copyright 2026 The Phi Project Authors
# Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
#
# Date created: 04-03-2026
# Original author: Zevi Berlin (zeviberlin@gmail.com)
# Latest editor: Zevi Berlin (zeviberlin@gmail.com)

import urllib3
import time
import os
import subprocess

#---------> [ Config. Separator ] <---------#

def build(debug: bool = False):
  print("cmake -S . -B _build -DCMAKE_BUILD_TYPE=" + ("Debug" if debug else "Release"))
  print(subprocess.check_output("cmake -S . -B _build -DCMAKE_BUILD_TYPE=" + ("Debug" if debug else "Release"), shell=True).decode())

  print("cmake --build _build")
  print(subprocess.check_output("cmake --build _build", shell=True).decode())

  print("mv _build/khuzdoor ./ " + ("&& dsymutil ./khuzdoor" if debug else ""))
  print(subprocess.check_output("mv _build/khuzdoor ./ " + ("&& dsymutil ./khuzdoor" if debug else ""), shell=True).decode())

#------------[ Func. Implementation Separator ]------------#

def format():
  print("find src -type f \\( -name \"*.c\" -o -name \"*.cpp\" \\) -exec clang-format -i {} +")
  print(subprocess.check_output("find src -type f \\( -name \"*.c\" -o -name \"*.cpp\" \\) -exec clang-format -i {} +", shell=True).decode())
  print("find include -type f \\( -name \"*.h\" -o -name \"*.hpp\" \\) -exec clang-format -i {} +")
  print(subprocess.check_output("find include -type f \\( -name \"*.h\" -o -name \"*.hpp\" \\) -exec clang-format -i {} +", shell=True).decode())

#------------[ Func. Implementation Separator ]------------#

def analyze():
  print("find src -type f \\( -name \"*.c\" -o -name \"*.cpp\" \\) -exec clang-tidy --config-file=./.clang-tidy --format-style=file -p _build {} +")
  print(subprocess.check_output("find src -type f \\( -name \"*.c\" -o -name \"*.cpp\" \\) -exec clang-tidy --config-file=./.clang-tidy --format-style=file -p _build {} +", shell=True).decode())

#------------[ Func. Implementation Separator ]------------#

def cleanup():
  print("mv _build/compile_commands.json ./")
  print(subprocess.check_output("mv _build/compile_commands.json ./", shell=True).decode())
  print("rm -rf _build/ && mkdir _build")
  print(subprocess.check_output("rm -rf _build/ && mkdir _build", shell=True).decode())
  print("mv ./compile_commands.json _build/")
  print(subprocess.check_output("mv ./compile_commands.json _build/", shell=True).decode())
  print("rm -rf khuzdoor khuzdoor.dSYM .cache")
  print(subprocess.check_output("rm -rf khuzdoor khuzdoor.dSYM .cache", shell=True).decode())

#------------[ Func. Implementation Separator ]------------#

def templateCppFilePair(header_path: str, source_path: str, author_name: str, author_email: str) -> None:
  date: str = time.strftime("%d-%m-%Y")

  comment: str = f"""// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: {date}
// Original author: {author_name} ({author_email})
// Latest editor: {author_name} ({author_email})
"""
  

  with open(header_path, "w") as header:
    header.write(comment)

    include_name: str = header_path.split("/")[-1].replace(".", "_").upper()
    header.write(f"\n#ifndef {include_name}\n#define {include_name}\n\n\n#endif /* {include_name} */")

  with open(source_path, "w") as source:
    source.write(comment)
    source.write(f"\n#include \"{header_path}\"\n\n")

#------------[ Func. Implementation Separator ]------------#

def installManPage(install_dir: str):
  resp = urllib3.request("GET", "https://raw.githubusercontent.com/the-phi-project/khuzdoor/refs/heads/main/documentation/khuzdoor.1")
  
  path: str = os.path.join(install_dir, "khuzdoor.1")
  try:
    with open(path, "w") as file:
      file.write(resp.data.decode())
  except FileNotFoundError:
    print(f"[ERROR] Could not write to the file `{path}` because the file or directory does not exist")
    return
  except PermissionError:
    print(f"[ERROR] Could not write to the file `{path} due to lack of sufficient permissions")
    return
  
  print("SUCCESS!")

#
#------------[ Func. Implementation Separator ]------------#
#

def main():
  selected: int = 0

  while True:
    given = input("      > ")
    try:
      selected = int(given)
      break
    except ValueError:
      pass
    print(f"Invalid input `{given}` - please select an option from above")
  
  match (selected):
    case 1:
      build(False) # release mode
    case 2:
      build(True) # debug mode
    case 3:
      format()
    case 4:
      analyze()
    case 5:
      cleanup()
    case 6:
      filename: str = input("Filename (including extension): ")
      header_path: str = input("Base path for header (e.g encryption/symmetric/): ")
      name: str = input("Your name: ")
      email: str = input("Your email: ")
      templateCppFilePair(os.path.join("include/", header_path, filename), os.path.join("src/", filename), name, email)
    case 7:
      path: str = input("Path in which to install khuzdoor.1 manpage (default is your CWD)? ")
      path = os.getcwd() if path == "" else path
      installManPage(path)
    case _:
      print(f"Invalid input `{selected}` - please select an option from above")
      main()

if __name__ == "__main__":
  print("""Khuzdoor Project Utilities Python Script
    1) Build (Release)
    2) Build (Debug)
    3) Format with clang-format
    4) Analyze with clang-tidy
    5) Cleanup project directory
  ------
    6) Create Header/Source File Pair
    7) Install manpage
""")
  
  main()