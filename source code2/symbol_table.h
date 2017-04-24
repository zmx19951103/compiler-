/* 不同的作用域用不同的table ，用 stack 表示当前处于哪个作用域，每次碰到一个  {，新建一个 table 然后把 table * 压在 stack 中，每次
 *找变量的话，从 stack 中 找到当前作用域，然后在里面找变量。不就好了。。。。。
 * 离开当前作用域的话,只要把当前数组的下标-1.
 */





#ifndef __SYMBOL_TABLE_H

#define __SYMBOL_TABLE_H

#include "tokentype.h"

#define TABLE_SIZE 211

#define var_SIZE 30


union VALUE
{
    int INTvar;
    bool BOOLvar;
    char CHARvar;
    double DOUBLEvar;
};

struct VAR
{

    tokentype type;

    VALUE value;

    char name[var_SIZE];

    int lineno;

    VAR * next;


    VAR() {}
};



struct TABLE
{

    VAR * var[TABLE_SIZE];

};


#endif // __SYMBOL_TABLE_H






VAR * insert_var(TABLE * S,char[], int,tokentype);

VAR * find_var(char name[]);

int hash(char []);


































