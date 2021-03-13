#include "word_count.h" 
//Linxin Li V00929949
int real_main(int argc, char *argv[]) {
    int infile =FALSE;
    int sort=FALSE;
    int print_words=FALSE;
    int index_input_file=-1;

    if(argc<2){
        printf("%s: missing '--infile <filename> [--sort] [--print-words]'\n",argv[0]);
        return BAD_ARGS;
    }
    //Find mode from input arvg[]
    for (int i=0;i<argc;i++){
        if(strcmp(argv[i],"--infile")==0){
            infile=TRUE;
        }else if(strcmp(argv[i],"--sort")==0){
            sort=TRUE;
        }else if(strcmp(argv[i],"--print-words")==0){
            print_words=TRUE;
        }
    }
    //If no mode is found return BAD_ARGS
    if(infile==FALSE && sort==FALSE && print_words==FALSE){
        printf("%s: missing '--infile <filename> [--sort] [--print-words]'\n",argv[0]);
        return BAD_ARGS;
    }
    //Find index at which the file is stored.
    int count=0;
    for(int i=0;i<argc;i++){
        if(strstr(argv[i],".txt")==NULL){
            count++;
        }else{
            index_input_file=i;
        }
        if (count>=argc){
            printf("%s: missing '--infile <filename> [--sort] [--print-words]'\n",argv[0]);
            return BAD_ARGS;
        }
    }
    // test read file
    FILE *fp;
    fp = fopen(argv[index_input_file], "r");
    if(fp == NULL){
        printf("unable to open '%s' for reading\n",argv[index_input_file]);
        return FILE_NOT_FOUND;
    }
    fclose(fp);
    if(infile==TRUE && sort==TRUE && print_words==TRUE){   //Part C
        char* buffer_part_c;
        buffer_part_c="";
        word_t bucket =read_file(argv[index_input_file], buffer_part_c);
        calculate_frequency(&bucket);
        calculate_frequency_with_different_words(&bucket);
        by_alphanum(&bucket);

    }else if(infile==TRUE && sort==TRUE){  //Part B
        char* buffer_part_b;
        buffer_part_b="";
        word_t bucket =read_file(argv[index_input_file], buffer_part_b);
        calculate_frequency(&bucket);
        by_len(&bucket);

    }else if(infile==TRUE){ //Part A
        char* buffer_part_a;
        buffer_part_a ="";
        word_t bucket =read_file(argv[index_input_file], buffer_part_a);
        calculate_frequency(&bucket);
        by_freq(&bucket);
    }
    return 0;
}



#ifndef TEST
int main(int argc, char *argv[]) {
    return real_main(argc, argv);
}
#endif
