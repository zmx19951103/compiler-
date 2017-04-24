/* ��ͬ���������ò�ͬ��table ���� stack ��ʾ��ǰ�����ĸ�������ÿ������һ��  {���½�һ�� table Ȼ��� table * ѹ�� stack �У�ÿ��
 *�ұ����Ļ����� stack �� �ҵ���ǰ������Ȼ���������ұ��������ͺ��ˡ���������
 * �뿪��ǰ������Ļ�,ֻҪ�ѵ�ǰ������±�-1.
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


































