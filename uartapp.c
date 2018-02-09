#include "devicedrivers.h"

int gets(char* str, int str_size)
{
    char c;
    int i=0, ret=0;
    do{
        c=readUART0();
        //Eingabe reflektieren
        writeUART0(c);
        str[i]=c;
        i++;
        if(i>=str_size){
            ret=-1;
            break;
        }
    }while(c!='\n');
    
    //newline nicht übernehmen
    ret=i-1;
    str[ret]='\0';
    
    return ret;
}

int puts(char* str)
{
    while(*str)
        writeUART0(*str);
    return 0;
}

int put_line(char* line)
{
    if(puts(line)==0)
    {
        writeUART0('\n');
    }
    return 0;
}

int itoa(int32_t number, char* str, int str_max)
{
    char digits[]="0123456789";
    char buf[12];
    int i=0, counter=0,sign;
    
    str_max--;
    
    sign=number>0?1:-1;
    
    if( (sign>0&&str_max<1) || (sign<0&&str_max<2) )
        return -1;
    
    if(number==0){
        str[0]='0';
        str[1]='\0';
        return 0;
    }
    //abs berechnen
    number=number*sign;
    for ( ; number; number /= 10)
        buf[counter++] = digits[number%10];
    
    if(sign<0){
        str[0]='-';
        str++;
        str_max--;
    }
    for (i = 0; i < counter; ++i){
        if(i>=str_max){
            str[i-1]='\0';
            return -1;
        }
        str[i] = buf[counter - i - 1];
    }
    str[i]='\0';
    return i;
}

int _strcmp(char* str1, char* str2)
{
    int i=0;
    while(str1[i]&&str2[i]){
        if( str1[i]!=str2[i])
            return -1;
        i++;
    }
    return str1[i]==0&&str2[i]==0;
}

int _strncmp(char* str1, char* str2, int n)
{
    int i=0;
    while(str1[i]&&str2[i]){
        if( str1[i]!=str2[i] )
            return -1;
        if(i==n)
            return 0;
        i++;
    }
    return str1[i]==0&&str2[i]==0;
}

void list_procs()
{
}

void run_proc(char* name)
{
}
/*
void uartapplication(void)
{
    char line[80];
    int len;
	initUART0();
	while(1)
	{
        put_line("EbSy>");
		len=gets(line, 80);
        if(len<0){
            put_line("command too long");
            continue;
        }
        if(len==0) //kein Kommando
            continue;
        if(_strcmp("ps", line)==0){
            list_procs();
            continue;
        }else if(_strncmp("run ", line, 4)==0){
            run_proc(line+4);
            continue;
        }else{
            puts("unknown command ->"); put_line(line); 
            continue;
        }
    }
}*/

void uartapplication(void)
{
	uint8_t data;
	initUART0();
	while(1)
	{
		data = readUART0();
		writeUART0(data);
	}
}
