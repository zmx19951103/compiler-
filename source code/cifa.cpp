#include <iostream>
#include "tokentype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



using namespace std;
extern tokentype gettoken(FILE *);
extern char tokenstring[300];
tokentype token; // ��¼��һ��Ҫƥ��� token
extern int lineno;



struct node
{

    node_kind kind;

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

void definition();

void match(tokentype);





FILE *f;

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
    definition();
        break;

    case INT:
        definition();
        break;

    case CHAR:
        definition();
        break;

    case BOOL:
        definition();
        break;

    case IF:

        t= if_statement();
        break;


    case WHILE: t=while_statement();break;


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



void definition()
{

    match(token);






    match(ID);

    while(token==douhao)
    {

        match(token);


        /*

        match(ID); ��¼���������ű�

        */

        match(ID);

    }

//match(SEMI);

}







// simple_exp  comparison_op simple_exp


node * comparison_exp()
{


    node * t = new node(EXP_KIND);


    t->child[0]=arithmetic_statement();



    t->attribute.op=token;

    match(token);



    t->child[1]=arithmetic_statement();




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


        t = p;

    }

    return t;

}





/*


 while (exp) {   statement_sequence    }



*/


node *while_statement(){


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









// if_statement �������鷳

// if (logic_exp){ statement_sequence}elif{ statement_sequence}else{statement_sequence}

// �����Ӧ�ĺ��ӽڵ�Ϊ NULL����ô��Ӧ�ĵĵط���û�б��ʽ��





node * if_statement()
{


    match(token);

    node * t= new node(IF_KIND);


    memset(t->child,0,sizeof(t->child));


    match(LPAREN);

    t->child[0]=logic_exp();

    match(RPAREN);

    match(left_da_kuo);

// ������

    t->child[1]=statement_sequence();

    match(right_da_kuo);

    if(token==ELIF)
    {
        match(token);

        match(left_da_kuo);

        t->child[2]=statement_sequence();

        match(right_da_kuo);

    }


    if(token==ELSE)
    {
        match(token);
        match(left_da_kuo);
        t->child[3]=statement_sequence();
        match(right_da_kuo);
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

        strcpy(t->attribute.name,tokenstring);

        match(token);

        break;

    case NUM:



        t->kind=BASIC_KIND;

        strcpy(t->attribute.name,tokenstring);

        match(token);



        break;





    case LPAREN:

        match(LPAREN);

        t=arithmetic_statement();

        match(RPAREN);

        break;
    default:

        fprintf(stderr, "error");
      exit(1);
        break;

    }
    return t;

}


node* simple_exp()
{

    node* t = factor();

    while(token==TIMES || token == OVER )
    {

        node * p= new node(EXP_KIND);

        p->child[0]=t;

        p->attribute.op = token;

        match(token);

        p->child[1]=factor();

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

        t=p;

    }

    return t;

}





node * assign_statement()
{

    node * t= new node(ASSIGN_KIND);

// ��¼������

    strcpy(t->attribute.name,tokenstring);

    match(token);

    match(token);

    t->child[0] = arithmetic_statement();

    return t;

}

// ��Ϊ�Ҷ���û�н������������ǰ���и����壬��ô����������Ͷ��ˡ������ˣ��������忨���뽨����


node* statement_sequence()
{
    node * t=statement();
    node *p,*q;
    p=t;

while( token!=ENDFILE&& token !=right_da_kuo )
    {
        if(token==SEMI)match(SEMI);

        if(token!=ENDFILE&&token!=right_da_kuo){  // ��� �� �� �Ļ���match ֮��������Ҵ����Ż����� endifle ���Ի�Ҫ�ж�
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


        case(EXP_KIND):

            travel(root->child[0]);

            cout<<root->attribute.op<<endl;

            travel(root->child[1]);

/*
    for(node * it = root->sibling ; it!=NULL ; it=it->sibling)

        {

           travel(it);

         }

*/

            break;


        case(IF_KIND):

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

    for(node * it = root->sibling ; it!=NULL ; it=it->sibling)

        {

           travel(it);

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

            for(node * it = root->sibling ; it!=NULL ; it=it->sibling)

            {

                        travel(it);

                }
                      break;



        case(BASIC_KIND):

            cout<<root->attribute.name<<endl;

            break;



        case(ASSIGN_KIND):

            cout<<root->attribute.name<<endl;

            cout<<"="<<endl;

           travel(root->child[0]);



            for(node * it = root->sibling ; it!=NULL ; it=it->sibling)
        {


           travel(it);


         }



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


   if(token!=ENDFILE){


    root = statement_sequence();


    for(node * it = root ; it!=NULL ; it=it->sibling)
    {

        travel(it);

        cout<<"\n\n\n\n";

    }

   }

    return 0;

}



