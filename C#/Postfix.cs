namespace calc;

class Postfix {
    private readonly List<Token> postfix;

    public Postfix(List<Token> postfix) {
        this.postfix = postfix;
    }

    public override string ToString() {
        string str = "";
        foreach(Token token in postfix){
            str += token.ToString() + " ";
        }
        return str;
    }

    public float Eval(){
        List<float> stack = new();
        foreach(Token token in postfix){
            if(token.type == Type.NUMBER){
                stack.Add(token.value);
                continue;
            }

            float b = stack.Last();
            stack.RemoveAt(stack.Count - 1);
            float a = stack.Last();

            stack[^1] = token.type switch{
                Type.SUB => a - b,
                Type.ADD => a + b,
                Type.DIV => a / b,
                Type.MUL => a * b,
                Type.POW => (float)System.Math.Pow(a, b),
                _ => throw new Exception("Invalid Postfix"),
            };

        }

        return stack[0];
    }
}