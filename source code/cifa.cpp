#include <iostream>
#include "tokentype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



using namespace std;
extern tokentype gettoken(FILE *);
extern char tokenstring[300];
tokentype token; // 记录下一个要匹配的 token
extern int lineno;



struct node
{

    node_kind kind;

    union
    {

        char name[300]; //如果是赋值表达式 变量名

        tokentype op; // 如果是算术表达式，表示 符号

    } attribute;



    node * child [4]; // 最多有4个孩子

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

        exit(1); // 异常退出

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


// 定义

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

    可扩展

    */


    default:

        fprintf(stderr,"error happened %d low\n",lineno);

        exit(1); // 异常退出

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

        match(ID); 记录下来到符号表？

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









// if_statement 建树很麻烦

// if (logic_exp){ statement_sequence}elif{ statement_sequence}else{statement_sequence}

// 如果对应的孩子节点为 NULL，那么对应的的地方就没有表达式。





node * if_statement()
{


    match(token);

    node * t= new node(IF_KIND);


    memset(t->child,0,sizeof(t->child));


    match(LPAREN);

    t->child[0]=logic_exp();

    match(RPAREN);

    match(left_da_kuo);

// 出问题

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

// 记录变量名

    strcpy(t->attribute.name,tokenstring);

    match(token);

    match(token);

    t->child[0] = arithmetic_statement();

    return t;

}

// 因为我定义没有建树，所以如果前面有个定义，那么这样这棵树就断了。不行了，看来定义卡必须建树。


node* statement_sequence()
{
    node * t=statement();
    node *p,*q;
    p=t;

while( token!=ENDFILE&& token !=right_da_kuo )
    {
        if(token==SEMI)match(SEMI);

        if(token!=ENDFILE&&token!=right_da_kuo){  // 如果 有 ； 的话，match 之后可能是右大括号或者是 endifle 所以还要判断
            q = statement();

            p->sibling=q;

            p=q;
        }

    }
    p->sibling=NULL;
    return t;
}

// 单元测试,把树画出来




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




// 单元测试

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



