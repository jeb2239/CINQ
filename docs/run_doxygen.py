#!/usr/bin/env python
import os
from os.path import isfile
from subprocess import call
import shutil

def commentReqs(filename):
    fp=open(filename,'r')
    modLines=[]
    lines=fp.readlines()
    for line in lines:
        if "requires" not in line.strip().split(' ')[0]:
            modLines.append(line)
        elif '///' in line or '//' in line:
            modLines.append(line)
        else:
            modLines.append("///@requires"+line)
    
    os.chdir('./src/')
    nfd=open(os.path.basename(filename), "w")
    nfd.writelines(modLines)
    os.chdir('../')

def copyDoxy(filename):
    fp=open(filename,'r')
    modLines=[]
    lines=fp.readlines()
    for line in lines:
            modLines.append(line)
    os.chdir('docs')
    nfd=open(filename,"w")
    nfd.writelines(modLines)
    os.chdir('../')

def eachSrc():
    try:
        os.mkdir('src')
    except(OSError):
        pass

    filenames=[]

    for (dirpath, dirnames, dirfiles) in os.walk('../src/'):
        for dirfile in dirfiles:
            filenames.append(os.path.join(dirpath, dirfile))
        break

    for name in filenames:
       # if 'Doxyfile' in name : copyDoxy(name)
        if '.cpp' in name: commentReqs(name)
        if '.hpp' in name: commentReqs(name)

    call("doxygen")
    
    shutil.rmtree('src')

eachSrc()
