
#include "AST.h"

typedef struct No{
	struct No *child;
	struct No *parent;
	struct No *prox;
	char *chars;
}No;

typedef struct Numero{
	struct Numero* prox;
	char num[255];
}Numero;

No* CreateTree();
No* insert					(No *no, char *choice, char *chars);
void print					(No *no, FILE* fp);
void BuildTree				(struct PROGRAM* head);
void  visitDeclaration2		(struct DECLARATION* decl);
void  visitFunction2		(struct FUNCTION* func);
void  visitIdentifier2		(struct IDENTIFIER* iden);
void  visitSTMT2			(struct STMT* stmt);
void  visitParameter2		(struct PARAMETER* param);
void  visitCompoundStmt2	(struct COMPOUNDSTMT* cstmt);
void visitAssignStmt2		(struct ASSIGN* assign);
void visitCallStmt2			(struct CALL* call);
void visitArg2				(struct ARG* arg);
void visitExpr2				(struct EXPR* expr);
void visitWhile_s2			(struct WHILE_S* while_s);
void visitFor_s2			(struct FOR_S* for_s);
void visitIf_s2				(struct IF_S* if_s);
void visitId_s2				(struct ID_S* id_s);
void InsertSemicolon		(struct STMT* stmt);