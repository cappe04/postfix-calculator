namespace calc;

class PostfixFactory {

    public static Postfix FromString(string expr){
        List<Token> postfix = new();
        List<Token> stack = new();
        string num_stack = "";

        foreach (char symbol in expr)
        {
            if (symbol == ' '){
                continue;
            }

            if(char.IsDigit(symbol)){
                num_stack += symbol;
                continue;
            }
            else if (symbol == '.' || symbol == ','){
                num_stack += ',';
                continue;
            }
            else if (num_stack.Length > 0){
                float value;
                float.TryParse(num_stack, out value);
                postfix.Add(new Token(Type.NUMBER, value));
                num_stack = "";
            }

            if(symbol == '('){
                stack.Add(new Token(Type.OPAR));
            }
            else if(symbol == ')'){
                while(stack.Count > 0 && stack.Last().type != Type.OPAR){
                    postfix.Add(stack.Last());
                    stack.RemoveAt(stack.Count - 1);
                }
                stack.RemoveAt(stack.Count - 1);
            }
            else {
                Token token = new Token(symbol switch {
                    '-' => Type.SUB,
                    '+' => Type.ADD,
                    '/' => Type.DIV,
                    '*' => Type.MUL,
                    '^' => Type.POW,
                    _ => throw new Exception("Syntax Error"),
                });
                while(stack.Count > 0 && stack.Last().value >= token.value){
                    postfix.Add(stack.Last());
                    stack.RemoveAt(stack.Count - 1);
                }
                stack.Add(token);
            }
        }

        if(num_stack.Length > 0){
            postfix.Add(new Token(Type.NUMBER, float.Parse(num_stack)));
        }

        if(stack.Count > 0){
            while(stack.Count > 0){
                postfix.Add(stack.Last());
                stack.RemoveAt(stack.Count - 1);
            }
        }

        return new Postfix(postfix);
    }
}