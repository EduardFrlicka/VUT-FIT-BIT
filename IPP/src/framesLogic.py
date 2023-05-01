import sys
import xml.etree.ElementTree as et
import re
from classes import cVar, cVariable
import helpFunc


currFrames = {"GF": {}}
localFrames = []


def upateLF():
    if len(localFrames) == 0:
        currFrames.pop("LF")
        return
    newLF = localFrames[len(localFrames)-1]
    currFrames.update({"LF": newLF})


def createFrame():
    currFrames.update({"TF": {}})


def popFrame():
    lf = currFrames.get("LF")
    if lf == None:
        helpFunc.error_exit(55, "Pop none LF")
    currFrames.update({"TF": lf})
    localFrames.pop()
    upateLF()


def pushFrame():
    tf = currFrames.get("TF")
    if tf == None:
        helpFunc.error_exit(55, "Pop none TF")
    localFrames.append(tf)
    currFrames.pop("TF")
    upateLF()


def printFrames():
    print(f"{'='*20}", file=sys.stderr)
    for key, frame in currFrames.items():
        print(f"{key}: {frame}", file=sys.stderr)
    print(f"Stack: {localFrames}", file=sys.stderr)


# def frameExists(frame):
#     pass


def setVarValue(var: cVar, value):
    global currFrames
    getVar(var).setValue(value)
    return


def getVar(var: cVar):
    frame = currFrames.get(var.frame)
    if frame == None:
        helpFunc.error_exit(55, f"Variable ({var}) does not exists")
    v = frame.get(var.name)
    if v == None:
        helpFunc.error_exit(54, f"Variable ({var}) does not exists")
    return v


def getVarValue(var: cVar):
    return getVar(var)()


def createVar(var: cVar):
    global currFrames
    if currFrames.get(var.frame) == None:
        helpFunc.error_exit(55, f"DEFVAR: Frame ({var.frame}) does not exists")
    currFrames[var.frame][var.name] = cVariable()
    return


def varExists(var: cVar):
    return var.name in currFrames.get(var.frame, {}).keys()
