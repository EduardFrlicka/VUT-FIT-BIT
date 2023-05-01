import xml.etree.ElementTree as et
import sys
# from helpFunc import checkInstruction
# from classes import *
import argparse
from flowLogic import flowInitialization, setFirstInstruction, getCurrInstruction, setNextInstruction, setCurrInstruction
# from framesLogic import *
import helpFunc
import instructions


def main():
    argParser = argparse.ArgumentParser()
    argParser.add_argument("--source")
    argParser.add_argument("--input")
    args = argParser.parse_args()

    if args.source:
        file = args.source
    else:
        file = sys.stdin

    instructions.setInputFile(args.input)
        
    try:
        tree = et.parse(file)
    except Exception:
        helpFunc.error_exit(31,"Bad xml")

    program = tree.getroot()
    flowInitialization(program)

    setFirstInstruction()
    while getCurrInstruction() != None:
        setNextInstruction()
        getCurrInstruction()()
        setCurrInstruction()
        # printFrames()

    return


if __name__ == "__main__":
    main()
exit(0)
