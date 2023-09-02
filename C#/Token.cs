namespace calc;

enum Type {
    NUMBER = 0,
    SUB,
    ADD,
    DIV,
    MUL,
    POW,
    OPAR,
    CPAR,
};

struct Token
{
    public Type type;
    public float value;

    public Token(Type type, float value){
        this.type = type;
        this.value = value;
    }
    public Token(Type type){
        this.type = type;
        this.value = this.type switch
        {
            Type.SUB => 0,
            Type.ADD => 0,
            Type.DIV => 1,
            Type.MUL => 1,
            Type.POW => 2,
            _ => -1,
        };
    }

    public override string ToString() {
        return type switch
        {
            Type.NUMBER => value.ToString(),
            Type.SUB => "-",
            Type.ADD => "+",
            Type.DIV => "/",
            Type.MUL => "*",
            Type.POW => "^",
            Type.OPAR => "(",
            Type.CPAR => ")",
            _ => "?",
        };
    }
};
