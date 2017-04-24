#ifndef __TOKENTYPE_H
#define __TOKENTYPE_H

typedef enum
{

    ID,IF,ELSE,WHILE,ELIF,

    ENDFILE,NUM,

    ASSIGN,EQ,MT,LT,PLUS,MINUS,TIMES,OVER,LPAREN,RPAREN,SEMI,douhao,right_da_kuo,left_da_kuo, qie,huo,

    chudeng,chengdeng,jiadeng,jiandeng, // Ŀǰ��ûʵ��

    INT,DOUBLE,CHAR,BOOL,VOID,// Ŀǰֻ֧��INT


    ERROR

} tokentype;




typedef enum
{

    ASSIGN_KIND, IF_KIND, WHILE_KIND, EXP_KIND, BASIC_KIND,DEFINITION_KIND

} node_kind;




#endif // __TOKENTYPE_H
