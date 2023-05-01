from typing import Dict, Tuple
import xml. etree.cElementTree as et
import helpFunc
from classes import cInstructionInstance, cInstruction
import instructions
import sys


callStack = []
labels = {}
currInstruction: cInstructionInstance = None
nextInstruction: cInstructionInstance = None
insstructionDict = {}
firstInstruction: cInstructionInstance = None

DEFAULTINSTRUCTION = cInstruction(None, [])
INSTRUCTIONS = {
    "MOVE": cInstruction(instructions.instructionMove, ["var", "symb"]),
    "DEFVAR": cInstruction(instructions.instructionDefvar, ["var"]),
    "CREATEFRAME": cInstruction(instructions.instructionCreateFrame, []),
    "PUSHFRAME": cInstruction(instructions.instructionPushFrame, []),
    "POPFRAME": cInstruction(instructions.instructionPopFrame, []),
    "CALL": cInstruction(instructions.instructionCall, ["label"]),
    "RETURN": cInstruction(instructions.instructionReturn, []),
    "PUSHS": cInstruction(instructions.instructionPushs, ["symb"]),
    "POPS": cInstruction(instructions.instructionPops, ["var"]),
    "ADD": cInstruction(instructions.instructionAdd, ["var", "symb", "symb"]),
    "SUB": cInstruction(instructions.instructionSub, ["var", "symb", "symb"]),
    "MUL": cInstruction(instructions.instructionMul, ["var", "symb", "symb"]),
    "IDIV": cInstruction(instructions.instructionIdiv, ["var", "symb", "symb"]),
    "LT": cInstruction(instructions.instructionLt, ["var", "symb", "symb"]),
    "GT": cInstruction(instructions.instructionGt, ["var", "symb", "symb"]),
    "EQ": cInstruction(instructions.instructionEq, ["var", "symb", "symb"]),
    "AND": cInstruction(instructions.instructionAnd, ["var", "symb", "symb"]),
    "OR": cInstruction(instructions.instructionOr, ["var", "symb", "symb"]),
    "NOT": cInstruction(instructions.instructionNot, ["var", "symb"]),
    "INT2CHAR": cInstruction(instructions.instructionInt2Char, ["var", "symb"]),
    "STRI2INT": cInstruction(instructions.instructionStri2Int, ["var", "symb", "symb"]),
    "READ": cInstruction(instructions.instructionRead, ["var", "type"]),
    "WRITE": cInstruction(instructions.instructionWrite, ["symb"]),
    "CONCAT": cInstruction(instructions.instructionConcat, ["var", "symb", "symb"]),
    "STRLEN": cInstruction(instructions.instructionStrLen, ["var", "symb"]),
    "GETCHAR": cInstruction(instructions.instructionGetChar, ["var", "symb", "symb"]),
    "SETCHAR": cInstruction(instructions.instructionSetChar, ["var", "symb", "symb"]),
    "TYPE": cInstruction(instructions.instructionType, ["var", "symb"]),
    "LABEL": cInstruction(instructions.instructionLabel, ["label"]),
    "JUMP": cInstruction(instructions.instructionJump, ["label"]),
    "JUMPIFEQ": cInstruction(instructions.instructionJumpIfEq, ["label", "symb", "symb"]),
    "JUMPIFNEQ": cInstruction(instructions.instructionJumpIfNeq, ["label", "symb", "symb"]),
    "EXIT": cInstruction(instructions.instructionExit, ["symb"]),
    "DPRINT": cInstruction(instructions.instructionDPrint, ["symb"]),
    "BREAK": cInstruction(instructions.instructionBreak, []),
}


def flowInitialization(program: et.Element):

    global firstInstruction

    if program.attrib.get("language") != "IPPcode22":
        helpFunc.error_exit(32, "flowInitialization: Bad XML, wrong language")

    if len(program) != len(set([helpFunc.getOrder(i) for i in program])):
        helpFunc.error_exit(32, "flowInitialization: Bad XML, duplicit order")

    program[:] = sorted(program, key=helpFunc.getOrder)
    if len(program) == 0:
        return

    opcode, lastInstructionInstance = initializeInstruction(program[0])
    firstInstruction = lastInstructionInstance
    if program[0].tag != "instruction":
            helpFunc.error_exit(
                32, "flowInitialization: Bad XML, wrong instruction tag")
    # print(opcode, file=sys.stderr)
    createLabel(opcode, lastInstructionInstance)

    for instruction in program[1:]:
        if instruction.tag != "instruction":
            helpFunc.error_exit(
                32, "flowInitialization: Bad XML, wrong instruction tag")
        opcode, instructionInstance = initializeInstruction(instruction)
        lastInstructionInstance.setNext(instructionInstance)
        lastInstructionInstance = instructionInstance
        createLabel(opcode, instructionInstance)


    printTree()
    print(labels, file=sys.stderr)


def initializeInstruction(instruction: et.Element) -> Tuple[str, cInstructionInstance]:
    instruction[:] = sorted(instruction, key=lambda child: child.tag)
    opcode = instruction.attrib.get("opcode","None").upper()
    instructionInstance = cInstructionInstance(
        INSTRUCTIONS.get(opcode, DEFAULTINSTRUCTION), instruction)

    return opcode, instructionInstance


def getLabel(label: str) -> cInstructionInstance:
    instruction = labels.get(label)
    if instruction == None:
        helpFunc.error_exit(52,"getLabel: this label does not exist")
    return instruction


def setNextInstruction(toSet: cInstructionInstance = "None"):
    global nextInstruction
    if toSet == "None":
        nextInstruction = currInstruction.next
    else:
        nextInstruction = toSet


def printlen():
    tmp = firstInstruction
    cnt = 0
    while tmp != None:
        cnt += 1
        tmp = tmp.next
    print(cnt, file=sys.stderr)

def printTree():
    tmp = firstInstruction
    while tmp != None:
        print(tmp,file=sys.stderr)
        tmp = tmp.next


def getCurrInstruction() -> cInstructionInstance:
    return currInstruction


def setCurrInstruction():
    global currInstruction
    currInstruction = nextInstruction


def setFirstInstruction():
    global currInstruction
    currInstruction = firstInstruction


def createLabel(opcode: str, instruction: cInstructionInstance):
    if opcode == "LABEL":
        label = instruction.arguments[0].value
        if label in labels:
            helpFunc.error_exit(52,"Label allready exists")
        labels[label] = instruction


def CallLabel(label: str):
    global callStack
    callStack.append(currInstruction.next)
    setNextInstruction(getLabel(label))
    print("call",callStack,file=sys.stderr)


def Return():
    global callStack
    print(callStack,file=sys.stderr)
    if len(callStack) <= 0:
        helpFunc.error_exit(56,"Return: Call Stack empty")
    setNextInstruction(callStack.pop())
