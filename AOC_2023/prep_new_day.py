import shutil
import sys
import os
import subprocess
from jinja2 import FileSystemLoader, Environment
import json

#usage: 
if __name__ == '__main__':
    day = sys.argv[1]
    try:
        int(day)
    except ValueError:
        print(f"The input {day} was not a valid number for the day")
    tgt_filename = f"aoc_{day}/Cargo.toml"
    test_filename = f"Test_{day}.txt"
    data_filename = f"Data_{day}.txt"

    # generate new cargo
    cargo_projectname = f"aoc_{day}"
    if not os.path.exists(tgt_filename):
        subprocess.run(["cargo", "new", cargo_projectname], shell=True)

    # fill main.rs with basic code
    env = Environment(loader=FileSystemLoader(searchpath="../misc/templates/"))
    template = env.get_template("rs.template")
    file_content = template.render(test_file=test_filename, data_file=data_filename)
    f = open(f"aoc_{day}/src/main.rs", "w")
    f.write(file_content)
    f.close()

    # Update Cargo.toml
    dont_update = False
    with open(f"aoc_{day}/Cargo.toml", "r") as fr:
        if 'rust_utils = {path = "../../rust_utils"}' in fr.read():
            dont_update = True
    if not dont_update:
        with open(f"aoc_{day}/Cargo.toml", "a") as f:
            f.write('rust_utils = {path = "../../rust_utils"}')
    
    #Update settings.json
    if os.path.exists("../.vscode/settings.json"):
        with open("../.vscode/settings.json", "r") as f:
            file_content = json.load(f)
            links = file_content["rust-analyzer.linkedProjects"]
            links.append(f"AOC_2023/aoc_{day}/Cargo.toml")
            links = set(links)
            file_content["rust-analyzer.linkedProjects"] = list(links)
        with open("../.vscode/settings.json", "w") as f:
            json.dump(file_content, f, indent=4)


    # create empty files for test / puzzle input (if not existing yet)
    if not os.path.exists("inputs"):
        os.mkdir("inputs")
    open(test_filename, "a").close()
    open(data_filename, "a").close()

    # delete executables (from cpp-compiling)
    for item in [x for x in os.listdir(".") if x.endswith(".exe")]:
        os.remove(item)
    
    # # delete steps-folder from visu gen
    # if(os.path.isdir(os.getcwd() + "/steps")):
    #     shutil.rmtree(os.getcwd() + "/steps")
