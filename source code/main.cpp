/*
 *
 *
 *词法分析器，包括注释
 * 定义，声明，赋值，算术
 *
 *
 */




#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include "tokentype.h"



using namespace std;
const int buffsize= 300;
const int max_token_length=300;

typedef enum                                   //对应于/* */  对应//
{

    START,END,INNUM,INID,  mayINCOMMENT_OR_OVER_CHUDENG,INCOMMENT1, INCOMMENT2,INASSIGN,

    mayINCHENGDENG,mayINJIADENG,MAYINJIANDENG,OUTCOMMENT1,mayINQIE,mayINHUO,

} state;







int currentPos=0;
int bufflength=0; // 当前行字符长度
char buff[buffsize];
bool EOF_FLAG=false;
const int MAXRESERVED=8; // 这个数字根据实际数目调整，负责会越界引起奔溃
char tokenstring[max_token_length];
int lineno=0;







static struct
{
    char* str;
    tokentype tok;

} reservedWords[MAXRESERVED]

=
{

    {"if",IF},{"else",ELSE},{"elif",ELIF},

    {"int",INT},{"double",DOUBLE},{"char",CHAR},{"bool",BOOL},{"while",WHILE}



};


// 返回源文件中的下一个字符

char get_next_char(FILE *f )
{

    if(currentPos<bufflength)return buff[currentPos++];

    else
    {

         lineno++;

        // read new line  fgets();

        if (fgets(buff,buffsize,f))   // 如果 fgets 返回 NULL 则文件出错
        {

            bufflength=strlen(buff);

            currentPos=0;

            return buff[currentPos++];

        }

        else   // return eof
        {

            EOF_FLAG=true;

            return  EOF;

        }

    }

}


// 倒退一个字符

void daotui()
{

    if(!EOF_FLAG)currentPos--;

}

static tokentype lookup(char * s)
{

    for (int i=0; i<MAXRESERVED; i++)
    {

        if (!strcmp(s,reservedWords[i].str))

            return reservedWords[i].tok;
    }
    return ID;

}




tokentype gettoken(FILE *f )
{


    tokentype currenttoken;

    bool save; // 标识变量，判断当前字符要不要存储

    state currentState=START;

    int c;


    int tokenindex=0;


    while(currentState!=END)
    {

        save=true;

        c=get_next_char(f);


        switch(currentState)
        {

        case START:

            if(c=='/')
            {

                currentState=mayINCOMMENT_OR_OVER_CHUDENG;

            }else if(c=='&'){

             currentState=mayINQIE;

            }else if(c=='|'){
            currentState=mayINHUO;
            }
            else if(c=='*')
            {

                currentState=mayINCHENGDENG;

            }
            else if(c=='+')
            {

                currentState=mayINJIADENG;

            }
            else if(c=='-')
            {

                currentState=MAYINJIANDENG;

            }

            else if(isalpha(c)||c=='_')
            {


                currentState=INID;


            }

            else if(isdigit(c))
            {

                currentState=INNUM;

            }

            else if(c=='\n'||c==' '||c=='\t')
            {

                save=false;
            }


            else if(c=='=')
            {

                currentState=INASSIGN;

            }

            else
            {

                currentState=END;

                switch(c)
                {

                case EOF:

                    save=false;

                    currenttoken=ENDFILE;

                    break;

                case '{':



                    currenttoken=left_da_kuo;

                    break;

                case '}':

                    currenttoken=right_da_kuo;
                    break;


                case ';':

                    currenttoken=SEMI;

                    break;

                case ',':
                    currenttoken=douhao;
                    break;



                case '(':

                    currenttoken=LPAREN;

                    break;


                case ')':

                    currenttoken=RPAREN;

                    break;

                case '<':
                    currenttoken=LT;
                    break;

                case '>':

                    currenttoken=MT;

                    break;


                default:


                    currenttoken = ERROR;



                }

            }

            break;



                case mayINQIE:

                    if(c=='&'){

                        currenttoken=qie;
                        currentState=END;
                    }
                    break;


                case mayINHUO:

                     if(c=='|'){

                        currenttoken=huo;

                        currentState=END;

                     }

                     break;


// 识别 *=
        case mayINCHENGDENG:

            if(c=='=')
            {

                currentState=END;

                currenttoken=chengdeng;

            }
            else
            {
                daotui();
                save=false;

                currentState=END;

                currenttoken=TIMES;

            }
            break;


        // 识别 +=;

        case mayINJIADENG:
            if(c=='=')
            {


                currentState=END;
                currenttoken=jiadeng;


            }
            else
            {

                daotui();
                save=false;
                currentState=END;

                currenttoken=PLUS;

            }
            break;

        //识别 -=;

        case MAYINJIANDENG:

            if(c=='=')
            {

                currentState=END;
                currenttoken=jiandeng;

            }
            else
            {
                daotui();
                save=false;

                currentState=END;

                currenttoken=MINUS;

            }
            break;



        // 如果已经有一个 等号，再来一个等号就是比较相等EQ，否则就是赋值



        case INASSIGN:

            currentState=END;

            if(c=='=')
            {


                currenttoken=EQ;


            }
            else
            {

                daotui();

                save=false;

                currenttoken=ASSIGN;


            }

            break;



        // 如果前面一个 是 '/',那么很有可能

        case  mayINCOMMENT_OR_OVER_CHUDENG:


            if(c=='*')
            {

                save=false;

                tokenindex--;

                currentState=INCOMMENT1;


            }
            else if(c=='/')
            {


                save=false;

                tokenindex--;

                currentState=INCOMMENT2;


            }
            else if(c=='=')
            {


                currentState=END;

                currenttoken=chudeng;

            }

            break;

        case INCOMMENT1:

            save=false;

            //如果是 other ，那么一直处于 注释状态中

            if(c=='*')
            {

                currentState=OUTCOMMENT1;

            }

            else if(c==EOF)
            {

                currentState=END;

                currenttoken=ENDFILE;

            }

            break;


        case INCOMMENT2:

            save=false;

            if(c==EOF)
            {
                currentState=END;

                currenttoken=ENDFILE;

            }

            else if(c=='\n')
            {

                currentState=START;

            }

            break;





        case OUTCOMMENT1:

            save=false;

            if(c=='/')
            {


                currentState=START;


            }
            else if(c==EOF)
            {

                currentState=END;

                currenttoken=ENDFILE;

            }
            else
            {


                currentState=INCOMMENT1;


            }
            break;


        case INNUM:

            if(!isdigit(c))
            {


                daotui();

                save=false;

                currentState=END;

                currenttoken=NUM;


            }

            break;


        case INID:

            if(isalpha(c)||isdigit(c)||c=='_')
            {


                currentState=INID;


            }
            else
            {

                daotui();

                save=false;

                currentState=END;

                currenttoken=ID;

            }


            break;


        // 这部分是永远不可能走到的；


        default:
            break;

        }


        if(save)
        {

            tokenstring[tokenindex++]=(char)c;

        }

        if(currentState==END)
        {

            tokenstring[tokenindex]='\0';



            if(currenttoken==ID)
            {


                currenttoken=lookup(tokenstring);


            }

        }

    }

//cout<<tokenstring<<" "<<currenttoken<<endl;


    return currenttoken;

}















/*
int main(){


   f=fopen("test.txt","r"); // 这个今天和 fgets 可以写博客


   gettoken();

   gettoken();


   return 0;




   }




*/
