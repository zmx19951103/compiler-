#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokentype.h"
#include "symbol_table.h"



using namespace std;
extern tokentype gettoken(FILE *);
extern char tokenstring[300];
tokentype token; // ��¼��һ��Ҫƥ��� token

extern int lineno;

extern int current_yu;



TABLE* able_array[200];



struct node
{
    node_kind kind;

    tokentype type;

    VALUE value;

    union
    {

        char name[300]; //����Ǹ�ֵ���ʽ ������

        tokentype op; // ������������ʽ����ʾ ����

    } attribute;


    node * child [4]; // �����4������

    node * sibling ;


    node(node_kind kind):kind(kind)
    {

    }

    node() {}


};



node * assign_statement();

node * arithmetic_statement();

node * factor();

node * exp();

node * simple_exp();

node * statement();

node * statement_sequence();

node * if_statement();

node * while_statement();

node * definition();

void match(tokentype);

void changeValue(VAR *,VALUE ,tokentype);
int TOKEN_TO_INT(char []);
FILE *f;







void changeValue(VAR * goal,VALUE source,tokentype type){

switch(type){

case INT:
    goal->value.INTvar=source.INTvar;
    break;
case DOUBLE:
    goal->value.DOUBLEvar=source.DOUBLEvar;
    break;
case CHAR:
    goal->value.CHARvar=source.CHARvar;
    break;
case BOOL:
    goal->value.BOOLvar=source.BOOLvar;
    break;

    // һ���ǲ����ߵ���һ���ġ�

default:
    fprintf(stderr,"type is not defined!");

}
}


int TOKEN_TO_INT(char var[]){

int length=strlen(var);
int sum=0;
for(int i=0;i<length;i++){

    sum=sum*10+(var[i]-'0');
}
return sum;
}



void match(tokentype expected_token)
{

    if(token==expected_token)
    {
        token=gettoken(f);

    }

    else
    {
        fprintf(stderr,"error happened %d low\n",lineno);

        exit(1); // �쳣�˳�

    }

}


node* statement()
{

    node * t;

    switch(token)
    {

    case ID:
        t = assign_statement();
        break;
// ����

    case DOUBLE:

        t=definition();
        break;

    case INT:

        t=definition();

        break;

    case CHAR:
        t=definition();
        break;

    case BOOL:
        t=definition();
        break;

    case IF:

        t= if_statement();
        break;


    case WHILE:
        t=while_statement();
        break;


    /*

    ����չ

    */


    default:

        fprintf(stderr,"error happened %d low\n",lineno);

        exit(1); // �쳣�˳�

        break;


    }

    return t;

}







// �ڽ��﷨����ʱ�� definiton �ͽ��˸��㣬Ϊ�˲����﷨���ϵ���


node* definition()
{
    node * t=new node(DEFINITION_KIND);

    char name[300];

    tokentype temp=token;

    match(token);

    strcpy(name,tokenstring);

    match(token);

    if(insert_var(able_array[current_yu],name,lineno,temp)==NULL)
    {

        exit(1);

    }

    while(token==douhao)
    {

        match(token);

        strcpy(name,tokenstring);

        if(insert_var(able_array[current_yu],name,lineno,temp)==NULL)
        {
            exit(1);

        }

        match(ID);

    }

    return t;

}



// simple_exp  comparison_op simple_exp



node * comparison_exp()
{
    node * t = new node(EXP_KIND);
    t->child[0]=arithmetic_statement();
    t->attribute.op=token;
    match(token);

    tokentype type1,type2;

    t->child[1]=arithmetic_statement();

     type1=t->child[0]->type,type2=t->child[1]->type;

     if(type1==type2){

// ��������ظ�����̫�࣬��Ҫ����ȥ��

       t->type=BOOL;

         switch(type1){

         case INT:

        switch(t->attribute.op){

          case EQ:
             if(t->child[0]->value.INTvar==t->child[1]->value.INTvar)t->value.BOOLvar=true;

             else t->value.BOOLvar=false;
             break;

          case MT:
            if(t->child[0]->value.INTvar > t->child[1]->value.INTvar)t->value.BOOLvar=true;

             else t->value.BOOLvar=false;

            break;

          case LT:



            if(t->child[0]->value.INTvar < t->child[1]->value.INTvar)t->value.BOOLvar=true;

             else t->value.BOOLvar=false;

            break;

          default:

              fprintf(stderr,"it is not a comp token!");
              exit(1);

              break;


        }


        break;
/*

     case DOUBLE :


         switch(t->attribute.op){

      case EQ:
           if(t->child[0]->value.DOUBLEvar==t->child[1]->value.DOUBLEvar)t->value.BOOLvar=true;//DOUBLLE ��ô��ȣ�
             else t->value.BOOLvar=false;
             break;

          case MT:
            if(t->child[0]->value.DOUBLEvar > t->child[1]->value.DOUBLEvar)t->value.BOOLvar=true;

             else t->value.BOOLvar=false;

            break;


          case LT:

            if(t->child[0]->value.DOUBLEvar < t->child[1]->value.DOUBLEvar)t->value.BOOLvar=true;

             else t->value.BOOLvar=false;

            break;

          default:

              fprintf(stderr,"it is not a comp token!");
              exit(1);

              break;

         }
        break;


     case CHAR:

         switch(t->attribute.op){


     case EQ:

          if(t->child[0]->value.CHARvar==t->child[1]->value.CHARvar)t->value.BOOLvar=true;
             else t->value.BOOLvar=false;
             break;

          case MT:
            if(t->child[0]->value.CHARvar > t->child[1]->value.CHARvar)t->value.BOOLvar=true;

             else t->value.BOOLvar=false;

            break;


          case LT:

            if(t->child[0]->value.CHARvar < t->child[1]->value.CHARvar)t->value.BOOLvar=true;

             else t->value.BOOLvar=false;

            break;

          default:

              fprintf(stderr,"it is not a comp token!");
              exit(1);

              break;




         }

        break;


*/

     default:

      fprintf(stderr,"variable of this type can't compare!");

         exit(1);

    }

     }else{

     fprintf(stderr,"different type can't compare!");

     exit(1);
     }

    return t;

}






node * logic_simple_exp()
{

    node *  t = comparison_exp();

    while (token==qie)
    {
        node * p = new node(EXP_KIND);

        p->child[0] = t;

        p->attribute.op = token;

        match(token);

        p->child[1] = comparison_exp();




         if(p->child[0]->value.BOOLvar==false||p->child[1]->value.BOOLvar==false)
                p->value.BOOLvar=false;

         else  p->value.BOOLvar=true;




         p->type=BOOL;


        t = p;
    }

    return t;

}








node * logic_exp()
{
    node * t = logic_simple_exp();

    while(token==huo)
    {
        node *p = new node(EXP_KIND);
        p->child[0] = t;
        p->attribute.op = token;
        match(token);
        p->child[1] =logic_simple_exp();


           if(p->child[0]->value.BOOLvar==true||p->child[1]->value.BOOLvar==true)
                p->value.BOOLvar=true;

         else  p->value.BOOLvar=false;

         p->type=BOOL;


        t = p;
    }

  // cout<<t->value.BOOLvar<<endl;


    return t;

}



/*


 while (exp) {   statement_sequence    }



*/


node *while_statement()
{

    match(token);
    node * t=new node(WHILE_KIND);
    match(LPAREN);

    t->child[0]=logic_exp();

    match(RPAREN);

    match(left_da_kuo);

    t->child[1]=statement_sequence();

    match(right_da_kuo);

    return t;

}

node * if_statement()
{

    match(token);
    node * t= new node(IF_KIND);
    memset(t->child,0,sizeof(t->child));
    match(LPAREN);
    t->child[0]=logic_exp();
    match(RPAREN);
    match(left_da_kuo);
    current_yu++;
    able_array[current_yu]=new TABLE;

// ������

    t->child[1] = statement_sequence();

    match(right_da_kuo);

    current_yu--;//�뿪��ǰ������

    if(token==ELIF)
    {
        match(token);
        match(left_da_kuo);
        current_yu++;
        able_array[current_yu]=new TABLE;
        t->child[2]=statement_sequence();
        match(right_da_kuo);
        current_yu--;
    }

    if(token==ELSE)
    {
        match(token);
        match(left_da_kuo);
        current_yu++;
        able_array[current_yu]=new TABLE;
        t->child[3]=statement_sequence();
        match(right_da_kuo);
        current_yu--;
    }

    return t;

}


node* factor()
{
    node * t=new node;

    switch(token)
    {
    case ID:

        t->kind=BASIC_KIND;

         VAR *temp;

        if((temp=find_var(tokenstring))!=NULL){

         strcpy(t->attribute.name,tokenstring);

         t->type=temp->type;

        switch(t->type){

        case INT:

        t->value.INTvar=temp->value.INTvar;

        //cout<<t->value.INTvar<<endl;

        break;

        case DOUBLE:
            t->value.DOUBLEvar=temp->value.DOUBLEvar;
            break;

        case CHAR:
            t->value.CHARvar=temp->value.CHARvar;
             break;
        case BOOL:
            t->value.BOOLvar=temp->value.BOOLvar;
            break;

        // һ���ǲ����ߵ���һ���ġ�
        default:
            fprintf(stderr,"error in factor()1!");
            break;
        }
        match(token);

        }else{
        fprintf(stderr,"variable is not defined!");
        exit(1);
        }
        break;

    case NUM:

        t->kind=BASIC_KIND;

        t->type=INT; //Ŀǰֻ��int

        t->value.INTvar=TOKEN_TO_INT(tokenstring);// ת����int

        strcpy(t->attribute.name,tokenstring);

        match(token);

        break;

    case LPAREN:

        match(LPAREN);

        t=arithmetic_statement();

        match(RPAREN);

        break;
    default:

        fprintf(stderr, "error  in factor()2!");
        exit(1);
        break;

    }

    return t;

}


node* simple_exp()
{

    node* t = factor();
    while( token==TIMES || token == OVER )
    {
        node * p= new node(EXP_KIND);

        p->child[0]=t;

        p->attribute.op = token;

        match(token);

        p->child[1]=factor();

         p->type=INT;
        switch(p->attribute.op){
     case TIMES:
        p->value.INTvar=p->child[0]->value.INTvar*p->child[1]->value.INTvar;
        break;

     case OVER:
        p->value.INTvar=p->child[0]->value.INTvar/p->child[1]->value.INTvar;
        break;
        // һ�㲻���ߵ���һ��
     default:
        break;
        }
        t=p;

    }

    return t;

}

node* arithmetic_statement()
{

    node *t = simple_exp();


    while (token ==PLUS || token == MINUS )
    {

        node * p = new node(EXP_KIND) ;

        p->attribute.op= token;

        p->child[0]=t;

        match(token);

        p->child[1]=simple_exp();

        p->type=INT;

        switch(p->attribute.op){

    case PLUS:

         p->value.INTvar=p->child[0]->value.INTvar+p->child[1]->value.INTvar;


        break;

    case MINUS:

        p->value.INTvar=p->child[0]->value.INTvar-p->child[1]->value.INTvar;


       break;

       // һ�㲻���ߵ���һ��

    default:

        break;
        }

        t=p;

    }

    return t;

}


node * assign_statement()
{
    node * t= new node(ASSIGN_KIND);
    strcpy(t->attribute.name,tokenstring);

    // �� ��ǰ�����������Ƿ����������
     VAR * temp;
    if((temp=find_var(tokenstring))==NULL){
    fprintf(stderr,"variable %s is not defined!",tokenstring);
    exit(1);
    }
    match(token);
    match(token);

    t->child[0] = arithmetic_statement();

    // ���������ͬ����ȷ�����򱨴�
    if(t->child[0]->type==temp->type){
      changeValue(temp,t->child[0]->value,temp->type); //�ı���ű��е�ֵ
        //cout<<temp->value.INTvar<<endl;
    }else{
    fprintf(stderr,"type is not suitable!");
    exit(1);
    }
    return t;
}

node* statement_sequence()
{
    node * t=statement();
    node *p,*q;
    p=t;

    while( token!=ENDFILE&& token !=right_da_kuo )
    {
        if(token==SEMI)match(SEMI);

        if(token!=ENDFILE&&token!=right_da_kuo)   // ��� �� �� �Ļ���match ֮��������Ҵ����Ż����� endifle ���Ի�Ҫ�ж�
        {
            q = statement();
            p->sibling=q;
            p=q;
        }
    }
    p->sibling=NULL;

    return t;
}








// ��Ԫ����,����������





void travel(node * root)
{
    if(root)
    {
        switch(root->kind)
        {

        case DEFINITION_KIND:
            break;
        case EXP_KIND:
            travel(root->child[0]);
            cout<<root->attribute.op<<endl;
            travel(root->child[1]);
            break;

        case IF_KIND:

            cout<<"if("<<endl;
            travel(root->child[0]);
            cout<<")"<<endl;
            cout<<"{"<<endl;
            travel(root->child[1]);
            cout<<"}"<<endl;
            if(!(root->child[2]==0))
            {
                cout<<"elif {"<<endl;
                travel(root->child[2]);
                cout<<"}"<<endl;
            }
            if(!(root->child[3]==0))
            {
                cout<<"else {"<<endl;
                travel(root->child[3]);
                cout<<"}"<<endl;

            }



            break;

        case WHILE_KIND:

            cout<<"while"<<endl;

            cout<<"("<<endl;

            travel(root->child[0]);

            cout<<")"<<endl;

            cout<<"{"<<endl;

            travel(root->child[1]);
            cout<<"}"<<endl;

            break;

        case  BASIC_KIND:

            cout<<root->attribute.name<<endl;

            break;



        case ASSIGN_KIND:

            cout<<root->attribute.name<<endl;

            cout<<"="<<endl;

            travel(root->child[0]);

            break;


        default:

            fprintf(stderr,"error");

            exit(1);

        }
    }

}








// ��Ԫ����

int main()
{

    node * root;

    f=fopen("test.txt","r");


    token=gettoken(f);


    if(token!=ENDFILE)
    {


        able_array[current_yu]=new TABLE;

        root = statement_sequence();

        for(node * it = root ; it!=NULL ; it=it->sibling)
        {

            travel(it);

            cout<<"\n\n\n\n";

        }


        // ����












    }

    return 0;

}



