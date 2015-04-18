#!/usr/bin/env python
import os
from os.path import isfile
from subprocess import call

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

    os.chdir('docs')
    nfd=open(filename,"w")
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
     os.mkdir('docs')
    except(OSError):
        pass
    
    filenames=[]

    for (dirpath, dirnames, f) in os.walk('./'):
        filenames.append(f)
        break

    for name in filenames[0]:
       # if 'Doxyfile' in name : copyDoxy(name)
        if '.cpp' in name: commentReqs(name)
        if '.hpp' in name: commentReqs(name)
    os.chdir('docs')


    call("doxygen")

eachSrc()