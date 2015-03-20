#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "lexer.h"

int change=0;

struct node{
char a[20];
struct node* parent;
struct node* down;
struct node* next;
};


struct parseNode{
char name[20];
char val[255];
struct parseNode *parent;
struct parseNode *down;	
struct parseNode *next;
};

struct xnode{
struct node *c1;
struct parseNode *c2;
};
#define MAXSIZE 500
 
struct stack
{
    struct parseNode *stk[MAXSIZE];
    int top;
};
typedef struct stack STACK;
struct stack1
{
    struct xnode *stk[MAXSIZE];
    int top;
};
typedef struct stack1 STACK1;

STACK s;
STACK1 s1; 

void push (struct parseNode *p)
{
    int num;
    if (s.top == (MAXSIZE - 1))
    {
        printf ("Stack is Full\n");
        return;
    }
    else
    {
       s.top = s.top + 1;
        s.stk[s.top] = p;
    }
    return;
}
struct parseNode *pop ()
{
    int num;
    if (s.top == - 1)
    {
        return NULL;
    }
    else
    {
		s.top = s.top - 1;
        return s.stk[s.top+1];
     }
}
void push1 (struct node *p,struct parseNode *p1)
{
    int num;
    if (s1.top == (MAXSIZE - 1))
    {
        printf ("Stack is Full\n");
        return;
    }
    else
    {
       s1.top = s1.top + 1;
		s1.stk[s1.top] = (struct xnode *)malloc(sizeof(struct xnode));
        s1.stk[s1.top]->c1 = p;
		s1.stk[s1.top]->c2 = p1;
    }
    return;
}
struct xnode *pop1 ()
{
    int num;
    if (s1.top == - 1)
    {
        return NULL;
    }
    else
    {
		s1.top = s1.top - 1;
        return s1.stk[s1.top+1];
     }
}


void error(){
printf("\n\n****Error here****\nMISMATCH\n\n");
exit(0);
}


struct parseNode *convertParse(struct node *root){
struct xnode *x;
struct parseNode *parseTree;
struct parseNode *trav,*trav1,*trav2;
parseTree = (struct parseNode *)malloc(sizeof(struct parseNode));
push1(root,parseTree);
while((x = pop1()) && x!=NULL){
if(x->c2->parent==NULL){
//printf("Check this out*****************%s\n",x->c1->a);
}
strcpy(x->c2->name,x->c1->a);
trav = x->c2;
struct node *c = x->c1->down;
if(c!=NULL){
trav->down = (struct parseNode *)malloc(sizeof(struct parseNode));
trav1 = trav->down;
trav1->parent = trav;
push1(c,trav1);
for(;c->next!=NULL;c=c->next){
trav1->next = (struct parseNode *)malloc(sizeof(struct parseNode));
trav1 = trav1->next;
trav1->parent = trav;
push1(c->next,trav1);
}
}
}
return parseTree;
}

void traverse1(struct parseNode* start,int tab){
int i;
struct parseNode *head;
head = (struct parseNode *)malloc(sizeof(struct parseNode));
head=start;
for(i=0;i<tab;i++) printf("     ");
printf("%s\n",head->name);
head=head->down;
while(head!=NULL){
	traverse1(head,tab+1);
	head=head->next;
	}
}

struct parseNode *ReduceExpr(struct parseNode *root){
push(root);
struct parseNode *u,*u1;
while((u = pop()) && u!=NULL){
if(strcmp(u->name,"ADD")==0||strcmp(u->name,"SUB")==0||strcmp(u->name,"MUL")==0||strcmp(u->name,"DIV")==0||strcmp(u->name,"ASSIGN")==0){
	struct parseNode *BQ,*DQ,*AQ,*CQ,*XQ;
	BQ = u->parent;
	XQ = u->next;
	AQ = u->parent->parent;
	DQ = AQ->down;
	CQ = AQ->parent;

	DQ->next = u->next;
	u->next->parent = AQ;

	strcpy(AQ->name,u->name);
}else{
for(u1=u->down;u1!=NULL;u1=u1->next)
push(u1);
}
}
return root;
}


void traverse(struct node* start,int tab){
int i;
struct node *head;
head = (struct node*)malloc(sizeof(struct node));
head=start;
for(i=0;i<tab;i++) printf("     ");
printf("%s\n",head->a);
head=head->down;
while(head!=NULL){
	traverse(head,tab+1);
	head=head->next;
	}
}




void ast(struct node* start, int tab){
int i;
struct node *head;
head = (struct node*)malloc(sizeof(struct node));
head=start;
if(head==NULL) return;
if((head->a)[0]=='<'){			//move up
	if(head->down!=NULL){
		if((head->down->a)[0]!='<'){
			if(head->down->next==NULL){
				//printf("Yo %s\n",head->a);
				strcpy(head->a,head->down->a);
				head->down=NULL;
				ast(head,tab);
				return;
			}	
		}
		else{
			if(head->down->next==NULL){
				head->down->next=head->next;
				head->down->parent=head->parent;
				*head=*head->down;
				ast(head,tab);
				return;
			}
		}
	}
}
//printf("head->a is =%s\n",head->a);
if(head->a[0]=='<' && head->a[1]=='i'){
	//printf("head->a=%s head->down->next=%s\n",head->a,head->down->next->a);
}
if(head->down!=NULL){
	if(head->down->down==NULL && (head->down->a)[0]=='<'){
		//printf("Inside head->a=%s\n",head->a);
		head->down=head->down->next;
		ast(head->parent,tab);
		return;
	}
	else head=head->down;
}else {
	if(head->a[0]!='<') return;
}
//printf("head->a is =%s\n",head->a);
while(head!=NULL){
	ast(head,tab+1);
	while(1){
		if(head->next!=NULL){
			if(head->next->down==NULL && (head->next->a)[0]=='<')
			{
				head->next=head->next->next;
				ast(head->parent->parent,tab);
				return;
			}
			else
				break;	
		}
		else{
			break;
		}
	}
	if(head!=NULL) head=head->next;
}
return;
}

void astFurCond(struct node* start, int tab){
int i;
struct node *head;
head = (struct node*)malloc(sizeof(struct node));
head=start;
if(!strcmp(head->a,"<furCond>")){
	strcpy(head->a,head->down->next->a);
	*(head->down->next)=*(head->down->next->next);
	return;	
}
head=head->down;
while(head!=NULL){
	astFurCond(head,tab+1);
	head=head->next;
}
}

void astObjOrVec(struct node* start, int tab){
int i;
struct node *head;
head = (struct node*)malloc(sizeof(struct node));
head=start;
if(!strcmp(head->a,"<objOrVec>")){
	if(!strcmp(head->down->a,"<vecAss>")){
		*head=*(head->down);	
	}
	return;	
}
head=head->down;
while(head!=NULL){
	astObjOrVec(head,tab+1);
	head=head->next;
}
}

void astRemoveSep(struct node* start, int tab){
int i;
struct node *head;
head = (struct node*)malloc(sizeof(struct node));
head=start;
if((head->a)[0]!='<'){
	if(!strcmp(head->a,"SEP") || !strcmp(head->a,"LP") || !strcmp(head->a,"RP") || !strcmp(head->a,"FS") || !strcmp(head->a,"FE") || !strcmp(head->a,"LC") || !strcmp(head->a,"RC") || !strcmp(head->a,"SMC") || !strcmp(head->a,"EOP") || !strcmp(head->a,"COMMA") || !strcmp(head->a,"INT") || !strcmp(head->a,"FLOAT") || !strcmp(head->a,"OBJECT") || !strcmp(head->a,"VECTOR") || !strcmp(head->a,"LINEAR")){
			if(head->parent->down==head){
				change=1;
				*head->parent->down=*head->next;
				head=NULL;
				return;
			}else{
				change=1;
				struct node *buf;
				buf = head->parent->down;
				while(buf->next!=head){/*printf(buf->a,buf->parent->a);*/buf=buf->next;}
				buf->next=head->next;	
			}
	}
}
head=head->down;
while(head!=NULL){
	astRemoveSep(head,tab);
	head=head->next;
}
}

void astAssign(struct node* start, int tab){
int i;
struct node *head;
head = (struct node*)malloc(sizeof(struct node));
head=start;
return;
}

char *incid(char a[]){
int len = strlen(a);
int y;
int carry = 1;
for(y=len-1;y>=2;y--){
	if(carry==1){
		if(a[y]!='9'){
			a[y] = (char)(a[y] + 1);
			carry = 0;
		}
	else {
	a[y]='0';
	}
}
}
if(carry==1){
	for(y=len;y>=2;y--){
		a[y+1] = a[y];
	}
	a[2] = '1';
}
return a;
}

char *incin(char a[]){
int len = strlen(a);
int y;
int carry = 1;
for(y=len-1;y>=8;y--){
	if(carry==1){
		if(a[y]!='9'){
			a[y] = (char)(a[y] + 1);
			carry = 0;
		}
	else {
	a[y]='0';
	}
}
}
if(carry==1){
	for(y=len;y>=8;y--){
		a[y+1] = a[y];
	}
	a[8] = '1';
}
return a;
}

char *incfl(char a[]){
int len = strlen(a);
int y;
int carry = 1;
for(y=len-1;y>=10;y--){
	if(carry==1){
		if(a[y]!='9'){
			a[y] = (char)(a[y] + 1);
			carry = 0;
		}
	else {
	a[y]='0';
	}
}
}
if(carry==1){
	for(y=len;y>=10;y--){
		a[y+1] = a[y];
	}
	a[10] = '1';
}
return a;
}

void backToRoot(int i){printf(" ");}

int main(int argc, char *argv[]){
if(argc!=2){
exit(0);
}	
FILE *fp;
char buff[250];
memset(buff,'\0',250);
strcat(buff,"LexerResultFor");
strcat(buff,argv[1]);
FILE *fp1 = fopen(buff,"w+");
fp = fopen(argv[1],"r+");
struct idNode *hashTable[hashSize] = {NULL};
struct idNode *listPoint;
struct lineNode *list;
list = lexer(fp,list,hashTable);
struct lineNode *list1;
struct tokenNode *trav;
list1 = list;
int idT=0;
int idI=0;int idF=0;
for(list1=list;list1!=NULL;list1=list1->next){
for(trav = list1->tokenList;trav->next!=NULL;trav=trav->next){
if(strcmp(trav->token,"ID")==0){
fprintf(fp1,"\"%s%d\",",trav->token,idT);
idT++;
}
else if(strcmp(trav->token,"INTCONST")==0){
fprintf(fp1,"\"%s%d\",",trav->token,idI);
idI++;
}
else if(strcmp(trav->token,"FLOATCONST")==0){
fprintf(fp1,"\"%s%d\",",trav->token,idF);
idF++;
}
else{
fprintf(fp1,"\"%s\",",trav->token);
}
}
fprintf(fp1,"\"%s\"\n",trav->token);
}
int size = getSize(list);
char *t[size];
getArray(t,list);
fclose(fp);
fclose(fp1);
int i;
//printf("obtained array\n");
for(i=0;i<size;i++){
printf("%s,",t[i]);
}
int buf=0;
i=0;
//char t[][20]  = {"INIT","LP","RP","SEP","FS","SEP","OBJECT","ID0","ASSIGN","LC","FLOATCONST0","COMMA","FLOATCONST1","SMC","FLOATCONST2","COMMA","FLOATCONST3","SMC","FLOATCONST4","COMMA","FLOATCONST5","SMC","FLOATCONST6","RC","SEP","FLOAT","ID1","ASSIGN","FLOATCONST7","SEP","FS","ID2","ASSIGN","ID3","ADD","ID4","SEP","FE","WHEN","LP","NOT","LP","TIME","EC","INTCONST0","RP","AND","LP","TIME","GTE","TIME","RP","RP","SEP","OUTA","LP","ID5","RP","AT","LP","LP","TIME","EC","INTCONST1","RP","RP","SEP","FE","SEP"};
char id[20] = "ID0";
char in[20] = "INTCONST0";
char fl[20] = "FLOATCONST0";
struct node *start;
start = (struct node*)malloc(sizeof(struct node));
strcpy(start->a,"<program>"); 
start->parent = NULL;
start->down = NULL;
start->next = NULL;
int no_of_tokens = size;
//printf("no_of_tokens = %d\n",no_of_tokens);

struct node* head;
head = (struct node*)malloc(sizeof(struct node));
head=start;
//breakpoint here line:35
while(i<no_of_tokens){
//printf("head=%s t[%d]=%s id=%s in=%s fl=%s\n",head->a,i,t[i],id,in,fl);
if((head->a)[0]=='<'){

	if(!strcmp(head->a,"<SEPA>")){	
		if(!strcmp(t[i],"SEP")){
			//printf("Doing <SEPA> -> SEP <SEPA>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"SEP");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<SEPA>");
			newList2->parent=head;
			newList2->down=NULL;
						
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"INIT")) || (!strcmp(t[i],"INT")) || (!strcmp(t[i],"FLOAT")) || (!strcmp(t[i],"OBJECT")) || (!strcmp(t[i],"VECTOR")) || (!strcmp(t[i],"LINEAR")) || (!strcmp(t[i],id)) || (!strcmp(t[i],"FS")) || (!strcmp(t[i],"FE"))){
			//printf("Doing <SEPA> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
		
	}
//breakpoint here line:54
	else if(!strcmp(head->a,"<program>")){	
		if((!strcmp(t[i],"INIT")) || (!strcmp(t[i],"INT")) || (!strcmp(t[i],"FLOAT")) || (!strcmp(t[i],"OBJECT")) || (!strcmp(t[i],"VECTOR")) || (!strcmp(t[i],"LINEAR"))){
			//printf("Doing <program> -> <function> <SEPA> <program>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<function>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<SEPA>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<program>");
			newList3->parent=head;
			newList3->down=NULL;
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
			//printf("head->a=%s t[i]=%s\n",head->a,t[i]);
		}
		else if(!strcmp(t[i],"EOP")){
			//printf("Doing <program> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}		
	}
	
	else if(!strcmp(head->a,"<function>")){	
//printf("ENtered here\n");
		if(!strcmp(t[i],"INIT")){
			//printf("Doing <function> -> INIT LP RP <SEPA> FS <SEPA> <functionBody> FE\n");
			struct node *newList1, *newList2, *newList3, *newList4, *newList5, *newList6, *newList7, *newList8;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"INIT");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"LP");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RP");
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"<SEPA>");
			newList4->parent=head;
			newList4->down=NULL;
			newList5 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList5->a,"FS");
			newList5->parent=head;
			newList5->down=NULL;
			newList6 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList6->a,"<SEPA>");
			newList6->parent=head;
			newList6->down=NULL;
			newList7 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList7->a,"<functionBody>");
			newList7->parent=head;
			newList7->down=NULL;
			newList8 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList8->a,"FE");
			newList8->parent=head;
			newList8->down=NULL;

			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=newList5;
			newList5->next=newList6;
			newList6->next=newList7;
			newList7->next=newList8;
			newList8->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"INT")) || (!strcmp(t[i],"FLOAT")) || (!strcmp(t[i],"OBJECT")) || (!strcmp(t[i],"VECTOR")) || (!strcmp(t[i],"LINEAR"))){
			//printf("Doing <function> -> <dataType> ID LP <paramList> RP <SEPA> FS <SEPA> <functionBody> FE\n");
			struct node *newList1, *newList2, *newList3, *newList4, *newList5, *newList6, *newList7, *newList8, *newList9, *newList10;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<dataType>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,id);
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"LP");
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"<paramList>");
			newList4->parent=head;
			newList4->down=NULL;
			newList5 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList5->a,"RP");
			newList5->parent=head;
			newList5->down=NULL;
			newList6 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList6->a,"<SEPA>");
			newList6->parent=head;
			newList6->down=NULL;
			newList7 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList7->a,"FS");
			newList7->parent=head;
			newList7->down=NULL;
			newList8 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList8->a,"<SEPA>");
			newList8->parent=head;
			newList8->down=NULL;
			newList9 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList9->a,"<functionBody>");
			newList9->parent=head;
			newList9->down=NULL;
			newList10 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList10->a,"FE");
			newList10->parent=head;
			newList10->down=NULL;

			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=newList5;
			newList5->next=newList6;
			newList6->next=newList7;
			newList7->next=newList8;
			newList8->next=newList9;
			newList9->next=newList10;
			newList10->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<functionBody>")){	
		if((!strcmp(t[i],"INT")) || (!strcmp(t[i],"FLOAT")) || (!strcmp(t[i],"OBJECT")) || (!strcmp(t[i],"VECTOR")) || (!strcmp(t[i],"LINEAR")) || (!strcmp(t[i],"COM")) || (!strcmp(t[i],"FS")) || (!strcmp(t[i],id)) || (!strcmp(t[i],"OUT")) || (!strcmp(t[i],"OUTA")) || (!strcmp(t[i],"LOOP")) || (!strcmp(t[i],"RETURN")) || (!strcmp(t[i],"INF")) || (!strcmp(t[i],"ININ"))){
			//printf("Doing <functionBody> -> <stnt> SEP <functionBody> \n");
			struct node *newList1, *newList2, *newList3, *newList4;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<stnt>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"SEP");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<functionBody>");
			newList3->parent=head;
			newList3->down=NULL;			

			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"FE"))){
			//printf("Doing <functionBody> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<stnt>")){	
		if((!strcmp(t[i],"INT")) || (!strcmp(t[i],"FLOAT")) || (!strcmp(t[i],"OBJECT")) || (!strcmp(t[i],"VECTOR")) || (!strcmp(t[i],"LINEAR"))){
			//printf("Doing <stnt> -> <declStnt> <C>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<declStnt>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<C>");
			newList2->parent=head;
			newList2->down=NULL;			

			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"COM")){
			//printf("Doing <stnt> -> <C>\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<C>");
			newList1->parent=head;
			newList1->down=NULL;			

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"FS")){
			//printf("Doing <stnt> -> <condStnt> <C>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<condStnt>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<C>");
			newList2->parent=head;
			newList2->down=NULL;			

			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],id)){
			//printf("Doing <stnt> -> <assignPlusFunct> <C>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<assignPlusFunct>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<C>");
			newList2->parent=head;
			newList2->down=NULL;			

			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"LOOP")){
			//printf("Doing <stnt> -> <loop> <C>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<loop>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<C>");
			newList2->parent=head;
			newList2->down=NULL;			

			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"RETURN")){
			//printf("Doing <stnt> -> <returnCall> <C>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<returnCall>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<C>");
			newList2->parent=head;
			newList2->down=NULL;			

			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"OUT")) || (!strcmp(t[i],"OUTA"))){
			//printf("Doing <stnt> -> <outStnt> <C>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<outStnt>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<C>");
			newList2->parent=head;
			newList2->down=NULL;			

			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"INF")) || (!strcmp(t[i],"ININ"))){
			//printf("Doing <stnt> -> <inStnt> <C>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<inStnt>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<C>");
			newList2->parent=head;
			newList2->down=NULL;			

			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}

	}

	else if(!strcmp(head->a,"<dataType>")){	
		if(!strcmp(t[i],"INT")){
			//printf("Doing <dataType> -> INT \n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"INT");
			newList1->parent=head;
			newList1->down=NULL;			

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"FLOAT")){
			//printf("Doing <dataType> -> FLOAT \n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"FLOAT");
			newList1->parent=head;
			newList1->down=NULL;			

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"OBJECT")){
			//printf("Doing <dataType> -> OBJECT \n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"OBJECT");
			newList1->parent=head;
			newList1->down=NULL;			

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"LINEAR")){
			//printf("Doing <dataType> -> LINEAR \n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LINEAR");
			newList1->parent=head;
			newList1->down=NULL;			

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"VECTOR")){
			//printf("Doing <dataType> -> VECTOR \n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"VECTOR");
			newList1->parent=head;
			newList1->down=NULL;			

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<paramList>")){	
		if((!strcmp(t[i],"INT")) || (!strcmp(t[i],"FLOAT")) || (!strcmp(t[i],"OBJECT")) || (!strcmp(t[i],"VECTOR")) || (!strcmp(t[i],"COM"))){
			//printf("Doing <paramList> -> <dataType> ID <paramList2> \n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<dataType>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,id);
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<paramList2>");
			newList3->parent=head;
			newList3->down=NULL;			

			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"RP"))){
			//printf("Doing <paramList> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}

	}

	else if(!strcmp(head->a,"<paramList2>")){	
		if(!strcmp(t[i],"SMC")){
			//printf("Doing <paramList2> -> SMC <dataType> ID <paramList2> \n");
			struct node *newList1, *newList2, *newList3, *newList4;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"SMC");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<dataType>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,id);
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"<paramList2>");
			newList4->parent=head;
			newList4->down=NULL;			

			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"RP"))){
			//printf("Doing <paramList2> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}

	}

	else if(!strcmp(head->a,"<paramListN>")){	
		if(!strcmp(t[i],id)){
			//printf("Doing <paramListN> -> ID <paramListN2> \n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,id);
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<paramListN2>");
			newList2->parent=head;
			newList2->down=NULL;			

			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"RP"))){
			//printf("Doing <paramListN> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<paramListN2>")){	
		if(!strcmp(t[i],"SMC")){
			//printf("Doing <paramListN2>->SMC ID <paramListN2>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"SMC");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,id);
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<paramListN2>");
			newList3->parent=head;
			newList3->down=NULL;			

			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"RP"))){
			//printf("Doing <paramListN2> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL){
					head=head->parent->next;}
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<C>")){	
		if(!strcmp(t[i],"COM")){
			//printf("Doing <C> -> <comment>\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<comment>");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP")) || (!strcmp(t[i],"FE")) || (!strcmp(t[i],"EOP"))){
			//printf("Doing <C> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<comment>")){	
		if(!strcmp(t[i],"COM")){
			//printf("Doing <comment> -> COM STRING\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"COM");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"STRING");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<declStnt>")){	
		if(!strcmp(t[i],"INT")){
			//printf("Doing <declStnt> -> INT ID <i>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"INT");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,id);
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<i>");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"FLOAT")){
			//printf("Doing <declStnt> -> FLOAT ID <f>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"FLOAT");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,id);
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<f>");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"OBJECT")){
			//printf("Doing <declStnt> -> OBJECT ID <o>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"OBJECT");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,id);
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<o>");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"VECTOR")){
			//printf("Doing <declStnt> -> VECTOR ID <i>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"VECTOR");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,id);
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<i>");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"LINEAR")){
			//printf("Doing <declStnt> -> LINEAR ID <i>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LINEAR");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,id);
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<i>");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}
	
	else if(!strcmp(head->a,"<furID>")){	
		if(!strcmp(t[i],"SMC")){
			//printf("Doing <furID> -> SMC ID <furID>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"SMC");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,id);
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<furID>");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP")) || (!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <furID> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<objVecVal>")){	
		if(!strcmp(t[i],id)){
			//printf("Doing <objVecVal> -> <err>\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<err>");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"LC")){
			//printf("Doing <objVecVal> -> LC <vecAss> <objOrVec>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LC");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<vecAss>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<objOrVec>");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
			continue;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<objOrVec>")){	
		if(!strcmp(t[i],"SMC")){
			//printf("Doing <objOrVec> -> SMC <vecAss> SMC <vecAss> SMC <intOrFloatVal> RC\n");
			struct node *newList1, *newList2, *newList3, *newList4, *newList5, *newList6, *newList7;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"SMC");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<vecAss>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"SMC");
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"<vecAss>");
			newList4->parent=head;
			newList4->down=NULL;
			newList5 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList5->a,"SMC");
			newList5->parent=head;
			newList5->down=NULL;
			newList6 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList6->a,"<intOrFloatVal>");
			newList6->parent=head;
			newList6->down=NULL;
			newList7 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList7->a,"RC");
			newList7->parent=head;
			newList7->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=newList5;
			newList5->next=newList6;
			newList6->next=newList7;
			newList7->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"RC")){
			//printf("Doing <objOrVec> -> RC <toLinVal>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"RC");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<toLinVal>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<vecAss>")){	
		if( (!strcmp(t[i],id)) || (!strcmp(t[i],"LP")) || (!strcmp(t[i],in)) || (!strcmp(t[i],fl)) || (!strcmp(t[i],"TIME"))){
			//printf("Doing <vecAss> -> <intOrFloatVal> COMMA <intOrFloatVal>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<intOrFloatVal>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"COMMA");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<intOrFloatVal>");
			newList3->parent=head;
			newList3->down=NULL;
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<toLinVal>")){	
		if(!strcmp(t[i],"COM")){
			//printf("Doing <toLinVal> -> LIN <objVecVal> <furLin> <Z>\n");
			struct node *newList1, *newList2, *newList3, *newList4;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LIN");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<objVecVal>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<furLin>");
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"<Z>");
			newList4->parent=head;
			newList4->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP"))){
			//printf("Doing <toLinVal> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<Z>")){	
		if(!strcmp(t[i],"ADD")){
			//printf("Doing <Z> -> ADD <linVal>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"ADD");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<linVal>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP"))){
			//printf("Doing <Z> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<furLin>")){	
		if(!strcmp(t[i],"FRICT")){
			//printf("Doing <furLin> -> FRICT LP <intOrFloatVal> RP \n");
			struct node *newList1, *newList2, *newList3, *newList4;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"FRICT");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"LP");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<intOrFloatVal>");
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"RP");
			newList4->parent=head;
			newList4->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP"))){
			//printf("Doing <furLin> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<intOrFloatVal>")){	
		if( (!strcmp(t[i],id)) || (!strcmp(t[i],"LP")) || (!strcmp(t[i],in)) || (!strcmp(t[i],fl)) || (!strcmp(t[i],"TIME")) ){
			//printf("Doing <intOrFloatVal> -> <intOrFloatExpr> \n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<intOrFloatExpr>");
			newList1->parent=head;
			newList1->down=NULL;

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<o>")){	
		if((!strcmp(t[i],"LS")) || (!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <o> -> <o2> <objAssign>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<o2>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<objAssign>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"SMC")){
			//printf("Doing <o> -> <furID>\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<furID>");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP"))){
			//printf("Doing <o> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<o2>")){	
		if(!strcmp(t[i],"LS")){
			//printf("Doing <o2> -> LS <intOrFloatVal> RS \n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LS");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatVal>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RS");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <o2> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<objAssign>")){	
		if(!strcmp(t[i],"ASSIGN")){
			//printf("Doing <objAssign> -> ASSIGN <objVecVal>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"ASSIGN");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<objVecVal>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<v>")){	
		if((!strcmp(t[i],"LS")) || (!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <v> -> <v2> <vectorAssign>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<v>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<vectorAssign>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"SMC")){
			//printf("Doing <v> -> <furID>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<furID>");
			newList1->parent=head;
			newList1->down=NULL;

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP"))){
			//printf("Doing <v> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<v2>")){	
		if(!strcmp(t[i],"LS")){
			//printf("Doing <v2> -> LS <intOrFloatVal> RS\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LS");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatVal>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RS");
			newList3->parent=head;
			newList3->down=NULL;			
			newList1->next=newList2;		
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <v2> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<vectorAssign>")){	
		if(!strcmp(t[i],"ASSIGN")){
			//printf("Doing <vectorAssign> -> ASSIGN <objVecVal>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"ASSIGN");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<objVecVal>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<l>")){	
		if((!strcmp(t[i],"LS")) || (!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <l> -> <l2> <linAssign>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<l2>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<linAssign>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"SMC")){
			//printf("Doing <l> -> <furID>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<furID>");
			newList1->parent=head;
			newList1->down=NULL;

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP"))){
			//printf("Doing <l> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<l2>")){	
		if(!strcmp(t[i],"LS")){
			//printf("Doing <l2> -> LS <intOrFloatVal> RS\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LS");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatVal>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RS");
			newList3->parent=head;
			newList3->down=NULL;			
			newList1->next=newList2;		
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <l2> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<linAssign>")){	
		if(!strcmp(t[i],"ASSIGN")){
			//printf("Doing <linAssign> -> ASSIGN <linVal>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"ASSIGN");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<linVal>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<f>")){	
		if((!strcmp(t[i],"LS")) || (!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <f> -> <f2> <floatAssign>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<f2>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<floatAssign>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"SMC")){
			//printf("Doing <f> -> <furID>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<furID>");
			newList1->parent=head;
			newList1->down=NULL;

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP"))){
			//printf("Doing <f> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<f2>")){	
		if(!strcmp(t[i],"LS")){
			//printf("Doing <f2> -> LS <intOrFloatVal> RS\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LS");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatVal>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RS");
			newList3->parent=head;
			newList3->down=NULL;			
			newList1->next=newList2;		
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <f2> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<floatAssign>")){	
		if(!strcmp(t[i],"ASSIGN")){
			//printf("Doing <floatAssign> -> ASSIGN <intOrFloatVal>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"ASSIGN");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatVal>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<i>")){	
		if( (!strcmp(t[i],"LS")) || (!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <i> -> <i2> <intAssign>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<i2>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intAssign>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"SMC")){
			//printf("Doing <i> -> <furID>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<furID>");
			newList1->parent=head;
			newList1->down=NULL;

			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP"))){
			//printf("Doing <i> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<i2>")){	
		if(!strcmp(t[i],"LS")){
			//printf("Doing <i2> -> LS <intOrFloatVal> RS\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LS");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatVal>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RS");
			newList3->parent=head;
			newList3->down=NULL;			
			newList1->next=newList2;		
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"ASSIGN")){
			//printf("Doing <i2> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
					
					head=head->parent;
			}
				if(head->next!=NULL) head=head->next;
				if(!strcmp(head->a,"<S>")) backToRoot(i);
			}
		}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<intAssign>")){	
		if(!strcmp(t[i],"ASSIGN")){
			//printf("Doing <intAssign> -> ASSIGN <intOrFloatVal>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"ASSIGN");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatVal>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<assignPlusFunct>")){	
		if(!strcmp(t[i],id)){
			//printf("Doing <assignPlusFunct> -> <ID> <rest>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<ID>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<rest>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<ID>")){	
		if(!strcmp(t[i],id)){
			//printf("Doing <ID> -> ID <ARR>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,id);
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<ARR>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<ARR>")){	
		if(!strcmp(t[i],"LS")){
			//printf("Doing <ARR> -> LS <intOrFloatVal> RS\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LS");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatVal>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RS");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"LP")) || (!strcmp(t[i],"ASSIGN"))){
			//printf("Doing <ARR> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<rest>")){	
		if(!strcmp(t[i],"ASSIGN")){
			//printf("Doing <rest> -> ASSIGN <assign> \n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"ASSIGN");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<assign>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"LP")){
			//printf("Doing <rest> -> LP <paramListN> RP\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LP");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<paramListN>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RP");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<val>")){	
		if( (!strcmp(t[i],id)) || (!strcmp(t[i],in)) || (!strcmp(t[i],fl)) || (!strcmp(t[i],"TIME"))){
			//printf("Doing <val> -> <intOrFloatVal> <Z>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<intOrFloatVal>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<Z>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if( (!strcmp(t[i],"LC"))){
			//printf("Doing <val> -> <objVecVal>\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<objVecVal>");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<assign>")){	
		if((!strcmp(t[i],id)) || (!strcmp(t[i],in)) || (!strcmp(t[i],fl)) || (!strcmp(t[i],"TIME"))){
			//printf("Doing <assign> -> <intOrFloatVal> <Z> \n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<intOrFloatVal>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<Z>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"LC")){
			//printf("Doing <assign> -> <objVecVal>\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<objVecVal>");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<err>")){	
		if(!strcmp(t[i],id)){
			//printf("Doing <err> -> ID <err2>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,id);
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<err2>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<err2>")){	
		if(!strcmp(t[i],"LP")){
			//printf("Doing <err2> -> <funcCall>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<funcCall>");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"SEP")) || (!strcmp(t[i],"RP")) || (!strcmp(t[i],"EC")) || (!strcmp(t[i],"LT")) || (!strcmp(t[i],"GT")) || (!strcmp(t[i],"LTE")) || (!strcmp(t[i],"GTE")) || (!strcmp(t[i],"ADD")) || (!strcmp(t[i],"SUB")) || (!strcmp(t[i],"MUL")) || (!strcmp(t[i],"DIV"))){
			//printf("Doing <err2> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<condStnt>")){	
		if(!strcmp(t[i],"FS")){
			//printf("Doing <condStnt> -> FS <functionBody> FE WHEN LP <condCheck> RP \n");
			struct node *newList1, *newList2, *newList3, *newList4, *newList5, *newList6, *newList7;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"FS");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<functionBody>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"FE");
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"WHEN");
			newList4->parent=head;
			newList4->down=NULL;
			newList5 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList5->a,"LP");
			newList5->parent=head;
			newList5->down=NULL;
			newList6 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList6->a,"<condCheck>");
			newList6->parent=head;
			newList6->down=NULL;
			newList7 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList7->a,"RP");
			newList7->parent=head;
			newList7->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=newList5;
			newList5->next=newList6;
			newList6->next=newList7;
			newList7->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<condCheck>")){	
		if((!strcmp(t[i],"LP")) || (!strcmp(t[i],"NOT"))){
			//printf("Doing <condCheck> -> <not> <woCheck>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<not>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<woCheck>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<woCheck>")){	
		if(!strcmp(t[i],"LP")){
			//printf("Doing <woCheck> -> LP <furCond> RP <condCheck2>\n");
			struct node *newList1, *newList2, *newList3, *newList4;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LP");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<furCond>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RP");
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"<condCheck2>");
			newList4->parent=head;
			newList4->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<furCond>")){	
		if((!strcmp(t[i],id)) || (!strcmp(t[i],in)) || (!strcmp(t[i],fl)) || (!strcmp(t[i],"TIME")) || (!strcmp(t[i],"LP"))){
			//printf("Doing <furCond> -> <intOrFloatVal> <condOp> <intOrFloatVal>\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<intOrFloatVal>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<condOp>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<intOrFloatVal>");
			newList3->parent=head;
			newList3->down=NULL;
			/*newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"<condCheck2>");
			newList4->parent=head;
			newList4->down=NULL;*/
			
			newList1->next=newList2;
			newList2->next=newList3;
			//newList3->next=newList4;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<condCheck2>")){	
		if((!strcmp(t[i],"AND")) || (!strcmp(t[i],"OR"))){
			//printf("Doing <condCheck2> -> <condcon> <condCheck> \n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<condcon>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<condCheck>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"RP"))){
			//printf("Doing <condCheck2> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<not>")){	
		if(!strcmp(t[i],"NOT")){
			//printf("Doing <not> -> NOT\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"NOT");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"LP"))){
			//printf("Doing <not> -> E\n");
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<condOp>")){	
		if(!strcmp(t[i],"EC")){
			//printf("Doing <condOp> -> EC\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"EC");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"LT")){
			//printf("Doing <condOp> -> LT\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LT");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"GT")){
			//printf("Doing <condOp> -> GT\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"GT");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"LTE")){
			//printf("Doing <condOp> -> LTE\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LTE");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"GTE")){
			//printf("Doing <condOp> -> GTE\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"GTE");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<condcon>")){	
		if(!strcmp(t[i],"AND")){
			//printf("Doing <condcon> -> AND\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"AND");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"OR")){
			//printf("Doing <condcon> -> OR\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"OR");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<loop>")){	
		if(!strcmp(t[i],"LOOP")){
			//printf("Doing <loop> -> LOOP LP <condCheck> RP <SEPA> FS <SEPA> <functionBody> FE <SEPA>\n");
			struct node *newList1, *newList2, *newList3, *newList4, *newList5, *newList6, *newList7, *newList8, *newList9, *newList10;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LOOP");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"LP");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<condCheck>");
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"RP");
			newList4->parent=head;
			newList4->down=NULL;
			newList5 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList5->a,"<SEPA>");
			newList5->parent=head;
			newList5->down=NULL;
			newList6 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList6->a,"FS");
			newList6->parent=head;
			newList6->down=NULL;
			newList7 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList7->a,"<SEPA>");
			newList7->parent=head;
			newList7->down=NULL;
			newList8 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList8->a,"<functionBody>");
			newList8->parent=head;
			newList8->down=NULL;
			newList9 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList9->a,"FE");
			newList9->parent=head;
			newList9->down=NULL;	
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=newList5;
			newList5->next=newList6;
			newList6->next=newList7;
			newList7->next=newList8;
			newList8->next=newList9;
			newList9->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<funcCall>")){	
		if(!strcmp(t[i],"LP")){
			//printf("Doing <funcCall> -> LP <paramlistN> RP\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LP");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<paramListN>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RP");
			newList3->parent=head;
			newList3->down=NULL;	
		
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<returnCall>")){	
		if(!strcmp(t[i],"RETURN")){
			//printf("Doing <returnCall> -> RETURN <val>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"RETURN");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<val>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<outStnt>")){	
		if(!strcmp(t[i],"OUT")){
			//printf("Doing <outStnt> -> <out>\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<out>");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"OUTA")){
			//printf("Doing <outStnt> -> <outall>\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<outall>");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<out>")){	
		if(!strcmp(t[i],"OUT")){
			//printf("Doing <out> -> OUT LP <varList> RP AT LP <condCheck> RP\n");
			struct node *newList1, *newList2, *newList3, *newList4, *newList5, *newList6, *newList7, *newList8;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"OUT");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"LP");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<varList>");
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"RP");
			newList4->parent=head;
			newList4->down=NULL;
			newList5 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList5->a,"AT");
			newList5->parent=head;
			newList5->down=NULL;
			newList6 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList6->a,"LP");
			newList6->parent=head;
			newList6->down=NULL;
			newList7 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList7->a,"<condCheck>");
			newList7->parent=head;
			newList7->down=NULL;
			newList8 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList8->a,"RP");
			newList8->parent=head;
			newList8->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=newList5;
			newList5->next=newList6;
			newList6->next=newList7;
			newList7->next=newList8;
			newList8->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<outall>")){	
		if(!strcmp(t[i],"OUTA")){
			//printf("Doing <outall> -> OUTA LP <objectList> RP AT LP <condCheck> RP\n");
			struct node *newList1, *newList2, *newList3, *newList4, *newList5, *newList6, *newList7, *newList8;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"OUTA");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"LP");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"<objectList>");
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"RP");
			newList4->parent=head;
			newList4->down=NULL;
			newList5 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList5->a,"AT");
			newList5->parent=head;
			newList5->down=NULL;
			newList6 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList6->a,"LP");
			newList6->parent=head;
			newList6->down=NULL;
			newList7 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList7->a,"<condCheck>");
			newList7->parent=head;
			newList7->down=NULL;
			newList8 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList8->a,"RP");
			newList8->parent=head;
			newList8->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=newList5;
			newList5->next=newList6;
			newList6->next=newList7;
			newList7->next=newList8;
			newList8->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<varList>")){	
		if(!strcmp(t[i],id)){
			//printf("Doing <varList> -> ID\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,id);
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"TIME")){
			//printf("Doing <varList> -> TIME\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"TIME");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<objectList>")){	
		if(!strcmp(t[i],id)){
			//printf("Doing <objectList> -> <err>\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<err>");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"TIME")){
			//printf("Doing <objectList> -> TIME\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"TIME");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{/*printf("\nNO MATCH head->a=%s t[i]=%s\n",head->a,t[i]);*/printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<inStnt>")){	
		if(!strcmp(t[i],"INF")){
			//printf("Doing <inStnt> -> <inFloat> LP ID RP\n");
			struct node *newList1, *newList2, *newList3, *newList4;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<inFloat>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"LP");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,id);
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"RP");
			newList4->parent=head;
			newList4->down=NULL;

			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"ININ")){
			//printf("Doing <inStnt> -> <inInt> LP ID RP\n");
			struct node *newList1, *newList2, *newList3, *newList4;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<inInt>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"LP");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,id);
			newList3->parent=head;
			newList3->down=NULL;
			newList4 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList4->a,"RP");
			newList4->parent=head;
			newList4->down=NULL;

			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=newList4;
			newList4->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<inFloat>")){	
		if(!strcmp(t[i],"INF")){
			//printf("Doing <inFloat> -> INF\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"INF");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<inInt>")){	
		if(!strcmp(t[i],"ININ")){
			//printf("Doing <inInt> -> ININ\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"ININ");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}


	else if(!strcmp(head->a,"<addOP>")){	
		if(!strcmp(t[i],"ADD")){
			//printf("Doing <addOP> -> ADD\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"ADD");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"SUB")){
			//printf("Doing <addOP> -> SUB\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"SUB");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<mulOP>")){	
		if(!strcmp(t[i],"MUL")){
			//printf("Doing <mulOP> -> MUL\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"MUL");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"DIV")){
			//printf("Doing <mulOP> -> DIV\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"DIV");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<intOrFloatExpr>")){	
		if( (!strcmp(t[i],id)) || (!strcmp(t[i],"LP")) || (!strcmp(t[i],"TIME")) || (!strcmp(t[i],in)) || (!strcmp(t[i],fl))){
			//printf("Doing <intOrFloatExpr> -> <intOrFloatMulExpr> <intOrFloatM>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<intOrFloatMulExpr>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatM>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<intOrFloatM>")){	
		if( (!strcmp(t[i],"ADD")) || (!strcmp(t[i],"SUB"))){
			//printf("Doing <intOrFloatM> -> <addOP> <intOrFloatExpr>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<addOP>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatExpr>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if((!strcmp(t[i],"COMMA")) || (!strcmp(t[i],"SEP")) || (!strcmp(t[i],"SMC")) || (!strcmp(t[i],"RC")) || (!strcmp(t[i],"EC")) || (!strcmp(t[i],"LT")) || (!strcmp(t[i],"GT")) || (!strcmp(t[i],"LTE")) || (!strcmp(t[i],"GTE")) || (!strcmp(t[i],"RP")) || (!strcmp(t[i],"RS")) || (!strcmp(t[i],"FE"))){
			//printf("Doing <intOrFloatM> -> E\n");
			if(head->next==NULL){
				//printf("next NULL\n");
				//printf("head->parent=%s\n",head->parent->a);
				if(head->parent->next!=NULL){
					//printf("parent->next not NULL\n");
					head=(head->parent)->next;
					//printf("head=%s\n",head->a);
				}
				else{
					//printf("parent->next NULL\n");
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
					if(head->next!=NULL) head=head->next;
					//printf("head=%s head->next=%s parent now=%s\n",head->a,head->next->a,head->parent->a);
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				//printf("next not NULL\n");
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<intOrFloatMulExpr>")){	
		if((!strcmp(t[i],id)) || (!strcmp(t[i],"LP")) || (!strcmp(t[i],"TIME")) || (!strcmp(t[i],in)) || (!strcmp(t[i],fl))){
			//printf("Doing <intOrFloatMulExpr> -> <intOrFloatFactor> <intOrFloatF>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<intOrFloatFactor>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatF>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else if(!strcmp(head->a,"<intOrFloatF>")){	
		if( (!strcmp(t[i],"MUL")) || (!strcmp(t[i],"DIV")) ){
			//printf("Doing <intOrFloatF> -> <mulOP> <intOrFloatMulExpr>\n");
			struct node *newList1, *newList2;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<mulOP>");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatMulExpr>");
			newList2->parent=head;
			newList2->down=NULL;
			
			newList1->next=newList2;
			newList2->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if( (!strcmp(t[i],"ADD")) || (!strcmp(t[i],"SUB")) || (!strcmp(t[i],"COMMA")) || (!strcmp(t[i],"SEP")) || (!strcmp(t[i],"SMC")) || (!strcmp(t[i],"RC")) || (!strcmp(t[i],"EC")) || (!strcmp(t[i],"LT")) || (!strcmp(t[i],"GT")) || (!strcmp(t[i],"LTE")) || (!strcmp(t[i],"GTE")) || (!strcmp(t[i],"RP")) || (!strcmp(t[i],"RS")) || (!strcmp(t[i],"FE"))){
			//printf("Doing <intOrFloatF> -> E\n");			
			if(head->next==NULL){
				if(head->parent->next!=NULL)
					head=head->parent->next;
				else{
					while(head->next==NULL && strcmp(head->a,"<S>")){
						head=head->parent;
					}
				if(head->next!=NULL) head=head->next;
					if(!strcmp(head->a,"<S>")) backToRoot(i);
				}
			}
			else {
				head=head->next;
			}
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}

	}

	else if(!strcmp(head->a,"<intOrFloatFactor>")){	
		if(!strcmp(t[i],id)){
			//printf("Doing <intOrFloatFactor> -> <err>\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"<err>");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"LP")){
			//printf("Doing <intOrFloatFactor> -> LP <intOrFloatExpr> RP\n");
			struct node *newList1, *newList2, *newList3;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"LP");
			newList1->parent=head;
			newList1->down=NULL;
			newList2 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList2->a,"<intOrFloatExpr>");
			newList2->parent=head;
			newList2->down=NULL;
			newList3 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList3->a,"RP");
			newList3->parent=head;
			newList3->down=NULL;
			
			newList1->next=newList2;
			newList2->next=newList3;
			newList3->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],in)){
			//printf("Doing <intOrFloatFactor> -> INTCONST\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,in);
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],fl)){
			//printf("Doing <intOrFloatFactor> -> FLOATCONST\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,fl);
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else if(!strcmp(t[i],"TIME")){
			//printf("Doing <intOrFloatFactor> -> TIME\n");
			struct node *newList1;
			newList1 = (struct node*)malloc(sizeof(struct node));
			strcpy(newList1->a,"TIME");
			newList1->parent=head;
			newList1->down=NULL;
			
			newList1->next=NULL;
			head->down = newList1;
			head = newList1;
		}
		else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
	}

	else{printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();break;}
}

else if((head->a)[0]!='<'){
	//printf("HERE at terminal with t[i]=%s\n",t[i]);
	if(!strcmp(head->a,t[i])){
		//printf("head->a=%s matches with t[i]=%s\n",head->a,t[i]);
		if(!strcmp(head->a,id)) {
			//printf("Incrementing id\n");
			*id = *incid(id);
		}
		else if(!strcmp(head->a,in)) {
			//printf("Incrementing in\n");
			*in = *incin(in);
		}
		else if(!strcmp(head->a,fl)) {
			//printf("Incrementing fl\n");
			*fl = *incfl(fl);
		}
		i++;
		/*if(i==no_of_tokens) break;
		else*/ if(head->next==NULL){
			if(head->parent->next!=NULL)
				head=head->parent->next;
			else{
				while(head->next==NULL && strcmp(head->a,"<S>")){
					
					head=head->parent;
				}
				if(head->next!=NULL) head=head->next;
				if(!strcmp(head->a,"<S>")) backToRoot(i);
			}
		}
		else {
			head=head->next;
		}
		continue;
	}
	else {/*printf("head->a=%s does not match with t[i]=%s\n",head->a,t[i]);*/printf("**head=%s t[%d]=%s id=%s in=%s fl=%s",head->a,i,t[i],id,in,fl); error();i++;break;};
}

}
//printf("i=%d stack_remaining=%s\n",i,head->a);
if(i==no_of_tokens && !strcmp(head->a,"<program>")) //printf("Parsing Done correctly\n");

//printf("\n\n\n\n");
printf("\n\n\t\t********PARSE TREE********\n\n");
traverse(start,0);
ast(start,0);
//printf("\n\n*TRAVERSING after initial ast*\n\n");
//traverse(start,0);

astRemoveSep(start,0);
ast(start,0);
//printf("\n\n*TRAVERSING after remove SEP*\n\n");
//traverse(start,0);

//printf("\n--------------%s\n",start->a);
astRemoveSep(start,0);
astFurCond(start,0);
astObjOrVec(start,0);

change=1;
while(change==1){
change=0;
ast(start,0);
astRemoveSep(start,0);
astFurCond(start,0);
astObjOrVec(start,0);
}

struct parseNode *parseTree = convertParse(start);
//printf("\n\n********new One***********\n\n");
//traverse1(parseTree,0);
s1.top = -1;
s.top = -1;

struct parseNode *x;
x = ReduceExpr(parseTree);
printf("\n\n\t********ABSTRACT SYNTAX TREE***********\n\n");
traverse1(x,0);
/*change=1;
while(change) {change=0; astRemoveSep(start,0);}*/
return 0;
}
