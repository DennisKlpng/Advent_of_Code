import shutil
import sys
import os
from jinja2 import FileSystemLoader, Environment

if __name__ == '__main__':
    day, ending = sys.argv[1].split(".")
    tgt_filename = f"AOC_{day}.{ending}"
    test_filename = f"inputs/Test_{day}.txt"
    data_filename = f"inputs/Data_{day}.txt"

    # generate empty cpp / py for the day if not existing
    if not os.path.exists(tgt_filename):
        env = Environment(loader=FileSystemLoader(searchpath="templates/"))
        template = None
        if ending == "cpp":
            template = env.get_template("cpp.template")
        elif ending == "py":
            template = env.get_template("python.template")
        file_content = template.render(test_file=test_filename, data_file=data_filename)
        f = open(tgt_filename, "w")
        f.write(file_content)
        f.close()

    # create empty files for test / puzzle input (if not existing yet)
    if not os.path.exists("inputs"):
        os.mkdir("inputs")
    open(test_filename, "a").close()
    open(data_filename, "a").close()

    # delete executables (from cpp-compiling)
    for item in [x for x in os.listdir(".") if x.endswith(".exe")]:
        os.remove(item)
    
    # delete steps-folder from visu gen
    shutil.rmtree(os.getcwd() + "/steps")
