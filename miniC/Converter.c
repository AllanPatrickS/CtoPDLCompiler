//Converte C para PDL utilizando MiniC
//autores: Philippe Geraldeli e Allan Patrick
 
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
bool _isTitlePrinted2 = false;
Type_e _curType2;
No *root;
No *aux;
No *aux2;
FILE* fp;
Numero* numero;
int x = 0;
int tag = 0;
int tam = 0;
int rows;
char *Parts3[100][4];
int pont = 0;


No* insert(No *no, char *choice, char *chars){
    if (root==NULL){
        root = (No*)malloc(sizeof(No));
        root->child = NULL;
        root->parent = NULL;
        root->chars = chars;
        aux = root;
 
        return root;
    }
    else {
        if (choice == "child"){
            No* noAux = (No*)malloc(sizeof(No));
            noAux->child = NULL;
            noAux->parent = NULL;
            noAux->chars = chars;
            no->child = noAux;
            aux = no->child;
            return aux;
 
        }else if(choice == "parent"){
            No* noAux = (No*)malloc(sizeof(No));
            noAux->child = NULL;
            noAux->parent = NULL;
            noAux->chars = chars;
            no->child = noAux;
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
    /*
	Comentei pelo warning

	for(int i = 0; i<tam;i++){
		for(int j = 0; j < 4; j++){
			printf(Parts3[i][j]);
			printf(" ");
		}
					printf("\n");
	}
	*/
    print(root, fp);
    printf("\n");
	fclose(fp);
}
 
void visitDeclaration2   (struct DECLARATION* decl) {
    _isParam2 = false;
    if(decl->prev != NULL) {
        visitDeclaration2(decl->prev);
    }
    if(!_isTitlePrinted2) {
        _isTitlePrinted2 = true;
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
void visitFunction2      (struct FUNCTION* func) {
	 if(func->prev != NULL) {
        visitFunction2(func->prev);
    }

    _curFuncName = func->ID;

    scopeTail = newScope(sFUNC, scopeTail); 

	if(strcmp(func->ID, "main")){
	    insert(aux,"child", func->ID);
	    if(func->param != NULL) {
        	_isTitlePrinted2 = true;
        	insert(aux,"child","(");
       		visitParameter2(func->param); 
       		insert(aux,"child",")");
	    }
		insert(aux, "child", "=");
	}
	insert(aux, "child", "(");
	_isTitlePrinted2 = false;
    
	visitCompoundStmt2(func->cstmt);
    insert(aux, "child", ")");

	insert(aux,"child","\n");

    deleteScope(&scopeTail);
    _isTitlePrinted2 = false;
}
void visitIdentifier2    (struct IDENTIFIER* iden) {
    if(iden->prev != NULL) {
        visitIdentifier2(iden->prev);

    }
	if(( _curType2 == eInt)) insert(aux,"child","int ");
	if(( _curType2 == eFloat)) insert(aux,"child","float ");	

	insert(aux, "child", iden->ID);
	if( _isParam2 == false) insert(aux,"child",";");
    if(iden->intnum > 0) {

		insert(aux, "child", "[");
		
		char num[255];
		
		snprintf(num, sizeof(iden->intnum), "%d",10);
		
		insert(aux, "child", num);
		insert(aux, "child", "]");

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
        
        if( _needPrinted2 == true) {
            char* curType;
            if(_curType2 == eInt)
                curType = "int";
            else
                curType = "float";
        }
    }
}
void visitStmt2          (struct STMT* stmt) {
    
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
				insert(aux, "child", "return;");
            }
            else {
				aux2 = aux;
				insert(aux, "child", "return ");
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

void visitParameter2     (struct PARAMETER* param) {
	
    _isParam2 = true;
    if(param->prev != NULL) {
        visitParameter2(param->prev);

		insert(aux, "child", ", ");
    }

    _needPrinted2 = true;
    visitIdentifier2(param->id);
    _needPrinted2 = false;
   
}
void visitCompoundStmt2  (struct COMPOUNDSTMT* cstmt) {
    if(_isCompound2 == true) {

        scopeTail = newScope(sCOMPOUND, scopeTail);
        _isTitlePrinted2 = false;
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
void visitAssignStmt2    (struct ASSIGN* assign) {
	

    //printf("%s ",assign->ID);
	aux2 = (No*)malloc(sizeof(No));
	aux2->child = NULL;
	aux2->parent = NULL;
	aux2->chars = assign->ID;
    
    if(assign->index != NULL) {

		insert(aux, "child", "[");
        visitExpr2	(assign->index);
		insert(aux, "child", "]");
    
    }
    
    visitExpr2(assign->expr);

}
void visitCallStmt2      (struct CALL* call) {
    //printf("%s(", call->ID);
	insert(aux, "child", call->ID);
	insert(aux, "child", "(");
    if(call->arg != NULL) {
        visitArg2(call->arg);
    }
    //printf(")");
	insert(aux, "child", ")");
}
void visitArg2           (struct ARG* arg) {
    if(arg->prev != NULL) {
        visitArg2(arg->prev);
        //printf(", ");
		insert(aux, "child", ", ");
    }
	aux2 = aux;
    visitExpr2(arg->expr);
}
	
void visitExpr2          (struct EXPR* expr) {
	switch(expr->e) {
        case eUnop:

			insert(aux, "child", "-");
            visitExpr2(expr->expression.unop_->expr);
            break;
			
		case eAddi:

			tag++;
            visitExpr2(expr->expression.addiop_->lhs);
            tag--;
			if(expr->expression.addiop_->a == ePlus){
                Parts3[tag][2] = "+";
			}
            else{
                Parts3[tag][2] = "-";
			}

            visitExpr2(expr->expression.addiop_->rhs);
			
			if(tag!=0)
				Parts(expr->expression.addiop_->rhs);
			else{
				Parts3[tam][0] = aux2->chars;
				tam++;
				for(int i = pont;i<tam;i++){
					if(i>0){
						Parts3[i][3]= Parts3[i-1][0];
					}
				}
				for(pont; pont<tam;pont++){
					insert(aux, "child", Parts3[pont][0]);
					insert(aux, "child", "=");
					insert(aux, "child", Parts3[pont][1]);
					insert(aux, "child", Parts3[pont][2]);
					insert(aux,"child",  Parts3[pont][3]);
					if(!(pont+1==tam))
						insert(aux, "child", ";");
				}

			}

            break;
		
		case eMulti:
			tag++;
            visitExpr2(expr->expression.multop_->lhs);
			tag--;
			if(expr->expression.multop_->m == eMult){
				Parts3[tag][2] = "*";
			}
            else{
				Parts3[tag][2] = "/";
			}

            visitExpr2(expr->expression.multop_->rhs);
			
			if(tag!=0)
				Parts(expr->expression.multop_->rhs);
			else{
				Parts3[tam][0] = aux2->chars;
				tam++;
				for(int i = pont;i<tam-1;i++){
					if(i>0){
						Parts3[i][3]= Parts3[i-1][0];
					}
				}
				for(pont; pont<tam;pont++){
					insert(aux, "child", Parts3[pont][0]);
					insert(aux, "child", "=");
					insert(aux, "child", Parts3[pont][1]);
					insert(aux, "child", Parts3[pont][2]);
					insert(aux, "child", Parts3[pont][3]);
					if(!pont+1==tam)
						insert(aux, "child", ";");
				}
			}
			
            break;
			
		case eRela:
            visitExpr2(expr->expression.relaop_->lhs);
            switch(expr->expression.relaop_->r) {
                case eLT:
                    insert(aux, "child", " < ");
                    break;

                case eGT:
                    insert(aux, "child", " > ");
                    break;

                case eLE:
                    insert(aux, "child", " <= ");
                    break;

                case eGE:
                    insert(aux, "child", " >= ");
                    break;
            }
            visitExpr2(expr->expression.relaop_->rhs);
            break;	
			
		case eEqlt:
            visitExpr2(expr->expression.eqltop_->lhs);
            if(expr->expression.eqltop_->e == eEQ) {
                insert(aux, "child", " == ");
            } else {
                insert(aux, "child", " != ");
            }
            visitExpr2(expr->expression.eqltop_->rhs);
            break;
			
		case eCallExpr:
            visitCallStmt2(expr->expression.call_);
            break;
			
		case eExpr:
            insert(aux, "child", "(");
            visitExpr2(expr->expression.bracket);
            insert(aux, "child", ")");
            break;
			
		case eId:
            visitId_s2(expr->expression.ID_);
            break;
			
		case eIntnum:
			if(numero != NULL){
            	//printf("%d", expr->expression.intnum);
				snprintf(numero->num, 255*sizeof(char), "%d", (int)expr->expression.intnum);
            	insert(aux, "child", numero->num);
				numero = numero->prox;
			}else if(numero == NULL){
				Numero* novo = (Numero*)malloc(sizeof(Numero));
				numero = novo;
				snprintf(numero->num, 255*sizeof(char), "%d", (int)expr->expression.intnum);
				insert(aux, "child", numero->num);
				numero = numero->prox;
			} 
            break;

		case eFloatnum:
			if(numero != NULL){
				snprintf(numero->num, 255*sizeof(char), "%f", (float)expr->expression.floatnum);
            	insert(aux,"child", numero->num);
				numero = numero->prox;
			}else if(numero == NULL){
				Numero* novo = (Numero*)malloc(sizeof(Numero));
				numero = novo;
				snprintf(numero->num, 255*sizeof(char), "%f", (float)expr->expression.floatnum);
                insert(aux,"child", numero->num);
                numero = numero->prox;
			}
            break;	
    }
}	

void visitWhile_s2       (struct WHILE_S* while_s) {
    if(while_s->do_while == true) {
        //making node for symbol table
        scopeTail = newScope(sDOWHILE, scopeTail);
        _isTitlePrinted2 = false;
        scopeTail->parent->dowhile_n++;

        //printf("do");
		insert(aux, "child", "(");
        visitStmt2(while_s->stmt);
        //printf("while (");
		insert(aux, "child", "(");
		aux2 = aux;
        visitExpr2(while_s->cond);
		insert(aux,"child",")?");
        //printf(");\n");
		insert(aux, "child", ")*;¬(");
		aux2 = aux;
		visitExpr2(while_s->cond);
		insert(aux,"child",")? ");
		
    } else {
        //making node for symbol table
        scopeTail = newScope(sWHILE, scopeTail);
        _isTitlePrinted2 = false;
        scopeTail->parent->while_n++;

        //printf("while (");
		insert(aux, "child", "( (");
		aux2 = aux;
        visitExpr2(while_s->cond);
		insert(aux,"child",")?");
        //printf(")\n");
		//insert(aux, "child", ")\n");
        visitStmt2(while_s->stmt);
    	insert(aux, "child", ")*;  ¬(");
		aux2 = aux;
        visitExpr2(while_s->cond);
        insert(aux,"child",")?");
    
	}

    //deleteCurScope 
    deleteScope(&scopeTail);
}
void visitFor_s2         (struct FOR_S* for_s) {
    //making node for symbol table
    scopeTail = newScope(sFOR, scopeTail);
    _isTitlePrinted2 = false;
    scopeTail->parent->for_n++;

    //printf("for (");
	insert(aux, "child","(");
    visitAssignStmt2(for_s->init);
	insert(aux,"child",";(");
	aux2 = aux;
    visitExpr2(for_s->cond);
    insert(aux,"child",")?");
    //printf("; ");
	insert(aux, "child", "; ");
    //printf("; ");
	visitStmt2(for_s->stmt);
	insert(aux,"child", "; ");
    visitAssignStmt2(for_s->inc);
    //printf(")\n");
	insert(aux,"child",")*;¬(");
	aux2 = aux;
	visitExpr2(for_s->cond);
	insert(aux,"child",")?");

    //deleteCurScope 
    deleteScope(&scopeTail);
}
void visitIf_s2          (struct IF_S* if_s) {
	    //making node for symbol table
    scopeTail = newScope(sIF, scopeTail);
    _isTitlePrinted2 = false;
    scopeTail->parent->if_n++;

    insert(aux, "child", "( (");
	aux2 = aux;
    visitExpr2(if_s->cond);
	insert(aux,"child",")? ");
	if(if_s->if_->s){
		insert(aux,"child",";");
	}
    visitStmt2(if_s->if_);
    insert(aux,"child"," )");
    if (if_s->else_ != NULL) {
        //printf("\nelse\n");
        insert(aux, "child", "U(¬(");
		aux2 = aux;
        visitExpr2(if_s->cond);
        insert(aux,"child",")? ");
		if(if_s->else_->s){
			insert(aux,"child",";");
		}
        visitStmt2(if_s->else_);
		insert(aux, "child", ")");
    } 
}
void visitId_s2          (struct ID_S* id_s) {
   //printf("%s",id_s->ID);
   rows = tag;
   if(tag==0){
	   Parts3[0][3]= id_s->ID;
   }
   else if(tag==1){
	   Parts3[0][1]= id_s->ID;
   }
   else {
	   rows--;
	   Parts3[rows][1]= id_s->ID;
   }

   if(id_s->expr != NULL) {
    //printf("[");
	insert(aux, "child", "[");
	aux2 = aux;
    visitExpr2(id_s->expr);
    //printf("]");
	insert(aux, "child", "]");
   }
}

void InsertSemicolon(struct STMT* stmt){
	if(stmt->prev !=NULL){
		struct STMT* stmtAux = stmt->prev;
		switch(stmtAux->s){

			case eWhile:
				insert(aux, "child", ";");
				return;
			case eFor:
				insert(aux, "child", ";");	
				return;
			case eIf:
				insert(aux, "child", ";");
				return;
			case eAssign:
				insert(aux, "child", ";");
				break;
			case eCall:
				insert(aux, "child", ";");
				break;
			case eSemi:
				insert(aux, "child", ";");
				break;

		}
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