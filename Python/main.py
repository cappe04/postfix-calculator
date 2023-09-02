from enum import Enum
from operator import add, sub, truediv, mul, pow
from typing import Callable

class Oper(Enum):
    SUB = (0, sub, "-")
    ADD = (0, add, "+")
    DIV = (1, truediv, "/")
    MUL = (1, mul, "*")
    POW = (2, pow, "^")

    def __init__(self, order: int, op: Callable, symbol: str):
        self.order = order
        self.op = op
        self.symbol = symbol

    def __call__(self, a, b):
        return self.op(a, b)
    
    def __str__(self):
        return self.symbol

    __repr__ = __str__
    
    @classmethod
    def get(cls, symbol):
        for member in cls:
            if symbol == member.symbol:
                return member

def postfix(expr: str):
    num_stack = ""
    stack = []
    postf = []

    for char in expr + " ":
        if char == " ": 
            continue

        if char.isdigit() or char == ".":
            num_stack += char
            continue
        elif num_stack != "":
            postf.append(float(num_stack))
            num_stack = ""
        
        if char == "(":
            stack.append(char)
        elif char == ")":
            while stack and stack[-1] != "(":
                postf.append(stack.pop())
            stack.pop()
        else:
            op = Oper.get(char)
            while stack and isinstance(stack[-1], Oper) and stack[-1].order >= op.order:
                postf.append(stack.pop())
            stack.append(op)

    if num_stack != "":
        postf.append(float(num_stack))
    
    if stack:
        while stack: postf.append(stack.pop())

    return postf + list(reversed(stack))

def eval_postfix(postfix):
    stack = []
    for token in postfix:
        if type(token) == float:
            stack.append(token)
            continue
        b = stack.pop()
        a = stack.pop()
        stack.append(token(a, b))
    return stack[0] 

# (2.71 - 3.14 + 4) * (5.412 + 6/7)
postf = postfix("(2.71 - 3.14 + 4) * (5.412 + 6/7)")
print("postfix: ", *postf)
value = eval_postfix(postf)
print("value: " + str(value))


        


