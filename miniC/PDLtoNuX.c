#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PDLtoNux.h"

int treeTam = 0;
int notTam = 0;
int notEndTam = 0;
int notChildsTam;
int isCondition = 0;
int isNotDet = 0;
int isEndDet = 0;
int isCheck = 0;
int notDeterministic[100][100];
int notEnd[100];
char *treeElements[100];
char *noString;
char *first;
FILE* fp;
int isValue = 0;

void conv(No *no,Nolist* novalue){
    fp = fopen("nuX.smv","w");
    if(no != NULL){
        mod(no,novalue);
    }
}

void mod(No *no,Nolist* novalue){
        fprintf(fp, "MODULE main\n");
        var(no,novalue);
}

void var(No *no,Nolist *novalue){
    fprintf(fp, "VAR\n\tprog : {");
    prog(no);
    progvalue(novalue);
    Assign();
}

void prog(No *no){
    if(no != NULL){
        if(!strcmp(no->chars, ")")){
            isEndDet = 1;
        }
        if(strcmp(no->chars, ";") && strcmp(no->chars, "(") && strcmp(no->chars, ")") && strcmp(no->chars, ")\n") && strcmp(no->chars, "_") && strcmp(no->chars, "*")) {
            if(!strcmp(no->chars, "U")){
                isNotDet = 1;
            }
            else if(!strcmp(no->chars, "?")){
                isCondition = 1;
            }
            treeElements[treeTam] = stringMemory(treeElements[treeTam], no->chars);
        }

        if(no->child!=NULL){
            if(strcmp(no->child->chars, ";") && strcmp(no->chars, "(") && strcmp(no->child->chars, "(") && strcmp(no->child->chars, ")") && strcmp(no->child->chars, ")\n") && strcmp(no->child->chars, "*")){
                treeElements[treeTam] = stringMemory(treeElements[treeTam], "_");
            }
            prog(no->child);
        }
        else{
            if(isCondition == 0 && treeElements[treeTam]!=NULL){
                fprintf(fp, "%s_%d", treeElements[treeTam], treeTam);
                treeTam++;
            }
            else{
                treeElements[treeTam] = NULL;
            }
        }
        if(no->parent!=NULL){
            if(isCondition == 0 && isEndDet == 1){
                notEnd[notEndTam] = treeTam;
                notEndTam++;
            }
            else if(isCondition == 0){
                fprintf(fp, ", ");
            }
            else if(isNotDet == 1 && isCondition == 1){
                notEndTam--;
                notDeterministic[notTam-1][notChildsTam] = treeTam;
                notChildsTam++;
                fprintf(fp, ", ");
            }
            else if(isCondition == 1){
                notDeterministic[notTam][0] = treeTam - 1;
                notDeterministic[notTam][1] = treeTam;
                notChildsTam = 2;
                notTam++;
            }
            isCondition = 0;
            isNotDet = 0;
            isEndDet = 0;
            prog(no->parent);
        }
    }
}

void progvalue(Nolist *nolist){
    //Acessar Declaration e separar o seu tipo e o nome da variavel
    /*
    char* aux = no->chars;
    char** decl = NULL;
    Split(aux,decl);

    //adicionar o maior numero possivel de seu tipo
    if(decl[0] == int){
        fprintf(fp, \n\t%s : 0..2147483647",decl[1]);
    }else if( decl[0] == float){
        fprintf(fp, \n\t%s : 0.00000..9.99999 , decl[1]); //nao tenho ctz dessa syntax(Olhar documentacao)
    }else if( decl[0] == double){
        fprintf(fp, \n\t%s : 0.00000..9.99999, decl[1]);
    }
    */
}

void Assign(){
    fprintf(fp, "}\nASSIGN\n\tinit(prog) := {");

    if(notTam>0 && notDeterministic[0][0]==-1){
        int m = 2;
        fprintf(fp, "%s_%d", treeElements[notDeterministic[0][1]], notDeterministic[0][1]);
        while(notDeterministic[0][m]){
            fprintf(fp, ", %s_%d", treeElements[notDeterministic[0][m]], notDeterministic[0][m]);
            m++;
        }
    }
    else{
        fprintf(fp, "%s", treeElements[0]);
    }

    fprintf(fp, "}\n\tnext(prog) := case");

    elements();

    fprintf(fp, "\n\t\t\t\tesac;");
}


void elements(){
    for(int i = 0; i<treeTam; i++){
        if(treeElements[i+1] != NULL){
            int isPresent = 0;
            int k = 0;
            for(int j = 0; j<notTam; j++){
                if(i ==  notDeterministic[j][0]){
                    isPresent = 1;
                    k = j;
                }
            }
            if(isPresent == 1 && notDeterministic[k][2] != 0){
                int m = 1;
                fprintf(fp, "\n\t\t\t\t\tprog = %s_%d: {", treeElements[i], i);
                while(notDeterministic[k][m]){
                    if(m!=1){
                        fprintf(fp, ", ");
                    }
                    fprintf(fp, "%s_%d", treeElements[notDeterministic[k][m]], notDeterministic[k][m]);
                    m++;
                }
                fprintf(fp, "}");
            }
            else {
                int end = 0;
                int m = 0;
                for(int j = 0; j < notTam; j++){
                    int n = 2;
                    while(notDeterministic[j][n]){
                        if(notDeterministic[j][n]-1 == i){
                            end = 1;
                            m = j;
                        }
                        n++;
                    }
                }
                if(end == 1){
                    if(treeElements[notEnd[m]]!=NULL){
                        fprintf(fp, "\n\t\t\t\t\tprog = %s_%d: %s_%d;", treeElements[i], i, treeElements[notEnd[m]], notEnd[m]);
                    }
                }
                else{
                    fprintf(fp, "\n\t\t\t\t\tprog = %s_%d: %s_%d;", treeElements[i], i, treeElements[i+1], i+1);
                }
            }
        }
    }
}

char* stringMemory(char *aux, char *str){
    if(aux == NULL){
        return str;
    }
    else{
        char* copAux = (char *) malloc(1 + strlen(aux) + strlen(str));
        strcpy(copAux, aux);
        strcat(copAux, str);
        return copAux;
    }
}

void Split(char* chars,char** res){

	char str[100];
	strcpy(str,chars);
	char *  p    = strtok (str, " ");
	int n_spaces = 0, i;

	while (p) {
    	  res = realloc (res, sizeof (char*) * ++n_spaces);

    	  if (res == NULL)
    		exit (-1); /* memory allocation failed */

    	  res[n_spaces-1] = p;

    	  p = strtok (NULL, " ");
   	}

   	res = realloc (res, sizeof (char*) * (n_spaces+1));
    res[n_spaces] = 0;
}
