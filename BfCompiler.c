/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define ERR_PRINT(x) printf("Error: File %s doesn't exist\n", x);

#define WARNING_INFO printf("Info : You may type 'export MALLOC_CHECK_=0' manually to remove warning.\n");
#define WARNING_INFO_HELP printf("Note: You should type 'export MALLOC_CHECK_=0' manually to remove warning.\n");

void HelpFunction(char** arg) {
	printf("BrainfuckPlusPlus\n");
	printf("Usage : %s filename [-o output|-c|-d|-r]\n", arg[0]);
	printf("-o output : Set output filename\n");
	printf("-c : Do not compile with gcc\n");
	printf("-d : Do not delete C source file\n");
	WARNING_INFO_HELP /* Definition */
}

int main(int argv, char* argc[]){
    /* Help Message */
    if(argv == 1) {
        HelpFunction(argc);
        return 0;
    }
    
    WARNING_INFO /* Definition */
    
    int i;
    bool doCompile = true;
    bool deleteSource = true;
    char* fileName = argc[1];
    char* outFileName;
    outFileName = (char*)malloc(1024);
    strncpy(outFileName,fileName,1000);
    strcat(outFileName,".o");
    bool isSetOut = false;
    /* Set flags */ 
    for(i=2;i<argv;i++){
        if(isSetOut){
            isSetOut = false;
            outFileName = argc[i];
        } else if(strcmp(argc[i], "-c") == 0){
            doCompile = false;
        } else if(strcmp(argc[i], "-d") == 0){
            deleteSource = false;
        } else if(strcmp(argc[i], "-o") == 0){
            isSetOut = true;
        }
    }
    char outFileC[1024];
    strncpy(outFileC,outFileName,1000);
    strcat(outFileC,".c");

    /* Brainfuck File. */
    FILE* bfFile;
    bfFile = fopen(fileName,"r"); /* Read */
    
    if(bfFile == NULL) { /* Unable to open file */
    	ERR_PRINT(fileName);
        return 2;
    }
    
    char c;
    /* C Code. */
    FILE* cFile;
    cFile = fopen(outFileC,"w"); /* Write */
    int add = 0;
    char prevC = '\0';
    
    /* Add libraries */
    fputs("#include<stdio.h>\n#include<stdlib.h>\n",cFile);
    /* Main */
    fputs("int main(){",cFile);
    fputs("unsigned char* _=(unsigned char*)malloc(32*1024);/*32kB*/if(_==0){printf(\"MEMORY ERROR!\\n\");return 1;}",cFile);
    /* Write codes */
    do{
        c = fgetc(bfFile);
        if(c!=EOF){
            bool isPrint = false;
            switch(c){
                case '>':
                case '<':
                    if((prevC=='+'||prevC=='-')&&add){//process something like +++- = ++
                        if(add==1||add==-1){
                            if(add==1) fputs("++*_",cFile);
                            else fputs("--*_",cFile);
                        }
                        else fprintf(cFile,"(*_)%c=%d",add>0?'+':'-',add>0?add:-add);
                        fputs(";",cFile);
                        add=0;
                    }
                    if(c=='>') add++;
                    else add--;
                    break;
                case '+':
                case '-':
                    if((prevC=='>'||prevC=='<')&&add){//process something like >>>< = >>
                        if(add==1||add==-1){
                            if(add==1) fputs("++_",cFile);
                            else fputs("--_",cFile);
                        }
                        else fprintf(cFile,"_%c=%d",add>0?'+':'-',add>0?add:-add);
                        fputs(";",cFile);
                        add=0;
                    }
                    if(c=='+') add++;
                    else add--;
                    break;
                case '.':
                case ',':
                case '[':
                case ']':
                    if(add){
                        if(prevC=='>'||prevC=='<'){//process something like >>>< = >>
                            if(add==1||add==-1){
                                if(c=='.'){
                                    isPrint = true;
                                    if(add==1) fputs("putchar(*(++_))",cFile);
                                    else fputs("putchar(*(--_))",cFile);
                                }else{
                                    if(add==1) fputs("++_",cFile);
                                    else fputs("--_",cFile);
                                }
                            }
                            else if(c=='.'){
                                isPrint = true;
                                fprintf(cFile,"putchar(*(_%c=%d))",add>0?'+':'-',add>0?add:-add);
                            }else fprintf(cFile,"_%c=%d",add>0?'+':'-',add>0?add:-add);
                            fputs(";",cFile);
                            add=0;
                        }else if(prevC=='+'||prevC=='-'){//process something like +++- = ++
                            if(add==1||add==-1){
                                if(c=='.'){
                                    isPrint = true;
                                    if(add==1) fputs("putchar(++*_)",cFile);
                                    else fputs("putchar(--*_)",cFile);
                                }else{
                                    if(add==1) fputs("++*_",cFile);
                                    else fputs("--*_",cFile);
                                }
                            }
                            else if(c=='.'){
                                isPrint = true;
                                fprintf(cFile,"putchar((*_)%c=%d)",add>0?'+':'-',add>0?add:-add);
                            }else fprintf(cFile,"(*_)%c=%d",add>0?'+':'-',add>0?add:-add);
                            fputs(";",cFile);
                            add=0;
                        }
                    }
                    switch(c){
                        case '.':
                            if(!isPrint) fputs("putchar(*_);",cFile);
                            break;
                        case ',':
                            fputs("*_=getchar();",cFile);
                            break;
                        case '[':
                            fputs("while(*_){",cFile);
                            break;
                        case ']':
                            fputs("}",cFile);
                            break;
                    }
                    break;
            }
        }
        if(c=='>'||c=='<'||c=='+'||c=='-'||c=='.'||c==','||c=='['||c==']') prevC = c;
    } while(c!=EOF);
    
    fputs("return 0;}",cFile);
    fclose(bfFile); /* Close Brainfuck file (Read) */
    fclose(cFile); /* Close C file (Write) */
    if(!doCompile) {
        printf("Output C code : %s\n",outFileC);
        return 0;
    }
    
    printf("Compile with GCC...\n");
    char op[2048] = "gcc ";
    strcat(op,outFileC);
    strcat(op," -o ");
    strcat(op,outFileName);
    system(op);
    
    if(deleteSource){
        strcpy(op,"rm ");
        strcat(op,outFileC);
        system(op);
    }
    
    printf("Done.\nOutput file name : %s\n",outFileName);
    return 0;
}
