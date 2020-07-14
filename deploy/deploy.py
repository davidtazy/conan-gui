#!/usr/bin/env python
# -*- coding: utf-8 -*-
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

from __future__ import print_function
import os
import json
import sys
from distutils.dir_util import copy_tree
import glob
import shutil

def get_dirs():
    data = json.load(open("conanbuildinfo.json"))
    dep_lib_dirs = dict()
    dep_bin_dirs = dict()
    for dep in data["dependencies"]:
        root = dep["rootpath"]
        lib_paths = dep["lib_paths"]
        bin_paths = dep["bin_paths"]
        for lib_path in lib_paths:
            if os.listdir(lib_path):
                 lib_dir = os.path.relpath(lib_path, root)
                 dep_lib_dirs[lib_path] = lib_dir
        for bin_path in bin_paths:
            if os.listdir(bin_path):
                bin_dir = os.path.relpath(bin_path, root)
                dep_bin_dirs[bin_path] = bin_dir
    return dep_lib_dirs, dep_bin_dirs

def find_qt_dep(json_data):
    for dep in json_data["dependencies"]:
        if dep["name"] == 'qt':
            return dep
    raise ValueError("no qt dep in json")



def get_qt_dirs(json_dir):
    data = json.load(open(os.path.join(json_dir,"conanbuildinfo.json")))
    
    qt_dep = find_qt_dep(data)
    qt_bin = qt_dep['bin_paths'][0]
    
    pattern = os.path.join(qt_bin,"*.dll")
    dlls = glob.glob(pattern)
    dll_files = dlls

    pattern = os.path.join(qt_bin,"*.so")
    dlls = glob.glob(pattern)
    dll_files.extend(dlls)

    qt_root = qt_dep['rootpath']
    plugins_dir = os.path.join(qt_root,'plugins')
    
    return dll_files,plugins_dir




def main(argv):
    json_dir = argv[1]
    destination = argv[2]

    dll_files,plugins_dir = get_qt_dirs(json_dir)

    print("copy qt plugin directory")
    copy_tree(plugins_dir,os.path.join(destination,'plugins'))

    print("copy qt shared libraries")
    for dll in dll_files:
        shutil.copy(dll,destination)
    
    print("create qt.conf")
    qt_conf = os.path.join(destination,'qt.conf')
    f = open(qt_conf, "w")
    f.write("[Paths]\nPrefix = .")
    f.close()


    print("done")
    

if __name__ == '__main__':
    main(sys.argv)