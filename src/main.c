//
//  main.c
//  gen_conf
//
//  Created by ruochenzuo on 7/27/15.
//  Copyright (c) 2015 ruochenzuo. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define MAXLINELEN 1024


#define MAXSCOPEKEYLEN 1024
#define MAXKEYLEN 1024
#define MAXVALUELEN 1024


typedef char mykey_t[MAXKEYLEN];
typedef struct key_value_scope key_value_scope_t;

struct key_value_scope{
    mykey_t scope_key;
    mykey_t *key;
    int n;
};

mykey_t* value_scope;
key_value_scope_t* kv_scope;

int value_scope_num = 0;
int key_value_scope_num = 0 ;


char* my_toupper(char *s)
{
    if(s == NULL)return NULL;
    char *p = s;
    while(*p != '\0')
    {
        *p = toupper(*p);
        p++;
    }
    return s;
}


char * my_tolower(char *s)
{
    if(s == NULL)return NULL;
    char *p = s;
    while(*p != '\0')
    {
        *p = tolower(*p);
        p++;
    }
    return s;
}

void print_tab(int loop,FILE *fp)
{
    int i;
    for(i = 0; i < loop; ++i)
        fprintf(fp,"\t");
        
}

static int all_space(char *begin, char *end)
{
    while(begin != end)
    {
        if (*begin == ' ' || *begin == '\n' || *begin == '\r' || *begin == '\t' )
            begin++;
        else
            return 0;
    }
    return 1;
}

static int get_key(char *begin, char *end, char *key)
{
    int i = 0;
    while (begin != end && (*begin == ' '|| *begin  == '\t')) {
        begin++;
    }
    if(begin == end)
        goto error;
    for(; begin != end; begin++)
    {
        if(*begin != ' ' && *begin != '\t')
            key[i++] = *begin;
        else
            break;
    }
    if(!all_space(begin,end))
        goto error;
    key[i] = '\0';
    return 1;
error:

    return 0;
}

void gen_func_all_space(FILE *fp)
{
    //util funcs
    fprintf(fp,"%s\n%s\n", "static int all_space(char *begin, char *end)","{");
    print_tab(1, fp);
    fprintf(fp, "%s\n\t%s\n","while(begin != end)","{");
    print_tab(2,fp);
    fprintf(fp, "%s\n","if (*begin == ' ' || *begin == '\\n' || *begin == '\\r' || *begin == '\\t' )");
    print_tab(3, fp);
    fprintf(fp, "%s\n","begin++;");
    print_tab(2, fp);
    fprintf(fp, "%s\n","else");
    print_tab(3, fp);
    fprintf(fp, "%s\n","return 0;");
    print_tab(1, fp);
    fprintf(fp,"%s\n\t%s\n", "}","return 1;");
    fprintf(fp, "%s\n\n","}");
}

void gen_func_get_key(FILE *fp)
{
    int loop = 0;
    fprintf(fp, "%s\n%s\n","static int get_key(char *begin, char *end, char *key)","{");
    loop++;
    print_tab(loop, fp);
    fprintf(fp, "%s\n","int i = 0;");
    print_tab(loop, fp);
    fprintf(fp, "%s\n","while (begin != end && (*begin == ' '|| *begin  == '\\t')) {");
    loop++;
    print_tab(loop, fp);
    fprintf(fp,"%s\n\t%s\n", "begin++;","}");
    loop--;
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "if(begin == end)");
    loop++;
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "goto error;");
    loop--;
    print_tab(loop, fp);
    fprintf(fp,"%s\n\t%s\n", "for(; begin != end; begin++)","{");
    loop++;
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "if(*begin != ' ' && *begin != '\\t')");
    loop++;
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "key[i++] = *begin;");
    loop--;
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "else");
    loop++;
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "break;");
    loop--;
    loop--;
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "}");
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "if(!all_space(begin,end))");
    loop++;
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "goto error;");
    loop--;
    print_tab(loop, fp);
    fprintf(fp, "%s\n","key[i] = '\\0';");
    print_tab(loop, fp);
    fprintf(fp, "%s\n","return 1;");
    fprintf(fp,"%s\n", "error:");
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "printf(\"line : %d , %s key formate error!\\n\",linenum,key);");
    print_tab(loop, fp);
    fprintf(fp,"%s\n%s\n\n", "return 0;","}");

}


void gen_func_parse_conf(FILE *fp)
{
    int loop = 0;
    fprintf(fp,"%s\n%s\n", "int parse_conf(const char * path)","{");
    loop++;
    print_tab(loop, fp);
    fprintf(fp,"%s\n\t%s\n\t%s\n", "FILE *fp;","int ret = 0;","fp = fopen(path,\"r+\");");
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "if (fp == NULL) {");
    loop++;
    print_tab(loop, fp);
    fprintf(fp,"%s\n","perror(\"open configure file error!\");" );
    print_tab(loop, fp);
    fprintf(fp,"%s\n","return 0;" );
    loop--;
    print_tab(loop, fp);
    fprintf(fp,"%s\n","}" );
    print_tab(loop, fp);
    fprintf(fp,"%s\n","ret = parse_conf_per_scope(fp);" );
    print_tab(loop, fp);
    fprintf(fp,"%s\n","fclose(fp);" );
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "if(ret) {");
    loop++;
    int i = 0;
    for( ; i < value_scope_num; ++i)
    {
        char upkey[MAXKEYLEN];
        char lowkey[MAXKEYLEN];
        strcpy(upkey,my_toupper(value_scope[i]));
        strcpy(lowkey,my_tolower(value_scope[i]));
        
        print_tab(loop, fp);
        fprintf(fp,"%s%s,%s,%s%s%s\n", "qsort(",upkey,lowkey,"sizeof(",upkey,"[0]),compare);");
    }
    loop--;
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "}");
    print_tab(loop, fp);
    fprintf(fp,"%s\n", "return ret;");
    loop--;
    print_tab(loop, fp);
    fprintf(fp,"%s\n\n", "}");
    
};


void  gen_func_parse_conf_per_scope(FILE *fp)
{
    int loop = 0;
    fprintf(fp,"%s\n%s\n", "static int parse_conf_per_scope(FILE *stream)","{");
    loop++;
    //print_tab(loop, fp);
    fprintf(fp,"%s\n", "\tint scope = 0;\n\
    char scopekey[MAXVALUELEN]={0};\n\
    char line[MAXLINELEN]={0};\n\
    char *p = NULL;\n\
    FILE *fp = stream;\n");
    fprintf(fp,"%s\n", "\twhile(1) {\n\
        char *ret;\n\
        ret = fgets(line, MAXLINELEN, fp);\n\
        \n\
        if (ret == NULL) {\n\
            assert(feof(fp));\n\
            if(feof(fp)){\n\
                printf(\" end of file,read configure file completed !\\n\");\n\
                break;\n\
            }else {\n\
                perror(\" read file error!\");\n\
                return 0;\n\
            }\n\
        }\n\
        linenum++;\n");
    fprintf(fp,"%s\n", "\t    if((p = strstr(line,\"#\")) != NULL){\n\
            char *tvl = line;\n\
            for( ; tvl != p ; tvl++ ){\n\
                if (*tvl != ' '&& *tvl != '\\n' && *tvl != '\\t' && *tvl != '\\r') {\n\
                    goto error1;\n\
                }\n\
            }\n\
            p = NULL;\n\
            continue;\n\
            \n");
    fprintf(fp,"%s\n", "\t    } else if((p = strstr(line,\"{\"))!= NULL){\n\
            assert(scope == 0);\n\
            if (scope) {\n\
                goto error1;\n\
            }\n\
            scope = 1;\n\
            char *tvl = p+1;\n\
            for ( ; tvl != line + strlen(line); tvl++) {\n\
                if (*tvl != ' '&& *tvl != '\\n' && *tvl != '\\t' && *tvl != '\\r') {\n\
                    goto error1;\n\
                }\n\
            }\n\
            if(!get_key(line, p, scopekey))\n\
                return 0;\n\
            p = NULL;\n");
    if( value_scope_num > 0){
        print_tab(3, fp);
        fprintf(fp,"%s%s%s\n","if(strcmp(scopekey,\"",value_scope[0],"\") == 0 ){" );
        print_tab(4, fp);
        fprintf(fp,"%s\n", "continue;");
        int i;
        for(i = 1 ; i < value_scope_num; ++i)
        {
            print_tab(3, fp);
            fprintf(fp,"%s%s%s\n","}else if(strcmp(scopekey,\"",value_scope[i],"\") == 0 ){" );
            print_tab(4, fp);
            fprintf(fp,"%s\n", "continue;");
        }
        for( i = 0; i < key_value_scope_num; ++i)
        {
            print_tab(3, fp);
            fprintf(fp,"%s%s%s\n","}else if(strcmp(scopekey,\"",kv_scope[i].scope_key,"\") == 0 ){" );
            print_tab(4, fp);
            fprintf(fp,"%s\n", "continue;");
        }
    }else {
        print_tab(3, fp);
        fprintf(fp,"%s%s%s\n","if(strcmp(scopekey,\"",kv_scope[0].scope_key,"\") == 0 ){" );
        print_tab(4, fp);
        fprintf(fp,"%s\n", "continue;");
        int i;
        for( i = 1; i < key_value_scope_num; ++i)
        {
            print_tab(3, fp);
            fprintf(fp,"%s%s%s\n","}else if(strcmp(scopekey,\"",kv_scope[i].scope_key,"\") == 0 ){" );
            print_tab(4, fp);
            fprintf(fp,"%s\n", "continue;");
        }
        
    }

    fprintf(fp,"%s\n","            }else {\n\
                printf(\"line: %d, %s is unknown scope key word!\\n\",linenum,scopekey);\n\
                return 0;\n\
            }");
    fprintf(fp, "%s\n","        }else if ((p = strstr(line,\"}\")) != NULL){\n\
            assert(scope);\n\
            if (!scope)\n\
                goto error1;\n\
            if(!all_space(line, p) || !all_space(p+1, line+strlen(line)))\n\
                goto error1;\n\
            scope = 0;\n\
            p = NULL;");
    fprintf(fp,"%s\n", "        }else if((p = strstr(line,\";\")) != NULL){\n\
            if(!scope){\n\
                goto error1;\n\
            }\n\
            if(!all_space(p+1, line+strlen(line))){\n\
                goto error1;\n\
            }\n\
            char key[MAXVALUELEN];");
    
    if (value_scope_num > 0) {
        print_tab(3, fp);
        fprintf(fp,"%s%s%s\n","if(strcmp(scopekey,\"",value_scope[0],"\") == 0 ){" );
        fprintf(fp,"%s\n", "                if (!get_key(line, p, key))\n\
                    goto error1;");
        char upkey[MAXKEYLEN];
        char lowkey[MAXKEYLEN];
        strcpy(upkey,my_toupper(value_scope[0]));
        strcpy(lowkey,my_tolower(value_scope[0]));
        print_tab(4, fp);
        fprintf(fp,"%s%s%s%s%s\n","strncpy(",upkey,"[",lowkey,"++], key, strlen(key));" );
        int i;
        for(i = 1 ; i < value_scope_num; ++i)
        {
            print_tab(3, fp);
            fprintf(fp,"%s%s%s\n","}else if(strcmp(scopekey,\"",value_scope[i],"\") == 0 ){" );
            fprintf(fp,"%s\n", "                if (!get_key(line, p, key))\n\
                    goto error1;");
            strcpy(upkey,my_toupper(value_scope[i]));
            strcpy(lowkey,my_tolower(value_scope[i]));
            print_tab(4, fp);
            fprintf(fp,"%s%s%s%s%s\n","strncpy(",upkey,"[",lowkey,"++], key, strlen(key));" );
        }
        for(i = 0 ; i < key_value_scope_num; ++i)
        {
            print_tab(3, fp);
            fprintf(fp,"%s%s%s\n","}else if(strcmp(scopekey,\"",kv_scope[i].scope_key,"\") == 0 ){" );
            fprintf(fp,"%s\n", "                char *m = NULL;\n\
                if((m = strstr(line,\":\")) != NULL){\n\
                    if(!get_key(line, m, key))\n\
                        goto error1;");
            int j;
            print_tab(5, fp);
            fprintf(fp,"%s%s%s\n","if (strcmp(key, \"",kv_scope[i].key[0],"\") == 0) {" );
            print_tab(6, fp);
            fprintf(fp,"%s%s%s\n", "if(!get_key(m+1, line+strlen(line)-2,",my_toupper(kv_scope[i].key[0]),"))");
            print_tab(7, fp);
            fprintf(fp,"%s\n", "goto error1;");
            
            for(j = 1; j < kv_scope[i].n; ++j){
                print_tab(5, fp);
                fprintf(fp,"%s%s%s\n","}else if (strcmp(key, \"",kv_scope[i].key[j],"\") == 0) {" );
                print_tab(6, fp);
                fprintf(fp,"%s%s%s\n", "if(!get_key(m+1, line+strlen(line)-2,",my_toupper(kv_scope[i].key[j]),"))");
                print_tab(7, fp);
                fprintf(fp,"%s\n", "goto error1;");
            }
            
            fprintf(fp,"%s\n", "                    }else {\n\
                        printf(\"line: %d, %s is unknown key word!\\n\",linenum,key);\n\
                        return 0;\n\
                    }\n\
                }");
        }
    }else {
        print_tab(3, fp);
        fprintf(fp,"%s%s%s\n","if(strcmp(scopekey,\"",kv_scope[0].scope_key,"\") == 0 ){" );
        fprintf(fp,"%s\n", "                char *m = NULL;\n\
                if((m = strstr(line,\":\")) != NULL){\n\
                    if(!get_key(line, m, key))\n\
                        goto error1;");
        int j;
        print_tab(5, fp);
        fprintf(fp,"%s%s%s\n","if (strcmp(key, \"",kv_scope[0].key[0],"\") == 0) {" );
        print_tab(6, fp);
        fprintf(fp,"%s%s%s\n", "if(!get_key(m+1, line+strlen(line)-2,",\
                my_toupper(kv_scope[0].key[0]),"))");
        print_tab(7, fp);
        fprintf(fp,"%s\n", "goto error1;");
        
        for(j = 1; j < kv_scope[0].n; ++j){
            print_tab(5, fp);
            fprintf(fp,"%s%s%s\n","}else if (strcmp(key, \"",kv_scope[0].key[j],"\") == 0) {" );
            print_tab(6, fp);
            fprintf(fp,"%s%s%s\n", "if(!get_key(m+1, line+strlen(line)-2,",my_toupper(kv_scope[0].key[j]),"))");
            print_tab(7, fp);
            fprintf(fp,"%s\n", "goto error1;");
        }
        fprintf(fp,"%s\n", "                    }else {\n\
                        printf(\"line: %d, %s is unknown key word!\\n\",linenum,key);\n\
                        return 0;\n\
                    }\n\
                }");
        int i;
        for(i = 1; i < key_value_scope_num; ++i){
            print_tab(3, fp);
            fprintf(fp,"%s%s%s\n","}else if(strcmp(scopekey,\"",kv_scope[i].scope_key,"\") == 0 ){" );
            fprintf(fp,"%s\n", "                char *m = NULL;\n\
                if((m = strstr(line,\":\")) != NULL){\n\
                    if(!get_key(line, m, key))\n\
                    goto error1;");
            int j;
            print_tab(5, fp);
            fprintf(fp,"%s%s%s\n","if (strcmp(key, \"",kv_scope[i].key[0],"\") == 0) {" );
            print_tab(6, fp);
            fprintf(fp,"%s%s%s\n", "if(!get_key(m+1, line+strlen(line)-2,",my_toupper(kv_scope[i].key[0]),"))");
            print_tab(7, fp);
            fprintf(fp,"%s\n", "goto error1;");
            
            for(j = 1; j < kv_scope[i].n; ++j){
                print_tab(5, fp);
                fprintf(fp,"%s%s%s\n","}else if (strcmp(key, \"",kv_scope[i].key[j],"\") == 0) {" );
                print_tab(6, fp);
                fprintf(fp,"%s%s%s\n", "if(!get_key(m+1, line+strlen(line)-2,",my_toupper(kv_scope[i].key[j]),"))");
                print_tab(7, fp);
                fprintf(fp,"%s\n", "goto error1;");
            }
            
            fprintf(fp,"%s\n", "                    }else {\n\
                        printf(\"line: %d, %s is unknown key word!\\n\",linenum,key);\n\
                        return 0;\n\
                    }\n\
                }");
        }

    }
    
    print_tab(3, fp);
    fprintf(fp, "%s\n","}");
    
    
    fprintf(fp,"%s\n", "        }else {\n\
            if(!all_space(line, line + strlen(line)))\n\
                goto error1;\n\
        }\n\
    }\n\
    if(scope)\n\
        goto error1;\n\
    return 1;\n\
error1:\n\
    printf(\"line : %d ,configure formate error!\\n\",linenum);\n\
    return 0;\n\
}\n");
    
}


//gen parse_conf.h
void gen_parse_conf_head_func_file(char *path_prefix)
{
    FILE *fp;
    char path[1024];
    int i,j;
    sprintf(path, "%s%s",path_prefix,"parse_conf.h");
    fp  = fopen(path,"w+");
    
    fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n","//","//parse_conf.h","//parseconf","//","//Auto generated by gen_conf","//  Copyright (c) 2015 ruochenzuo. All rights reserved.","//");
    //header
    fprintf(fp, "%s\n%s\n\n","#ifndef __PARSE_CONF__","#define __PARSE_CONF__");
    fprintf(fp, "%s\n%s\n%s\n\n","#define MAXLINELEN 1024","#define MAXVALUELEN 128","#define MAXLISTNUM 1024");
    
    //globle value scope array
    for(i = 0 ; i < value_scope_num; ++i){
        fprintf(fp, "%s %s%s\n","char",my_toupper(value_scope[i]),"[MAXLISTNUM][MAXVALUELEN];");
    }
    
    //globle key_value var
    for(i = 0; i < key_value_scope_num; ++i)
    {
        for( j = 0 ; j < kv_scope[i].n; ++j)
        {
            fprintf(fp, "%s %s%s\n","char",my_toupper(kv_scope[i].key[j]),"[MAXVALUELEN];");
        }
        fprintf(fp,"\n");
    }
    
    //the function to call
    fprintf(fp,"%s\n\n", "int parse_conf(const char * path);");
    
    //end
    fprintf(fp, "%s\n","#endif");
    
}



//gen parse_conf.c
void gen_parse_conf_func_file(char *path_prefix)
{
    FILE *fp;
    char path[1024];
    sprintf(path, "%s%s",path_prefix,"parse_conf.c");
    fp  = fopen(path,"w+");
    int i,j;
    
    //header
    fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n","//","//parse_conf.c","//parseconf","//","//Auto generated by gen_conf","//  Copyright (c) 2015 ruochenzuo. All rights reserved.","//");
    fprintf(fp, "%s\n%s\n%s\n%s\n%s\n\n","#include <stdio.h>","#include <string.h>","#include <assert.h>","#include <stdlib.h>","#include \"parse_conf.h\" ");
    
    //globle vars
    fprintf(fp,"%s\n", "int linenum = 0;");
    for( i = 0; i < value_scope_num; ++i)
    {
        fprintf(fp, "%s %s %s\n","int",my_tolower(value_scope[i]),"= 0;");
    }
    
    //we have changed all key to upper in gen_parse_conf_head_file(), change them back to lower
    for( i = 0; i < key_value_scope_num; ++i)
        for(j =0 ; j < kv_scope[i].n; ++j)
            my_tolower(kv_scope[i].key[j]);
    
    //static int compare(const void *a, const void *b); used to sort value scope
    fprintf(fp,"\n%s\n%s\n\t%s\n%s\n\n", "static int compare(const void *a, const void *b)","{","return strcmp(a,b);","}");
    
    gen_func_all_space(fp);
    
    gen_func_get_key(fp);
    
    gen_func_parse_conf_per_scope(fp);
    
    gen_func_parse_conf(fp);
    
}

//gen example.conf
void gen_parse_conf(char *path_prefix)
{
    int i,j;
    FILE *fp;
    char path[1024];
    sprintf(path, "%s%s",path_prefix,"example.conf");
    fp  = fopen(path,"w+");
    
    fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n","##","#example.conf","##parseconf","##","##Auto generated by gen_conf","##  Copyright (c) 2015 ruochenzuo. All rights reserved.","##");
    for(i = 0; i < value_scope_num ; ++i)
    {
        fprintf(fp, "%s %s\n   #xxx\n%s\n\n",value_scope[i],"{","}");
    }
    
    for(i = 0; i < key_value_scope_num; ++i)
    {
        fprintf(fp,"%s %s\n",kv_scope[i].scope_key,"{");
        for(j = 0; j < kv_scope[i].n; ++j)
        {
            fprintf(fp,"\t%s %s abc %s\n", kv_scope[i].key[j], ":",";");
        }
        fprintf(fp, "%s\n\n","}");
    }
    fclose(fp);
}

void gen_test_file(char *path_prefix)
{
    int i,j;
    FILE *fp;
    char path[1024];
    sprintf(path, "%s%s",path_prefix,"test.c");
    fp  = fopen(path,"w+");
    fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n","//","//test.c","//parseconf","//","//Auto generated by gen_conf","//  Copyright (c) 2015 ruochenzuo. All rights reserved.","//");
    fprintf(fp,"%s\n", "#include <stdio.h>");
    fprintf(fp,"%s\n\n", "#include \"parse_conf.h\"");
    for(i = 0; i < value_scope_num ; ++i)
    {
        fprintf(fp,"%s%s%s\n","extern int " ,my_tolower(value_scope[i]),";");
    }
    fprintf(fp,"\n%s\n", "int main(int argc, char* argv[])\n\
{\n\
    int ret;\n\
    if(argc != 2){\n\
        printf(\"usage: ./parse_conf <conf_path>\\n\");\n\
        return 1;\n\
    }\n\
    ret = parse_conf(argv[1]);\n\
    if (ret) {");
    
    print_tab(2, fp);
    fprintf(fp,"%s\n", "int i;");
    for( i = 0; i < value_scope_num; ++i)
    {
        print_tab(2, fp);
        fprintf(fp,"%s%s%s\n", "printf(\"\\n",my_tolower(value_scope[i]),":\\n\");");
        print_tab(2, fp);
        fprintf(fp,"%s%s%s\n", "for(i = 0 ; i  < ",value_scope[i],"; i++)");
        print_tab(3, fp);
        fprintf(fp,"%s%s%s\n", "printf(\" %s\\n\",",my_toupper(value_scope[i]),"[i]);");
        
    }
    for( i = 0 ; i < key_value_scope_num; ++i)
    {
        print_tab(2, fp);
        fprintf(fp,"%s%s%s\n", "printf(\"\\n",my_tolower(kv_scope[i].scope_key),":\\n\");");
        for(j = 0; j < kv_scope[i].n; ++j)
        {
            char upkey[MAXKEYLEN];
            char lowkey[MAXKEYLEN];
            strcpy(upkey,my_toupper(kv_scope[i].key[j]));
            strcpy(lowkey,my_tolower(kv_scope[i].key[j]));
            print_tab(2, fp);
            fprintf(fp,"%s%s%s%s%s\n","printf(\"\\n",lowkey," : %s\\n\",",upkey,");" );
        }
    }
    
    
    fprintf(fp,"%s\n","\t\t}\n\
    return 0;\n\
}");
    
}


int gen_read_conf(const char *path)
{
    FILE *fp;
    char line[MAXLINELEN] = {0};
    char *ret = NULL;
    char *p = NULL;
    char key[MAXKEYLEN];
    fp = fopen(path,"r+");
    if (fp == NULL) {
        perror("open configure file error!");
        return 0;
    }
    
    ret = fgets(line, MAXLINELEN, fp);
    p =strstr(line,":");
    get_key(p + 1, line + strlen(line) - 1, key);
    value_scope_num = atoi(key);
    value_scope = (mykey_t*)calloc(value_scope_num, sizeof(mykey_t));
    int i = 0;
    for(; i < value_scope_num; ++i)
    {
        fgets(line, MAXLINELEN, fp);
        p =strstr(line,":");
        get_key(p + 1, line + strlen(line) - 1, value_scope[i]);
        
    }
    
    ret = fgets(line, MAXLINELEN, fp);
    p =strstr(line,":");
    get_key(p + 1, line + strlen(line) - 1, key);
    key_value_scope_num = atoi(key);
    kv_scope = (key_value_scope_t*)calloc(key_value_scope_num,sizeof(key_value_scope_t));
    for(i = 0 ; i < key_value_scope_num; ++i)
    {
        fgets(line, MAXLINELEN, fp);
        p =strstr(line,":");
        char *m = NULL;
        m = strstr(p+1,":");
        get_key(p+1, m, kv_scope[i].scope_key);
        p = m;
        m = strstr(m+1,":");
        get_key(p+1, m, key);
        kv_scope[i].n = atoi(key);
        kv_scope[i].key = (mykey_t *)calloc(kv_scope[i].n, sizeof(mykey_t));
        int j = 0;
        for (; j < kv_scope[i].n; ++j) {
            p = m;
            m = strstr(m+1,":");
            get_key(p+1, m, kv_scope[i].key[j]);
        }
    }
    fclose(fp);
    return 1;
}


//free the space calloc in gen_read_conf()
void gen_read_conf_end()
{
    free(value_scope);
    int i = 0;
    for(; i < key_value_scope_num; ++i)
    {
        free(kv_scope[i].key);
    }
    free(kv_scope);
}

void print()
{
    int i;
    for(i = 0 ; i < value_scope_num ; ++i)
    {
        printf("%s\n",value_scope[i]);
    }
    for(i = 0; i < key_value_scope_num ; ++i)
    {
        int j;
        printf("%s:",kv_scope[i].scope_key);
        for(j = 0; j < kv_scope[i].n; ++j)
        {
            printf("%s  ",kv_scope[i].key[j]);
        }
        printf("\n");
    }
}


int main(int argc, const char * argv[]) {
    
    if(argc != 2){
        printf("usage: ./gen <conf_path>\n");
        return 1;
    }
    // insert code here...
    gen_read_conf(argv[1]);
    print();
    gen_parse_conf("conf/");
    gen_parse_conf_head_func_file("gen/");
    gen_parse_conf_func_file("gen/");
    gen_test_file("gen/");
    gen_read_conf_end();
    return 0;
}
