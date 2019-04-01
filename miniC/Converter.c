//Converte C para PDL utilizando MiniC
//autores: Allan Patrick e Philippe Geraldeli
 
#include <stdio.h>
#include <stdlib.h>
#include "print.h"
#include "symboltable.h"
#include "Converter.h"
#include "AST.h"
#include <string.h> 
 
bool _isParam2 = false;
bool _needPrinted2 = false;
bool _isOtherComp2 = false;
bool _isCompound2 = false;
Type_e _curType2;
No *root;
No *aux;
No *aux2;
No *treeSet[100];
FILE* fp;
Numero* numero;
int setTam = 0;
int x = 0;
int y = 0;
int tag = 0;
int id_tag = 0;
int tam = 0;
int tamAux = 0;
int choice = 0;
int tamMethod = 0;
char *Parts3[100][4];
char *methodVector[100][2];

No* insert(No *no, int choice, char *chars){
    if (root==NULL){
        root = (No*)malloc(sizeof(No));
        root->child = NULL;
        root->parent = NULL;
        root->chars = chars;
        aux = root;
 
        return root;
    }
    else {
        if (choice == 0){
            No* noAux = (No*)malloc(sizeof(No));
            noAux->child = NULL;
            noAux->parent = NULL;
            noAux->chars = chars;
            no->child = noAux;
            aux = no->child;
            return aux;
 
        }else if(choice == 1){
            No* noAux = (No*)malloc(sizeof(No));
            noAux->child = NULL;
            noAux->parent = NULL;
            noAux->chars = chars;
            no->parent = noAux;
            aux = no->parent;
            return aux;
        }
    }
}
 
void print(No *no, FILE* fp){
    if(no!=NULL){
       	printf("%s ",no->chars);
        fprintf(fp, "%s", no->chars);
 
        if(no->child!=NULL){
            print(no->child, fp);
        }
        if(no->parent!=NULL){
            print(no->parent, fp);
        }
    }
}
 
void BuildTree(struct PROGRAM* head){
	numero = (Numero*)malloc(sizeof(Numero));
	fp = fopen("resultado.txt","w");
	if(head->decl != NULL){
        visitDeclaration2(head->decl);
    }
    if(head->func != NULL){
        visitFunction2(head->func);
    }
    printf("\n");
    for(int i = 0; i<tamMethod;i++){
        printf("%s", methodVector[i][0]);
        printf(" = ");
        printf("%s", methodVector[i][1]);
        printf("\n");
    }
    printf("\n");
	for(int i = 0; i<tam;i++){
		for(int j = 0; j < 4; j++){
            if(j == 1){
                printf("= ");
            }
			printf("%s",Parts3[i][j]);
			printf(" ");
		}
		printf("\n");
	}
    printf("\n");
    print(root, fp);
    printf("\n");
	fclose(fp);
}
 
void visitDeclaration2(struct DECLARATION* decl) {
    _isParam2 = false;   //needed when we have to decide it is parameter or variable.
    if(decl->prev != NULL) {
        visitDeclaration2(decl->prev);
    }
    switch(decl->t) {
        case eInt:
            _curType2 = eInt;
            break;
        case eFloat:
            _curType2 = eFloat;
            break;
        default:
            exit(1);
    }
    _needPrinted2 = true;
    visitIdentifier2(decl->id);
    _needPrinted2 = false;
}
void visitFunction2(struct FUNCTION* func) {
	 if(func->prev != NULL) {
        visitFunction2(func->prev);
    }
    _curFuncName = func->ID;
    scopeTail = newScope(sFUNC, scopeTail);

    switch(func->t) {
        case eInt:
            break;
        case eFloat:
            break;
        default:
            exit(1);
    }
	if(strcmp(func->ID, "main")){
        int present = 0;
        for(int i = 0; i < tamMethod; i++){
            if(!strcmp(func->ID, methodVector[i][1])){
                present = 1;
                insert(aux, choice, methodVector[i][0]);
                choice = 0;
            }
        }
        if(present == 0){methodsFunction(func->ID);}
		insert(aux, choice, "=");
	}
	insert(aux, choice, "(");
    choice = 0;
    treeSet[setTam] = aux;
    setTam++;
    if(func->param != NULL) {
        visitParameter2(func->param);
    }
	visitCompoundStmt2(func->cstmt);
    insert(aux, choice, ")");
	insert(aux,choice,"\n");
    choice = 1;
    treeSet[setTam] = aux;
    setTam++;
    deleteScope(&scopeTail);
}
void visitIdentifier2(struct IDENTIFIER* iden) {
    if(iden->prev != NULL) {
        visitIdentifier2(iden->prev);
    }
	if(( _curType2 == eInt) && (_isParam2 == false)) insert(aux,choice,"int ");
	if(( _curType2 == eFloat) && (_isParam2 == false)) insert(aux,choice,"float ");	
	insert(aux, choice, iden->ID);
	insert(aux,choice,";");
    if(iden->intnum > 0) {
		insert(aux, choice, "[");
		
		char num[255];
		
		snprintf(num, sizeof(iden->intnum), "%d",10);
		
		insert(aux, choice, num);
		insert(aux, choice, "]");

        if( _needPrinted2 == true) {
            char* curType;
            if(_curType2 == eInt)
                curType = "int";
            else
                curType = "float";
        }
    } else if(iden->intnum < 0) {
        printf("minus array");
    } else { 
        //scalar
        if( _needPrinted2 == true) {
            char* curType;
            if(_curType2 == eInt)
                curType = "int";
            else
                curType = "float";
            }
    }
}
void visitStmt2(struct STMT* stmt) {
    if(stmt->prev != NULL)
        visitStmt2(stmt->prev);
	
    switch(stmt->s) {
        case eAssign:
			InsertSemicolon(stmt);
            visitAssignStmt2(stmt->stmt.assign_);
            break;

        case eCall:
			InsertSemicolon(stmt);
            visitCallStmt2(stmt->stmt.call_);
            break;

        case eRet:
			InsertSemicolon(stmt);
            if(stmt->stmt.return_ == NULL){
				insert(aux, choice, "return;");
            }
            else {
				aux2 = aux;
				insert(aux, choice, "return ");
                visitExpr2(stmt->stmt.return_);
            }
            break;

        case eWhile:
			InsertSemicolon(stmt);
             _isOtherComp2 = true;
            visitWhile_s2(stmt->stmt.while_);
            return;
        case eFor:
			InsertSemicolon(stmt);
             _isOtherComp2 = true;
            visitFor_s2(stmt->stmt.for_);
            return;
        case eIf:
			InsertSemicolon(stmt);
             _isOtherComp2 = true;
            visitIf_s2(stmt->stmt.if_);
            return;

        case eCompound:
			InsertSemicolon(stmt);
            if(_isOtherComp2 == false)
                _isCompound2 = true;
            visitCompoundStmt2(stmt->stmt.cstmt_);
            return;

        case eSemi:
			InsertSemicolon(stmt); 
            break;

    }
}
void visitParameter2(struct PARAMETER* param) {
    _isParam2 = true;
    if(param->prev != NULL) {
        visitParameter2(param->prev);
        if(strcmp(aux->chars, ";")) {
            insert(aux, choice, ", ");
            treeSet[setTam] = aux;
            setTam++;
            choice = 1;
        }
    }
    switch(param->t) {
        case eInt:
			insert(aux, choice, "int ");
            choice = 0;			
            _curType2 = eInt;
            break;
        case eFloat:
			insert(aux, choice, "float ");
            choice = 0;
            _curType2 = eFloat;
            break;
        default:
            printf("Declaration does not exist.\n");
            exit(1);
    }
    _needPrinted2 = true;
    visitIdentifier2(param->id);
    _needPrinted2 = false;
}
void visitCompoundStmt2(struct COMPOUNDSTMT* cstmt) {
    if(_isCompound2 == true) {
        scopeTail = newScope(sCOMPOUND, scopeTail);
        scopeTail->parent->compound_n++;
    }
    _isOtherComp2 = false;
    if(cstmt->decl != NULL) { 
        visitDeclaration2(cstmt->decl);
    }
    if(cstmt->stmt != NULL)
        visitStmt2(cstmt->stmt);

    if(_isCompound2 == true) {
        deleteScope(&scopeTail);
    }
    _isCompound2 = false;
    _isOtherComp2 = false;
}
void visitAssignStmt2(struct ASSIGN* assign) {
	aux2 = (No*)malloc(sizeof(No));
	aux2->child = NULL;
	aux2->parent = NULL;
	aux2->chars = assign->ID;
    if(assign->index != NULL) {
		insert(aux, choice, "[");
        visitExpr2	(assign->index);
		insert(aux, choice, "]");
    }
	switch(assign->expr->e){
		case eRela:
			insert(aux, choice, assign->ID);
			break;
		case eIntnum:
			insert(aux, choice, assign->ID);
			insert(aux, choice, "=");
			break;
		case eFloatnum:
			insert(aux, choice, assign->ID);
			insert(aux, choice, "=");
			break;
		case eId:
			insert(aux, choice, assign->ID);
			insert(aux, choice, "=");
			break;
	}

    visitExpr2(assign->expr);
}
void visitCallStmt2(struct CALL* call) {
    int present = 0;
    for(int i = 0; i < tamMethod; i++){
        if(!strcmp(call->ID, methodVector[i][1])){
            present = 1;
            insert(aux, choice, methodVector[i][0]);
        }
    }
    if(present == 0){methodsFunction(call->ID);}
	insert(aux, choice, "(");
    if(call->arg != NULL) {
        visitArg2(call->arg);
    }
	insert(aux, choice, ")");
}
void visitArg2           (struct ARG* arg) {
    if(arg->prev != NULL) {
        visitArg2(arg->prev);
		insert(aux, choice, ", ");
    }
	aux2 = aux;
    visitExpr2(arg->expr);
}
	
void visitExpr2(struct EXPR* expr) {
	switch(expr->e) {
        case eUnop:
			insert(aux, choice, "-");
            visitExpr2(expr->expression.unop_->expr);
            break;
			
		case eAddi:
			id_tag = 1;
            tamAux = tam;
			tag++;
            visitExpr2(expr->expression.addiop_->lhs);
            tag--;
			if(expr->expression.addiop_->a == ePlus){
                Parts3[tag+tamAux][2] = "+";
			}
            else{
                Parts3[tag+tamAux][2] = "-";
			}
			
            visitExpr2(expr->expression.addiop_->rhs);
			
			if(tag!=0)
				Parts(expr->expression.addiop_->rhs);
			else{
				Parts3[tam][0] = aux2->chars;
				tam++;
				for(int i = tamAux;i<tam;i++){
					if(i>tamAux){
						Parts3[i][3]= Parts3[i-1][0];
					}
				}
				for(tamAux; tamAux<tam;tamAux++){
					insert(aux, choice, Parts3[tamAux][0]);
					insert(aux, choice, "=");
					insert(aux, choice, Parts3[tamAux][1]);
					insert(aux, choice, Parts3[tamAux][2]);
					insert(aux,choice,  Parts3[tamAux][3]);
					if(!(tamAux+1==tam))
						insert(aux, choice, ";");
				}
				id_tag = 0;
			}
			
            break;
		
		case eMulti:
			id_tag = 1;
            tamAux = tam;
			tag++;
            visitExpr2(expr->expression.multop_->lhs);
			tag--;
			if(expr->expression.multop_->m == eMult){
				Parts3[tag+tamAux][2] = "*";
			}
            else{
				Parts3[tag+tamAux][2] = "/";
			}

            visitExpr2(expr->expression.multop_->rhs);
			
			if(tag!=0)
				Parts(expr->expression.multop_->rhs);
			else{
				Parts3[tam][0] = aux2->chars;
				tam++;
				for(int i = tamAux;i<tam;i++){
					if(i>tamAux){
						Parts3[i][3]= Parts3[i-1][0];
					}
				}
				for(tamAux; tamAux<tam;tamAux++){
					insert(aux, choice, Parts3[tamAux][0]);
					insert(aux, choice, "=");
					insert(aux, choice, Parts3[tamAux][1]);
					insert(aux, choice, Parts3[tamAux][2]);
					insert(aux, choice, Parts3[tamAux][3]);
					if(!(tamAux+1==tam))
						insert(aux, choice, ";");
				}
				id_tag = 0;
			}
			
            break;
			
		case eRela:
            visitExpr2(expr->expression.relaop_->lhs);
            switch(expr->expression.relaop_->r) {
                case eLT:
                    insert(aux, choice, " < ");
                    break;

                case eGT:
                    insert(aux, choice, " > ");
                    break;

                case eLE:
                    insert(aux, choice, " <= ");
                    break;

                case eGE:
                    insert(aux, choice, " >= ");
                    break;
            }
            visitExpr2(expr->expression.relaop_->rhs);
            break;	
			
		case eEqlt:
            visitExpr2(expr->expression.eqltop_->lhs);
            if(expr->expression.eqltop_->e == eEQ) {
                insert(aux, choice, " == ");
            } else {
                insert(aux, choice, " != ");
            }
            visitExpr2(expr->expression.eqltop_->rhs);
            break;
			
		case eCallExpr:
            visitCallStmt2(expr->expression.call_);
            break;
			
		case eExpr:
            insert(aux, choice, "(");
            visitExpr2(expr->expression.bracket);
            insert(aux, choice, ")");
            break;
			
		case eId:
            visitId_s2(expr->expression.ID_);
            break;
			
		case eIntnum:
			switch(id_tag){
				case 0:
				if(numero != NULL){
					snprintf(numero->num, 255*sizeof(char), "%d", (int)expr->expression.intnum);
					insert(aux, choice, numero->num);
					numero = numero->prox;
				}else if(numero == NULL){
					Numero* novo = (Numero*)malloc(sizeof(Numero));
					numero = novo;
					snprintf(numero->num, 255*sizeof(char), "%d", (int)expr->expression.intnum);
					insert(aux, choice, numero->num);
					numero = numero->prox;
				}
				break;
				case 1:
				if(numero != NULL){
					snprintf(numero->num, 255*sizeof(char), "%d", (int)expr->expression.intnum);
					if(tag==0){
					    Parts3[tamAux][3]= numero->num;
					}
					else if(tag==1){
					    Parts3[tamAux][1]= numero->num;
					}
					else {
					    Parts3[tag-1][1]= numero->num;
					}
					numero = numero->prox;
				}else if(numero == NULL){
					Numero* novo = (Numero*)malloc(sizeof(Numero));
					numero = novo;
					snprintf(numero->num, 255*sizeof(char), "%d", (int)expr->expression.intnum);
					if(tag==0){
					    Parts3[tamAux][3]= numero->num;
					}
					else if(tag==1){
					    Parts3[tamAux][1]= numero->num;
					}
					else {
                        Parts3[tag-1][1]= numero->num;
					}
					numero = numero->prox;
				}
				break;
			} 
            break;

		case eFloatnum:
			switch(id_tag){
				case 0:
				if(numero != NULL){
					snprintf(numero->num, 255*sizeof(char), "%f", (float)expr->expression.floatnum);
					insert(aux,choice, numero->num);
					numero = numero->prox;
				}else if(numero == NULL){
					Numero* novo = (Numero*)malloc(sizeof(Numero));
					numero = novo;
					snprintf(numero->num, 255*sizeof(char), "%f", (float)expr->expression.floatnum);
					insert(aux,choice, numero->num);
					numero = numero->prox;
				}
				break;
				case 1:
				if(numero != NULL){
					snprintf(numero->num, 255*sizeof(char), "%f", (float)expr->expression.floatnum);
                    if(tag==0){
					    Parts3[tamAux][3]= numero->num;
					}
					else if(tag==1){
					    Parts3[tamAux][1]= numero->num;
					}
					else {
					    Parts3[tag-1][1]= numero->num;
					}
					numero = numero->prox;
				}else if(numero == NULL){
					Numero* novo = (Numero*)malloc(sizeof(Numero));
					numero = novo;
					snprintf(numero->num, 255*sizeof(char), "%f", (float)expr->expression.floatnum);
                    if(tag==0){
					    Parts3[tamAux][3]= numero->num;
					}
					else if(tag==1){
					    Parts3[tamAux][1]= numero->num;
					}
					else {
					    Parts3[tag-1][1]= numero->num;
					}
					numero = numero->prox;
				}
				break;
				
			}
            break;	
    }
}	

void visitWhile_s2(struct WHILE_S* while_s) {
    if(while_s->do_while == true) {
        scopeTail = newScope(sDOWHILE, scopeTail);
        scopeTail->parent->dowhile_n++;
		insert(aux, choice, "(");
        visitStmt2(while_s->stmt);
		insert(aux, choice, "(");
		aux2 = aux;
        visitExpr2(while_s->cond);
		insert(aux,choice,")?");
        visitStmt2(while_s->stmt);
		insert(aux, choice, ")*;¬(");
		aux2 = aux;
		visitExpr2(while_s->cond);
		insert(aux,choice,")? ");
		
    } else {
        scopeTail = newScope(sWHILE, scopeTail);
        scopeTail->parent->while_n++;
		insert(aux, choice, "( (");
		aux2 = aux;
        visitExpr2(while_s->cond);
		insert(aux,choice,")?");
        visitStmt2(while_s->stmt);
    	insert(aux, choice, ")*;  ¬(");
		aux2 = aux;
        visitExpr2(while_s->cond);
        insert(aux,choice,")?");
    
	}
    deleteScope(&scopeTail);
}
void visitFor_s2(struct FOR_S* for_s) {
    scopeTail = newScope(sFOR, scopeTail);
    scopeTail->parent->for_n++;
	insert(aux, choice,"(");
    visitAssignStmt2(for_s->init);
	insert(aux,choice,";(");
	aux2 = aux;
    visitExpr2(for_s->cond);
    insert(aux,choice,")?");
	insert(aux, choice, "; ");
	visitStmt2(for_s->stmt);
	insert(aux,choice, "; ");
    visitAssignStmt2(for_s->inc);
	insert(aux,choice,")*;¬(");
	aux2 = aux;
	visitExpr2(for_s->cond);
	insert(aux,choice,")?");
    deleteScope(&scopeTail);
}
void visitIf_s2(struct IF_S* if_s) {
    scopeTail = newScope(sIF, scopeTail);
    scopeTail->parent->if_n++;

    insert(aux, choice, "( (");
	aux2 = aux;
    visitExpr2(if_s->cond);
	insert(aux,choice,")? ");
	if(if_s->if_->s){
		insert(aux,choice,";");
	}
    visitStmt2(if_s->if_);
    insert(aux,choice," )");
    if (if_s->else_ != NULL) {
        insert(aux, choice, "U(¬(");
		aux2 = aux;
        visitExpr2(if_s->cond);
        insert(aux,choice,")? ");
		if(if_s->else_->s){
			insert(aux,choice,";");
		}
        visitStmt2(if_s->else_);
		insert(aux, choice, ")");
    } 
}
void visitId_s2(struct ID_S* id_s) {
   if(id_tag==0){
	   insert(aux,choice,id_s->ID);
   }
   else{
		if(tag==0){
			Parts3[tamAux][3]= id_s->ID;
		}
		else if(tag==1){
			Parts3[tamAux][1]= id_s->ID;
		}
		else {
			Parts3[tag-1][1]= id_s->ID;
		}  
   }

   if(id_s->expr != NULL) {
	insert(aux, choice, "[");
	aux2 = aux;
    visitExpr2(id_s->expr);
	insert(aux, choice, "]");
   }
}

void InsertSemicolon(struct STMT* stmt){
	if(stmt->prev !=NULL){
		
		insert(aux, choice, ";");

	}
}

void Parts(struct EXPR* expr){
    char var[] = "_tX";
	int buffersize = 100;
	char* variable = malloc(buffersize);
	switch(expr->e) {
        case eId:
			var[2] = ++x + '0';
			strncpy(variable,var,buffersize);
			Parts3[tam][0] = variable;
			tam++;
            break;
        case eIntnum:
            var[2] = ++x + '0';
            strncpy(variable,var,buffersize);
			Parts3[tam][0] = variable;
			tam++;
            break;
        case eFloatnum:
			var[2] = ++x + '0';
			strncpy(variable,var,buffersize);
			Parts3[tam][0] = variable;
			tam++;
            break;
        default:
            break;    
    } 
      
}

void methodsFunction(char* ID){
    char var[] = "_yX";
	int buffersize = 100;
	char* variable = malloc(buffersize);
    var[2] = ++y + '0';
    strncpy(variable,var,buffersize);
    methodVector[tamMethod][0] = variable;
    methodVector[tamMethod][1] = ID;
    insert(aux, choice, methodVector[tamMethod][0]);
    tamMethod++;
}
