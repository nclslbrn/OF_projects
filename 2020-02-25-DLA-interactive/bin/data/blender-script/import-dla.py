
import bpy  # noqa

import sys


D = bpy.data
C = bpy.context


# Argument from command line
# blender --python import-dla.py -- <plyFilePath> <treeSize>

if "--" in sys.argv:
    args = sys.argv[sys.argv.index("--") + 1:]

    if args[0]:
        plyPath = args[0]

    if args[1]:
        treeSize = args[1]


print("we want to import " + plyPath ", wich has " + treeSize + " vertices.")
