#ifndef __MX_CALC_TEST_H__
#define __MX_CALC_TEST_H__
#include<stdio.h>
#include<stdlib.h>

//global data
void prompt(){
   printf(">");
}
void inte(char c[],int val) {

     FILE *in = fopen ("file.txt", "a+");  /* open the file for reading */
     /* "rt" means open the file for reading text */
     fprintf(in,"%s %d\n",c,val);
     fclose(in);
}
void mul(char st1[], char st2[])
{
   int val1 = extract(st1);
   int val2 = extract(st2);
   int mulval=val1 * val2;
   printf("Multiplied value %d\n",mulval);
   prompt();

}

void add(char st1[], char st2[])
{
   int val1 = extract(st1);
   int val2 = extract(st2); 
   int addedval=val1 + val2;
   printf("Added value %d\n",addedval);
   prompt();

}
void sub(char st1[], char st2[])
{
   int val1 = extract(st1);
   int val2 = extract(st2);
   int addedval=val1 - val2;
   printf("Substracted value %d\n",addedval);
   prompt();
}
int extract(char st1[])
{

     char line[512]; // or however large you think these lines will be
     char string1[20];
     int intval;
     int val2;

     FILE *in1 = fopen ("file.txt", "rw");  /* open the file for reading */
     int cur_line = 0;
     while(fgets(line, 512, in1) != NULL) {
            /* get a line, up to 512 chars from in.  done if NULL */
              sscanf (line, "%s %d\n",string1,&intval);
              if(strcmp(string1,st1)==0)
              {
                val2=intval;
         //       printf("%d",val2);
              }

             // now you should store or manipulate those strings
            //  printf("%s,%d",string1,intval);
      }
   fclose(in1);  /* close the file */
   return(val2);
}

void test() {
           
     char st1[10]="m";
     int val1 = 24;
     char st2[]="n";
     int val2=30;     

     inte(st1,val1);
     inte(st2,val2);
     add(st1,st2);
}

void parse (const char *cmd) {
     
       int i = 0;
       int j = 0;
       char supint[10]="integer";
       char supadd[10]="add";
       char supsub[10]="sub";
       char supmul[10]="mul";
       char supdiv[10]="div"; 
       char supexit[10]="exit";
       char var[10];
       char var1[10];
       char var2[10];
       int arg;
       while(j==0 && i<5) {
   
      if(!strncmp(cmd,supexit,4)) {
           j=1; exit(0); 
          }
      else if (!strncmp(cmd,supmul,3)) {
          if(sscanf(cmd+3,"%s %s",var1,var2) > 0) {
             mul(var1,var2);
             j=1;
          }
      }
      else if (!strncmp(cmd,supdiv,3)) {
          if(sscanf(cmd+3,"%s %s",var1,var2) > 0) {
//             div(var1,var2);
             j=1;
          }
      }
      else if(!strncmp(cmd,supint,7)) {  
          if(sscanf(cmd+7,"%s %d",var,&arg) > 0) {
             inte(var,arg);
             j=1;
          }
      }
     else if(!strncmp(cmd,supadd,3)){
          if(sscanf(cmd+3,"%s %s",var1,var2) > 0) {
           add(var1,var2); 
           j=1;
          }
      }
      else if(!strncmp(cmd,supsub,3)){
          if(sscanf(cmd+3,"%s %s",var1,var2) > 0) {
           sub(var1,var2);
           j=1;
          }
      }
     else {
           printf("Not a Valid Command\n");
           prompt();
           j=1;
       } 
    } //while loop closing 

}
void calctest() {

    int i = 0;

    char cmd[200];
    while(1) {
        prompt();
        char *test=fgets(cmd,200,stdin);
       // printf("%s",test);
       parse(test);
      //  parse(fgets(cmd,200,stdin));
    }

}

#endif
