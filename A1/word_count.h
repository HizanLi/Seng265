//Linxin Li V00929949
#ifndef A1_H
#define A1_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define FILE_NOT_FOUND  1
#define BAD_ARGS        2
#define MAX_FILESIZE    5000
#define MAX_WORD_CNT    750
#define MAX_WORD_LEN    35 //Supercalifragilisticexpialidocious
#define TRUE 1
#define FALSE 0

/* 
 * Debugging print function that can has the same 
 * format as `printf`.  If `-D DEBUG` is passed to
 * the compiler then the print code is compiled, 
 * otherwise, it's an empty function.
 *
 */
void debug(const char* format, ... );
void debug(const char* format, ... ) {
#ifdef DEBUG 
	va_list args;
	
	fprintf(stdout, "  -->  ");
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);

	fprintf(stdout, "\n");
	fflush(stdout);
#endif	
}

typedef struct word_t{
    char matrix[MAX_WORD_CNT][MAX_WORD_LEN];
    char matrix_with_no_same_word[MAX_WORD_CNT][MAX_WORD_LEN];
    int length[MAX_WORD_LEN+1];
    int num_word_read; //actual meanning is number of word readed form txt file(include some empty row)
    int different_word_with_same_length[MAX_WORD_LEN+1];
    int size; //could be used for some purpose, but since no requirment in assignmentm so leave it.
    int act_num_word_read;//how many line is not empty
} word_t;

word_t read_file(char* filepath, char* buffer);
void remove_newline(word_t *target);
void print_matrix(word_t *target);
void remove_punctuation(word_t *target);
void calculate_frequency(word_t *target);
size_t calculate_size(word_t *target);
void check_num_word_read(word_t *target);

//part a
void by_freq(word_t *target);

//part b
void by_len(word_t *target);
double find_median(int order[],int number_element);
int compare_part_b (const void * a, const void * b);

//part c
void by_alphanum(word_t *target);
void calculate_frequency_with_different_words(word_t *target);
int compare_part_c (const void *arg1, const void *arg2);
void print_histogram(word_t* buckets, int output_order[],int ind);


word_t read_file(char* filepath, char* buffer){
    // create a empty bucket
    word_t bucket={0};
    bucket.num_word_read=0;
    FILE *fp;
    // str length change as input file change
    char str[150];
    fp = fopen(filepath, "r");
    while ( fgets (str, 150, fp)!=NULL ) {
        if(strcmp(str," ")!=-1 && strcmp(str," ")!=0 ){
            buffer = strtok (str, " ");
            while (buffer && bucket.num_word_read < MAX_WORD_CNT) {
                strncpy (bucket.matrix[bucket.num_word_read],buffer, MAX_WORD_LEN);
                bucket.num_word_read++;
                buffer = strtok (NULL, " ");
            }
        }

    }
    fclose(fp);
    remove_newline(&bucket);
    remove_punctuation(&bucket);
    bucket.size = calculate_size(&bucket);
    check_num_word_read(&bucket);
    return bucket;
}

void check_num_word_read(word_t *target){
    target->act_num_word_read=0;
	for(int i=0;i<target->num_word_read;i++){
	    if(strlen(target->matrix[i])>0){
	    	target->act_num_word_read +=1;
	 }				
    }
}

void remove_newline(word_t *target){ //remove the '/n' at the end of each line (read from file)
    for(int i=0;i<target->num_word_read;i++){
        for(int j=0; j<=strlen(target->matrix[i]);j++){
            if(target->matrix[i][j]=='\n'){
                target->matrix[i][j]='\0';
            }
        }
    }
}

void remove_punctuation(word_t *target){ //remove characters that at the beginning and end of each index
    for(int i=0;i<target->num_word_read;i++) {
        int length = strlen(target->matrix[i]);
        //determine if the last index is punctuation
        while(isalnum(target->matrix[i][length-1])==0){
            target->matrix[i][length-1]='\0';
            length--;
        }
        // find the length after remove punctuation at end
        length = strlen(target->matrix[i]);
        //determine if the first index is punctuation
        while(length>0 && isalnum(target->matrix[i][0])==0){
            for(int j=0;j<length-1;j++){
                target->matrix[i][j]=target->matrix[i][j+1];
            }
            target->matrix[i][length-1]='\0';
            length--;
        }
    }
}

size_t calculate_size(word_t *target){
    int total_character_in_target=0;
    for(int i=0;i<target->num_word_read;i++){
        total_character_in_target += strlen(target->matrix[i]);
    }
    return total_character_in_target;
}

// calculate the frequency of a word with certain length
void calculate_frequency(word_t *target){
    for(int i=0;i<MAX_WORD_LEN+1;i++){
        target->length[i]=0;
    }
    for(int i=0;i<target->num_word_read+1;i++){
        target->length[strlen(target->matrix[i])] = target->length[strlen(target->matrix[i])]+1;
    }
}

// for a1 part a
void by_freq(word_t *target){
    if(target->act_num_word_read==0){
        return; 
    }	
    for(int i=1;i<MAX_WORD_LEN+1;i++) {
        if (target->length[i] != 0) {
            if(i>=10){
                fprintf(stdout,"Count[%d]",i);
            }else{
                fprintf(stdout,"Count[0%d]",i);
            }
            if(target->length[i]>=10){
                fprintf(stdout,"=%d;",target->length[i]);
            } else{
                fprintf(stdout,"=0%d;",target->length[i]);
            }
            fprintf(stdout,"\n");
        }
    }
}

//for a1 part b
void by_len(word_t *target){
    if(target->act_num_word_read==0){
        fprintf(stdout,"Median word length: 0.0\n");
        return;
    }
    int back_up_length[MAX_WORD_LEN+1];
    int sorted_back_up_length[MAX_WORD_LEN+1];
    int output_order[MAX_WORD_LEN+1];
    //Copying the int array that stores the frequency
    for(int i=0;i<MAX_WORD_LEN+1;i++){
        sorted_back_up_length[i]=target->length[i];
        back_up_length[i]=target->length[i];
        output_order[i]=0;
    }
    //Sort the back_up_length array from small to larger number.
    qsort(sorted_back_up_length, MAX_WORD_LEN+1, sizeof(int), compare_part_b);
    int frequency;
    int index_output_order=0;
    for(int i=MAX_WORD_LEN; i>0; i--){
        frequency=sorted_back_up_length[i];
        if(frequency==0){
            break;
        }
        if(frequency>0){
            for(int j=1; j<MAX_WORD_LEN+1; j++){//start at index 1 because target->length[i] has 36 index in range 0~35
                if(back_up_length[j]==frequency){
                    output_order[index_output_order]=j;
                    index_output_order++;
                    back_up_length[j]=0;
                    break;
                }
            }
        }
    }
    int backup_output_order[index_output_order];
    for( int i = 0 ; i < index_output_order; i++ ) {
        backup_output_order[i]=output_order[i];
    }
    qsort(backup_output_order, index_output_order, sizeof(int), compare_part_b);
    double median = find_median(backup_output_order,index_output_order);
    for(int i=0;i<index_output_order;i++){
        if(output_order[i]>=10){
            fprintf(stdout,"Count[%d]",output_order[i]);
        }else{
            fprintf(stdout,"Count[0%d]",output_order[i]);
        }
        if(target->length[output_order[i]]>=10){
            fprintf(stdout,"=%d;",target->length[output_order[i]]);
        } else{
            fprintf(stdout,"=0%d;",target->length[output_order[i]]);
        }
        fprintf(stdout,"\n");
    }

    fprintf(stdout,"Median word length: %.1f\n",median);
}

// find the median of word lengthes
double find_median(int order[],int number_element){
    if(number_element==0){
        return 0;
    } else if(number_element==1){
        return order[0];
    } else if(number_element%2==0){ //even number of element
        int median_right = number_element/2;
        int median_left=median_right-1;
        double median_sum = (order[median_left]+order[median_right])/2.0;
        return median_sum;
    } else if(number_element%2==1){
        int median = number_element/2;
        double median_sum = order[median];
        return median_sum;
    }else{
        return 0.0;
    }
}

int compare_part_b (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}// this compare function is modified base on the compare function in  https://www.tutorialspoint.com/c_standard_library/c_function_qsort.html

// for a1 part c
void by_alphanum(word_t *target){
    if(target->act_num_word_read==0){
        return;
    }
    int back_up_length[MAX_WORD_LEN+1];
    int sorted_back_up_length[MAX_WORD_LEN+1];
    int output_order[MAX_WORD_LEN+1];
    //Copying the int array that stores the frequency
    for(int i=0;i<MAX_WORD_LEN+1;i++){
        sorted_back_up_length[i]=target->length[i];
        back_up_length[i]=target->length[i];
        output_order[i]=0;
    }
    //Sort the back_up_length array
    qsort(sorted_back_up_length, MAX_WORD_LEN+1, sizeof(int), compare_part_b);
    int frequency;
    int index_output_order=0;
    for(int i=MAX_WORD_LEN; i>0; i--){
        frequency=sorted_back_up_length[i];
        if(frequency==0){
            break;
        }
        if(frequency>0){
            for(int j=1; j<MAX_WORD_LEN+1; j++){
                if(back_up_length[j]==frequency){
                    output_order[index_output_order]=j;
                    index_output_order++;
                    back_up_length[j]=0;
                    break;
                }
            }
        }
    }
    for(int i=0;i<index_output_order;i++){
        if(output_order[i]>=10){
            fprintf(stdout,"Count[%d]",output_order[i]);
        }else{
            fprintf(stdout,"Count[0%d]",output_order[i]);
        }
        if(target->length[output_order[i]]>=10){
            fprintf(stdout,"=%d;",target->length[output_order[i]]);
        } else{
            fprintf(stdout,"=0%d;",target->length[output_order[i]]);
        }

        print_histogram(target,output_order,i);
    }
}

void calculate_frequency_with_different_words(word_t *target){
    char back_up_matrix[MAX_WORD_CNT][MAX_WORD_LEN];
    for(int i=0;i<MAX_WORD_LEN+1;i++){
        target->different_word_with_same_length[i]=0;
    }
    for(int i=0;i<target->num_word_read;i++){
        strcpy(back_up_matrix[i],target->matrix[i]);
    }
    char compared_char[MAX_WORD_LEN];
    for(int j = 0; j < target->num_word_read ; j++) {
        strcpy(compared_char,back_up_matrix[j]);
        for (int i = j+1; i < target->num_word_read ; i++) {
            if (strcmp(compared_char, "")!=0 && strcmp(compared_char, back_up_matrix[i]) == 0) {
                strcpy(back_up_matrix[i],"");
            }
        }
    }
    for(int i=0;i<target->num_word_read;i++){
        if(strlen(back_up_matrix[i])>0){
            target->different_word_with_same_length[strlen(back_up_matrix[i])] +=1;
        }
    }
    for(int i=0;i<target->num_word_read;i++){
        strcpy(target->matrix_with_no_same_word[i],back_up_matrix[i]);
    }
}

void print_histogram(word_t* target, int output_order[],int ind){
    fprintf(stdout," (words: ");
    int length = output_order[ind];
    int num_word = target->different_word_with_same_length[length];
    char *storage[num_word];
    int index=0;
    for(int j=0;j<target->num_word_read;j++){
        if(strlen(target->matrix_with_no_same_word[j])==length){
            storage[index]=target->matrix_with_no_same_word[j];
            index+=1;
        }
    }
    qsort(storage,num_word,sizeof(char *),compare_part_c);
    for(int j=0;j<num_word;j++){
        fprintf(stdout,"\"%s\"" , storage[j]);
        if(num_word-j==2){
            fprintf(stdout," and ");
        }else if(num_word-j >2){
            fprintf(stdout,", ");
        }
    }
    fprintf(stdout,")\n");
}

int compare_part_c (const void *arg1, const void *arg2){
    return(strcmp(*(char **)arg1, *(char **)arg2));
}// this compare function is modified base on the compare function in https://www.tutorialspoint.com/c_standard_library/c_function_qsort.html

#endif
