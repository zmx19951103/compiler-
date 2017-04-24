#include <stdio.h>
#include <string.h>
#include "tokentype.h"
#include "symbol_table.h"
#include <iostream>



using namespace std;

#define SHIFT 4

extern TABLE * able_array[]; // able_array 在其他地方定义


int current_yu=0;




int hash(char key[])
{

    int temp = 0;
    int i = 0;

    while (key[i] !='\0')
    {
        temp = ((temp << SHIFT) + key[i]) % TABLE_SIZE;
        ++i;
    }

    return temp;

}


VAR * insert_var(TABLE * s,char name[],int lineno,tokentype type)
{
    int index=hash(name);

    VAR * i =s->var[index];

    while(i!=NULL &&( strcmp(i->name,name)!=0 ));

    if(i==NULL)
    {
        VAR * t=new VAR;

        t->lineno=lineno;

        strcpy(t->name,name);

        t->type=type;
        /*
                switch(t->type)
                {

                case INT:
                    t->value.INTvar=value.INTvar;
                    break;
                case BOOL:
                    t->BOOLvar=value.BOOLvar;
                    break;
                case CHAR:
                    t->CHARvar=value.CHARvar;
                    break;
                case DOUBLE:
                    t->DOUBLEvar=value.DOUBLEvar;
                    break;

                }
        */
        t->next=s->var[index];
        s->var[index]=t;
        return t;
    }
    else
    {
        int lineno=i->lineno;
        fprintf(stderr,"redefined variable , first defined in %d line\n",lineno);
        return NULL;
    }

}





// 如果找到返回指针，否则返回NULL;


VAR * find_var(char name[])
{
    int index = hash(name);
    int pos=current_yu;
    while(true)
    {
        VAR * i =able_array[pos]->var[index];

        while(i!=NULL&& strcmp(i->name,name)!=0);

        if(i==NULL)
        {
            pos--;
            if(pos<0)return NULL;
        }
        else return i;
    }
}
