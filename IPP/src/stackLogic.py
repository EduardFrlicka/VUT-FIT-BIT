import helpFunc



stack = []

def stackPush(value):
    stack.append(value)


def stackPop():
    if len(stack)<=0:
        helpFunc.error_exit(56,"StackPop: Stack is empty")
    return stack.pop()