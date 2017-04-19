typedef enum{

ID,IF,ELSE,WHILE,ELIF,

ENDFILE,NUM,

ASSIGN ,EQ,MT,LT,PLUS,MINUS,TIMES,OVER,LPAREN,RPAREN,SEMI,douhao,right_da_kuo,left_da_kuo, qie,huo,

chudeng,chengdeng,jiadeng,jiandeng, // 目前还没实现

INT,DOUBLE,CHAR,BOOL, // 目前只支持 四种基本变量类型

ERROR

}tokentype;





typedef enum
{

    ASSIGN_KIND, IF_KIND, WHILE_KIND, EXP_KIND, BASIC_KIND

} node_kind;


