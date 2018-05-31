#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define True 1
#define False 0
typedef struct strSplit{
    char *str;
    char *separator;
    char *parsingTarget;
    struct strSplit *(*run)(struct strSplit *);
    
}strSplit;

char *__strSplit_run(strSplit *self);

strSplit *new_strSplit(char *separator,const char *str)
{
    strSplit *self=malloc(sizeof(strSplit));
    self->str=malloc(sizeof(char)*(strlen(str)+1));
    strcpy(self->str,str);
    self->separator=separator;
    self->parsingTarget=self->str;
    self->run=__strSplit_run;
}

char *__strSplit_run(strSplit *self)
{
    char *target=NULL,*target2=NULL,*sTarget=NULL;
    
    if(!*self->parsingTarget)
        return NULL;

    for(target=self->parsingTarget;*target;target++)
    {
        if(*target==*(self->separator))
        {
            int match=True;
            for(target2=target+1,sTarget=self->separator+1;*sTarget;sTarget++,target2++)
            {
                if(*target2!=*sTarget)
                {
                    match=False;
                    break;
                }
            }
            
            if(match)
            {
                memset(target,0,sizeof(char)*(sTarget-self->separator));
                
                char *pTemp=self->parsingTarget;
                self->parsingTarget=target2;
                return pTemp;
            }
        }
    }
    
    char *pTemp=self->parsingTarget;
    self->parsingTarget=target;
    return pTemp;
}

del_strSplit(strSplit *self)
{
    free(self->str);
    free(self);
}

int main(void)
{
    strSplit *strSplitObj=new_strSplit("abc","34aabcjfdfiabcc454");
    char *r;
    int i;
    for(i=1;r=strSplitObj->run(strSplitObj);i++)
        printf("%s %d\n",r,i);
    
    del_strSplit(strSplitObj);
    return 0;
}