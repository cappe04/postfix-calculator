

class Program {
    public static void Main(string[] args){
        calc.Postfix postfix = calc.PostfixFactory.FromString("(2.71 - 3.14 + 4) * (5.412 + 6/7)");
        Console.WriteLine(postfix.ToString());
        Console.WriteLine(postfix.Eval());
    }
}
