import sys
from framesLogic import varExists, setVarValue, createFrame, createVar, pushFrame, popFrame, cVar, printFrames, getVar
from classes import cInstruction
import helpFunc


def setInputFile(file=None):
    global inputFile
    if file:
        inputFile = open(file, "r")
    else:
        inputFile = sys.stdin


def instructionDefvar(arguments: list):
    var = arguments[0].value
    if varExists(var):
        helpFunc.error_exit(52, f"DEFVAR: Variable ({var}) allready exists")
    createVar(var)
    pass


def instructionMove(arguments: list):
    setVarValue(arguments[0].value, arguments[1]())
    pass


def instructionCreateFrame(arguments: list):
    createFrame()
    pass


def instructionPushFrame(arguments: list):
    pushFrame()
    pass


def instructionPopFrame(arguments: list):
    popFrame()
    pass


def instructionCall(arguments: list):
    from flowLogic import CallLabel
    CallLabel(arguments[0]())
    pass


def instructionReturn(arguments: list):
    from flowLogic import Return
    Return()
    pass


def instructionPushs(arguments: list):
    from stackLogic import stackPush
    stackPush(arguments[0]())
    pass


def instructionPops(arguments: list):
    from stackLogic import stackPop
    setVarValue(arguments[0].value, stackPop())
    pass


def instructionAdd(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) or type(arguments[2]()) != int):
        helpFunc.error_exit(53, "Add: symb1 or symb2 is not type int")
    setVarValue(arguments[0].value, arguments[1]()+arguments[2]())
    pass


def instructionSub(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) or type(arguments[2]()) != int):
        helpFunc.error_exit(53, "Sub: symb1 or symb2 is not type int")
    setVarValue(arguments[0].value, arguments[1]()-arguments[2]())
    pass


def instructionMul(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) or type(arguments[2]()) != int):
        helpFunc.error_exit(53, "Mul: symb1 or symb2 is not type int")

    setVarValue(arguments[0].value, arguments[1]()*arguments[2]())
    pass


def instructionIdiv(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) or type(arguments[2]()) != int):
        helpFunc.error_exit(53, "Div: symb1 or symb2 is not type int")
    if(arguments[2]() == 0):
        helpFunc.error_exit(57, "Div: Zero division")

    setVarValue(arguments[0].value, arguments[1]()//arguments[2]())
    pass


def instructionLt(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) or type(arguments[1]()) == type(None)):
        helpFunc.error_exit(53, "Lt: wrong type")
    setVarValue(arguments[0].value, arguments[1]() < arguments[2]())
    pass


def instructionGt(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) or type(arguments[1]()) == type(None)):
        helpFunc.error_exit(53, "Gt: wrong type")
    setVarValue(arguments[0].value, arguments[1]() > arguments[2]())
    pass


def instructionEq(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) and (type(arguments[1]()) != type(None) and type(arguments[2]()) != type(None))):
        helpFunc.error_exit(53, "Eq: wrong type")
    setVarValue(arguments[0].value, arguments[1]() == arguments[2]())
    pass


def instructionAnd(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) or type(arguments[1]()) != bool):
        helpFunc.error_exit(53, "And: wrong type")
    setVarValue(arguments[0].value, arguments[1]() and arguments[2]())
    pass


def instructionOr(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) or type(arguments[1]()) != bool):
        helpFunc.error_exit(53, "Or: wrong type")
    setVarValue(arguments[0].value, arguments[1]() or arguments[2]())
    pass


def instructionNot(arguments: list):
    if(type(arguments[1]()) != bool):
        helpFunc.error_exit(53, "Not: wrong type")
    setVarValue(arguments[0].value, not arguments[1]())
    pass


def instructionInt2Char(arguments: list):
    c = arguments[1]()
    if type(c) != int:
        helpFunc.error_exit(53, "Int2Char: bad type")
    try:
        c = chr(c)
    except Exception:
        helpFunc.error_exit(58, "Int2Char: int is not valid char")
    setVarValue(arguments[0].value, c)
    pass


def instructionStri2Int(arguments: list):
    s = arguments[1]()
    i = arguments[2]()
    if type(s) != str or type(i) != int:
        helpFunc.error_exit(53, "Str2Int: bad type")

    if i < 0 or i > len(s):
        helpFunc.error_exit(58, "Str2Int: index out of range")
        
    try:
        c = s[i]
        i = ord(c)
    except Exception:
        helpFunc.error_exit(58, "Str2Int: number out of range")
    setVarValue(arguments[0].value, i)
    pass


def instructionRead(arguments: list):
    read = inputFile.readline()
    if read == "":
        setVarValue(arguments[0].value, None)
        return
    read = read.strip("\n")
    try:
        if arguments[1]() == bool:
            value = read.lower() == "true"
        else:
            value = arguments[1]()(read)
    except Exception:
        value = None
    setVarValue(arguments[0].value, value)


def instructionWrite(arguments: list):
    symb = arguments[0]()
    replace = {
        bool: lambda symb: str(symb).lower(),
        type(None): lambda symb: ""
    }
    print(replace.get(type(symb), lambda symb: symb)(symb), end="")
    pass


def instructionConcat(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) or type(arguments[1]()) != str):
        helpFunc.error_exit(53,"Concat: wrong type")
    setVarValue(arguments[0].value, arguments[1]() + arguments[2]())
    pass


def instructionStrLen(arguments: list):
    if(type(arguments[1]()) != str):
        print("StrLen: symbol is not string")
        exit(53)
    setVarValue(arguments[0].value, len(arguments[1]()))
    pass


def instructionGetChar(arguments: list):
    i = arguments[2]()
    s = arguments[1]()
    if type(s) != str or type(i) != int:
        helpFunc.error_exit(53, "GetChar: bad types")

    if (i < 0) or (i >= len(s)):
        helpFunc.error_exit(58, "GetChar: index out of range")

    setVarValue(arguments[0].value, s[i])
    pass


def instructionSetChar(arguments: list):
    i = arguments[1]()
    s = arguments[0]()
    c = arguments[2]()
    if type(i) != int or type(s) != str or type(c) != str:
        helpFunc.error_exit(53, "SetChar: bad type")
    if (i < 0) or (i >= len(s)) or len(c) == 0:
        helpFunc.error_exit(58, "SetChar: index out of range")
    s = s[:i] + c[0] + s[i+1:]
    setVarValue(arguments[0].value, s)
    pass


def instructionType(arguments: list):
    types = {
        type(None): "nil",
        int: "int",
        bool: "bool",
        str: "string"
    }
    var = arguments[1].value
    if(type(var) == cVar and getVar(var).initialized == False):
        symbType = ""
    else:
        symbType = type(arguments[1]())
        if(symbType not in types):
            helpFunc.error_exit(
                99, f"Type: Symb has unsupported type {symbType}")
        symbType = types[symbType]
    setVarValue(arguments[0].value, symbType)
    pass


def instructionLabel(arguments: list):
    pass


def instructionJump(arguments: list):
    from flowLogic import setNextInstruction, getLabel
    setNextInstruction(getLabel(arguments[0]()))
    pass


def instructionJumpIfEq(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) and (type(arguments[1]()) != type(None) and type(arguments[2]()) != type(None))):
        helpFunc.error_exit(53, "Eq: wrong type")
    from flowLogic import setNextInstruction, getLabel
    label = getLabel(arguments[0]())
    if arguments[1]() == arguments[2]():
        setNextInstruction(label)
    pass


def instructionJumpIfNeq(arguments: list):
    if(type(arguments[1]()) != type(arguments[2]()) and (type(arguments[1]()) != type(None) and type(arguments[2]()) != type(None))):
        helpFunc.error_exit(53, "Eq: wrong type")
    from flowLogic import setNextInstruction, getLabel
    label = getLabel(arguments[0]())
    if arguments[1]() != arguments[2]():
        setNextInstruction(label)
    pass


def instructionExit(arguments: list):
    err = arguments[0]()
    if type(arguments[0]()) != int:
        helpFunc.error_exit(53, "Error: zla navratova hodnota")
    if err < 0 or err > 49:
        helpFunc.error_exit(57, "Error: zly typ navratovej hodnoty")
    exit(err)


def instructionDPrint(arguments: list):
    print(arguments[0](), file=sys.stderr)


def instructionBreak(arguments: list):
    printFrames()
    pass
