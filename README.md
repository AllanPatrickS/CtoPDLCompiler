miniC (using flex&bison)
========
This project is converter/compiler project using flex and bison
Forked by Yubi Lee, Sungmin Kim project.
by Philippe Geraldeli and Allan Patrick.

## 1. Building
    make
    
    or

    flex miniC.l
    bison -d miniC.y
    gcc -o miniC miniC.tab.c print.c symboltable.c lex.yy.c Converter.c -g -lfl
    
## 2. How to run
    miniC < input.txt

## 3. Results
    
	Tree/Program in PDL:resultado.txt

## 4. We follow the rule below.

    Program := (DeclList)? (FuncList)?   // DeclList FuncList | DeclList | FuncList | ε
    DeclList := (Declaration)+          // Declaration | DeclList Declaration
    FuncList := (Function)+
    Declaration := Type IdentList ;
    IdentList := identifier (, identifier)*  // identifier | IdentList , identifier
    identifier := id | id [ intnum ]      // (Note) [, ] are not symbols used in regular expression
    Function := Type id ( (ParamList)? ) CompoundStmt
    ParamList := Type identifier (, Type identifier)*
    Type := int | float
    CompoundStmt := { (DeclList)? StmtList }
    StmtList := (Stmt)*
    Stmt := AssignStmt | CallStmt | RetStmt | WhileStmt | ForStmt | IfStmt | CompoundStmt | ;
    AssignStmt :=Assign ; 
    Assign := id = Expr | id [ Expr ] = Expr
    CallStmt := Call ;
    Call := id ( (ArgList)? )
    RetStmt := return (Expr)? ; 
	Expr := MINUS Expr | MathRel Eqltop Expr | MathRel | Call | Ids
	MathRel := MathEql Relaop MathRel | MathEql
	MathEql := TERM Addiop MathEql | TERM
	TERM := FACTOR Multop TERM | FACTOR
	FACTOR := ’(’ Expr ’)’ | FLOATNUM | INTNUM
	Id := ID | ID [ Expr ]

    So, Our miniC program doesn't follow the rule below.
        1. ++, --
        2. According to this rule 
            CompoundStmt := { (DeclList)? StmtList }
            
           Followings are illegal: 
            int main()
            {
                int a;
                {
                    int b; // this is wrong because there is no "StmtList".
                }
            }
        
   
