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
int y = 0;
int tag = 0;
int id_tag = 0;
int tam = 0;
int tamMethod = 0;
int rows;
char *Parts3[100][4];
char *methodVector[100][2];
int tamAux = 0;


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
 
void visitDeclaration2   (struct DECLARATION* decl) {
    _isParam2 = false;   //needed when we have to decide it is parameter or variable.
    if(decl->prev != NULL) {
        visitDeclaration2(decl->prev);
    }
    if(!_isTitlePrinted2) {
        _isTitlePrinted2 = true;
    }

    switch(decl->t) {
        case eInt:
            //printf ("int ");
			//insert(aux, "child", "int ");
            _curType2 = eInt;
            break;
        case eFloat:
            //printf ("float ");
			//insert(aux, "child", "float ");
            _curType2 = eFloat;
            break;
        default:
            //printf("Declaration does not exist.\n");
            exit(1);
    }
    _needPrinted2 = true;
    visitIdentifier2(decl->id);
    _needPrinted2 = false;
    //printf (";\n");
	//insert(aux, "child", ";");
}
void visitFunction2      (struct FUNCTION* func) {
	 if(func->prev != NULL) {
        visitFunction2(func->prev);
    }
    //for symboltable
    _curFuncName = func->ID;
    //list node
    scopeTail = newScope(sFUNC, scopeTail); //append it to the end of list

    switch(func->t) {
        case eInt:
            //printf ("int ");
			//insert(aux, "child", "int ");
            break;
        case eFloat:
            //printf ("float ");
			//insert(aux, "child", "float ");
            break;
        default:
            //printf("Declaration does not exist.\n");
            exit(1);
    }
	if(strcmp(func->ID, "main")){
        int present = 0;
        for(int i = 0; i < tamMethod; i++){
            if(!strcmp(func->ID, methodVector[i][1])){
                present = 1;
                insert(aux, "child", methodVector[i][0]);
            }
        }
        if(present == 0){methodsFunction(func->ID);}
		insert(aux, "child", "=");
	}
	insert(aux, "child", "(");
	_isTitlePrinted2 = false;
    if(func->param != NULL) {
        _isTitlePrinted2 = true;
        visitParameter2(func->param);    //parameter 
    }
    //printf (")\n");//function name
	visitCompoundStmt2(func->cstmt); //compoundStmt
    insert(aux, "child", ")");
	//printf("\n");
	//insert(aux, "child", "\n");
	insert(aux,"child","\n");
    //deleteCurScope 
    deleteScope(&scopeTail);
    _isTitlePrinted2 = false;
}
void visitIdentifier2    (struct IDENTIFIER* iden) {
    if(iden->prev != NULL) {
        visitIdentifier2(iden->prev);
       // printf(", ");
		//insert(aux, "child", ", ");
    }
	if(( _curType2 == eInt) && (_isParam2 == false)) insert(aux,"child","int ");
	if(( _curType2 == eFloat) && (_isParam2 == false)) insert(aux,"child","float ");	
    //printf ("%s", iden->ID);
	insert(aux, "child", iden->ID);
	insert(aux,"child",";");
    if(iden->intnum > 0) {
        //printf ("[%d]", iden->intnum);
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
            //fprintf( fp2, "%10d%10s%10s%10d%10s\n", _rowNumber++ , curType, iden->ID, iden->intnum, _isParam2 ? "parameter" : "variable");
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
            //fprintf( fp2, "%10d%10s%10s%10s%10s\n", _rowNumber++ , curType, iden->ID, "", _isParam2 ? "parameter" : "variable"); //_rowNumber(x) ++_rowNumber(x) _rowNumber++(o)
        }
    }
}
void visitStmt2          (struct STMT* stmt) {
    //TODO
    if(stmt->prev != NULL)
        visitStmt2(stmt->prev);
	
    switch(stmt->s) {
        case eAssign:
			InsertSemicolon(stmt);
            visitAssignStmt2(stmt->stmt.assign_);
            //printf(";");
            break;

        case eCall:
			InsertSemicolon(stmt);
            visitCallStmt2(stmt->stmt.call_);
            //printf(";");
            break;

        case eRet:
			InsertSemicolon(stmt);
            if(stmt->stmt.return_ == NULL){
                //printf ("return;");
				insert(aux, "child", "return;");
            }
            else {
                //printf ("return ");
				aux2 = aux;
				insert(aux, "child", "return ");
                visitExpr2(stmt->stmt.return_);
                //printf (";");
				//insert(aux, "child", ";");
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
            //break;

        case eSemi:
            //printf(";");
			InsertSemicolon(stmt); 
            break;

    }
    //printf("\n");
	//insert(aux, "child", "\n");
}
void visitParameter2     (struct PARAMETER* param) {
    _isParam2 = true;
    if(param->prev != NULL) {
        visitParameter2(param->prev);
        //printf (", ");
		insert(aux, "child", ", ");
    }
    switch(param->t) {
        case eInt:
            //printf ("int ");
			insert(aux, "child", "int ");			
            _curType2 = eInt;
            break;
        case eFloat:
            //printf ("float ");
			insert(aux, "child", "float ");
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
void visitCompoundStmt2  (struct COMPOUNDSTMT* cstmt) {
    if(_isCompound2 == true) {
        //making node for symbol table
        scopeTail = newScope(sCOMPOUND, scopeTail);
        _isTitlePrinted2 = false;
        scopeTail->parent->compound_n++;
    }
    _isOtherComp2 = false;

    //printf("{\n");
	//insert(aux, "child", "{\n");
    if(cstmt->decl != NULL) { 
        visitDeclaration2(cstmt->decl);
    }
    if(cstmt->stmt != NULL)
        visitStmt2(cstmt->stmt);
    //printf("}\n");
	//insert(aux, "child", "}\n");

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
        //printf("[");
		insert(aux, "child", "[");
        visitExpr2	(assign->index);
        //printf("]");
		insert(aux, "child", "]");
    }
    //printf(" = ");
	switch(assign->expr->e){
		case eRela:
			insert(aux, "child", assign->ID);
			break;
		case eIntnum:
			insert(aux, "child", assign->ID);
			insert(aux, "child", "=");
			break;
		case eFloatnum:
			insert(aux, "child", assign->ID);
			insert(aux, "child", "=");
			break;
		case eId:
			insert(aux, "child", assign->ID);
			insert(aux, "child", "=");
			break;
	}

    visitExpr2(assign->expr);
}
void visitCallStmt2      (struct CALL* call) {
    //printf("%s(", call->ID);
    int present = 0;
    for(int i = 0; i < tamMethod; i++){
        if(!strcmp(call->ID, methodVector[i][1])){
            present = 1;
            insert(aux, "child", methodVector[i][0]);
        }
    }
    if(present == 0){methodsFunction(call->ID);}
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
            //printf("-");
			insert(aux, "child", "-");
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
					insert(aux, "child", Parts3[tamAux][0]);
					insert(aux, "child", "=");
					insert(aux, "child", Parts3[tamAux][1]);
					insert(aux, "child", Parts3[tamAux][2]);
					insert(aux,"child",  Parts3[tamAux][3]);
					if(!(tamAux+1==tam))
						insert(aux, "child", ";");
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
				for(int i = tamAux;i<tam-1;i++){
					if(i>tamAux){
						Parts3[i][3]= Parts3[i-1][0];
					}
				}
				for(tamAux; tamAux<tam;tamAux++){
					insert(aux, "child", Parts3[tamAux][0]);
					insert(aux, "child", "=");
					insert(aux, "child", Parts3[tamAux][1]);
					insert(aux, "child", Parts3[tamAux][2]);
					insert(aux, "child", Parts3[tamAux][3]);
					if(!tamAux+1==tam)
						insert(aux, "child", ";");
				}
				id_tag = 0;
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
			switch(id_tag){
				case 0:
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
				case 1:
				if(numero != NULL){
					//printf("%d", expr->expression.intnum);
					snprintf(numero->num, 255*sizeof(char), "%d", (int)expr->expression.intnum);
					rows = tag + tam;
					if(tag==0){
					   Parts3[tamAux][3]= numero->num;
					}
					else if(tag==1){
					   Parts3[tamAux][1]= numero->num;
					}
					else {
					   Parts3[rows-1][1]= numero->num;
					}
					numero = numero->prox;
				}else if(numero == NULL){
					Numero* novo = (Numero*)malloc(sizeof(Numero));
					numero = novo;
					snprintf(numero->num, 255*sizeof(char), "%d", (int)expr->expression.intnum);
					rows = tag + tam;
					if(tag==0){
					   Parts3[tamAux][3]= numero->num;
					}
					else if(tag==1){
					   Parts3[tamAux][1]= numero->num;
					}
					else {
					   Parts3[rows-1][1]= numero->num;
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
				case 1:
				if(numero != NULL){
					snprintf(numero->num, 255*sizeof(char), "%f", (float)expr->expression.floatnum);
					rows = tag + tam;
                    if(tag==0){
					   Parts3[tamAux][3]= numero->num;
					}
					else if(tag==1){
					   Parts3[tamAux][1]= numero->num;
					}
					else {
					   Parts3[rows-1][1]= numero->num;
					}
					numero = numero->prox;
				}else if(numero == NULL){
					Numero* novo = (Numero*)malloc(sizeof(Numero));
					numero = novo;
					snprintf(numero->num, 255*sizeof(char), "%f", (float)expr->expression.floatnum);
					rows = tag + tam;
                    if(tag==0){
					   Parts3[tamAux][3]= numero->num;
					}
					else if(tag==1){
					   Parts3[tamAux][1]= numero->num;
					}
					else {
					   Parts3[rows-1][1]= numero->num;
					}
					numero = numero->prox;
				}
				break;
				
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
        visitStmt2(while_s->stmt);
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
   rows = tag + tam;
   if(id_tag==0){
	   insert(aux,"child",id_s->ID);
   }
   else{
		if(tag==0){
			Parts3[tamAux][3]= id_s->ID;
		}
		else if(tag==1){
			Parts3[tamAux][1]= id_s->ID;
		}
		else {
			Parts3[rows-1][1]= id_s->ID;
		}  
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

void methodsFunction(char* ID){
    char var[] = "_yX";
	int buffersize = 100;
	char* variable = malloc(buffersize);
    var[2] = ++y + '0';
    strncpy(variable,var,buffersize);
    methodVector[tamMethod][0] = variable;
    methodVector[tamMethod][1] = ID;
    insert(aux, "child", methodVector[tamMethod][0]);
    tamMethod++;
}
