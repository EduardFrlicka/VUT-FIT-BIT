from sys import stderr
from typing import Callable
import xml.etree.ElementTree as et
from re import search, findall, match, sub
import helpFunc


class cVar:
    frame: str
    name: str

    def __init__(self, frame, name):
        self.frame = frame
        self.name = name

    def __str__(self) -> str:
        return f"Var: {self.frame}@{self.name}"

    def __call__(self):
        from framesLogic import getVarValue
        return getVarValue(self)


class cVariable:
    initialized: bool = False
    value: object

    def __init__(self):
        self.initialized = False

    def setValue(self, value):
        self.value = value
        self.initialized = True

    def __call__(self):
        if not self.initialized:
            helpFunc.error_exit(56,"Variable not initialized")
        return self.value


class cInstruction:
    argTypes: list
    argCount: int
    func: Callable

    def __init__(self, func: Callable, argTypes: int):
        self.func = func
        self.argCount = len(argTypes)
        self.argTypes = argTypes

    def checkTypeCount(self, instruction: et.Element):
        if self.func == None:
            helpFunc.error_exit(32, "Bad XML: unknown or missing opcode")

        if len(instruction) != self.argCount:
            helpFunc.error_exit(32,
                                f"zly pocet argumentov v xml \n\tExpected: {self.argCount}, got: {len(instruction)}")


class cArgument:
    value = None

    def __str__(self) -> str:
        return f"arg: {self.value}"

    def __repr__(self) -> str:
        return f"arg: {self.value}"

    def __init__(self, expected: str, elem: et.Element):
        if expected == "symb":
            self.initSymb(elem)
            return
        if expected == "var":
            if elem.attrib.get("type") != "var":
                helpFunc.error_exit(32,
                                    f"initVar: Bad XML, wrong type attribute ({elem.attrib.get('type')})")
            self.initVar(elem.text)
        if expected == "label":
            if elem.attrib.get("type") == None:
                helpFunc.error_exit(32, "getLabel: Bad XML, no type attribute")
            self.initLabel(elem)
        if expected == "type":
            self.initType(elem)
        pass

    def initLabel(self, elem: et.Element):
        if not match(r"^([a-zA-Z_\-\$&%\*!\?][a-zA-Z\d_\-\$&%\*!\?]*)$", elem.text):
            helpFunc.error_exit(32, "getLabel: Bad XML, wrong name of label")
        self.value = elem.text

    def initType(self, elem: et.Element):
        if elem.attrib.get("type") != "type":
            helpFunc.error_exit(32,
                                f"initType: Bad XML, wrong type attribute ({elem.attrib.get('type')})")
        types = {
            "int": int,
            "bool": bool,
            "string": str,
            "float": float
        }
        self.value = types.get(elem.text)
        if self.value == None:
            helpFunc.error_exit(32,
                                f"initType: Bad XML, wrong type value ({elem.text})")
        pass

    def initVar(self, text: str):
        re_result = search(
            r"^([LTG]F)@([a-zA-Z_\-\$&%\*!\?][a-zA-Z\d_\-\$&%\*!\?]*)$", text)
        if not re_result:
            helpFunc.error_exit(32, "initVar: Bad XML, wrong name of variable")
        self.value = cVar(re_result.group(1), re_result.group(2))

    def initNil(self, text: str):
        if text != "nil":
            helpFunc.error_exit(32, "initNil: Bad XML, wrong value of nil")
        self.value = None

    def initFloat(self, text: str):
        helpFunc.error_exit(32, "float not implemented yet")

    def initBool(self, text: str):
        if not match(r"^(true|false)$", text):
            helpFunc.error_exit(32, "initBool: Bad XML, wrong bool value")
        self.value = text == "true"

    def initString(self, text: str):
        if text==None:
            self.value = ""
            return
        if len(findall(r"\\", text)) > len(findall(r"\\\d{3}", text)):
            helpFunc.error_exit(32,
                                "initString: Bad XML, wrong escape sequence in string")
        self.value = sub(r"\\(\d{3})", lambda match: chr(
            int(match.group(1))), text)

    def initInt(self, text: str):
        if not match(r"^[\-+]?\d+$", text):
            helpFunc.error_exit(32, "initInt: Bad XML, wrong value of int")
        self.value = int(text)

    def typeNotFound(self, text: str):
        helpFunc.error_exit(32, "initSymb: Bad XML, wrong type atribute")

    def initSymb(self, elem: et.Element):
        symbType = elem.attrib.get("type")
        if not symbType:
            helpFunc.error_exit(32, "initSymb: Bad XML, no type attribute")
        switch = {
            "int": self.initInt,
            "string": self.initString,
            "nil": self.initNil,
            "bool": self.initBool,
            "float": self.initFloat,
            "var": self.initVar
        }
        switch.get(symbType, self.typeNotFound)(elem.text)

    def __call__(self):
        if type(self.value) == cVar:
            return self.value()
        return self.value


class cInstructionInstance:
    order: int
    arguments: list = []
    func: Callable
    next = None

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return "%2d %20s : %s" % (self.order, self.func.__name__, self.arguments)

    def __init__(self, instruction: cInstruction, elem: et.Element):
        from helpFunc import getOrder
        instruction.checkTypeCount(elem)
        self.arguments = []
        for i in range(len(instruction.argTypes)):
            if elem[i].tag != f"arg{i+1}":
                helpFunc.error_exit(32, "arg number bad XML")
            self.arguments.append(cArgument(instruction.argTypes[i], elem[i]))
        self.func = instruction.func
        self.order = getOrder(elem)

    def __call__(self):
        print(f"\ncalled {self.order}", file=stderr)
        self.func(self.arguments)

    def setNext(self, next=None):
        self.next = next
