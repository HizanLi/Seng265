//Linxin Li
#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define Success 0
#define FILE_NOT_FOUND  1
#define BAD_ARGS        2
#define Unable_to_allocate_memory  3
#define TRUE 1
#define FALSE 0

typedef struct word_node{
    char *word;
    struct word_node* next_word;
    struct word_node* prev_word;

}word_node;

typedef struct word_list{
    struct word_node* word_node_head;
    struct word_node* word_node_tail;
    int words_length;
    int number_word_in_list;
    int number_of_word_with_length_read;
    struct word_list* next_list;
}word_list;

typedef struct word_list_matrix{
    struct word_list* word_list_head;
    struct word_list* word_list_tail;
    int num_list;
    int num_word_read;
    int num_nodes;
} word_list_matrix;

//functions
void initialize_matrix(struct word_list_matrix* matrix);
void initialize_list(struct word_list* new_list,int length);
void increase(struct word_list_matrix* matrix,struct word_list* list, int number_of_word_with_length_read, int number_word_in_list, int num_nodes);
int read_file(char* filepath, struct word_list_matrix* matrix);
void add_word_to_matrix(char *buffer,struct word_list_matrix* matrix);
void insert_list(struct word_list_matrix* matrix,int length);
void insert_node(struct word_list* list_ptr,char* buffer);
int find_in_list(struct word_list* list,char *word);
void print_order(struct word_list_matrix* matrix, int mode);
void print_histogram(struct word_list* list,  int mode);
void set_prev(struct word_list* list_ptr) ;
void check_null(const void* ptr,char* location);
void free_all(struct word_list_matrix* matrix);
void free_lists(struct word_list* list);
void free_words(struct word_node* node);
//functions

void initialize_matrix(struct word_list_matrix* matrix){
    matrix->num_word_read = 0;
    matrix->num_list= 0;
    matrix->num_nodes= 0;
    matrix->word_list_head = NULL;
    matrix->word_list_tail = NULL;
}

void initialize_list(struct word_list* new_list,int length){
    new_list->word_node_head=NULL;
    new_list->word_node_tail=NULL;
    new_list->words_length=length;
    new_list->number_word_in_list=0;
    new_list->number_of_word_with_length_read=0;
    new_list->next_list=NULL;
}

void increase(struct word_list_matrix* matrix,struct word_list* list, int number_of_word_with_length_read, int number_word_in_list, int num_nodes){
    matrix->num_nodes += num_nodes;
    list->number_word_in_list += number_word_in_list;
    list->number_of_word_with_length_read += number_of_word_with_length_read;
}

int read_file(char* filepath, struct word_list_matrix* matrix){
    FILE *fp;
    fp = fopen(filepath, "r");
    if(fp==NULL){
	free(matrix);
  	fprintf(stderr,"unable to open '%s' for read\n",filepath);
        exit(FILE_NOT_FOUND);
    }
    char *line = NULL;
    size_t len = 0;
    size_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
	if (1 == strlen(line)){
	    continue;
	}
  	//remove '\n' at then end of line
        line[read-1]='\0';
        //remove punctuation
        for(int i = 0; i < read; i++){
            if(isalnum(line[i]) == 0 && isspace(line[i]) == 0){
                line[i] = ' ';
            }
        }
        char* buffer = strtok(line," ");
        while(buffer){
            add_word_to_matrix(buffer,matrix);\
            matrix->num_word_read +=1;
            buffer = strtok(NULL," ");
        }
    }
    fclose(fp);
    if (line) {
        free(line);
    }
    if(matrix->num_word_read==0){
        return -1;
    }
    return Success;
}

void add_word_to_matrix(char *buffer,struct word_list_matrix* matrix){
    int length = (int)strlen((char *)buffer);

    struct word_list* cur = matrix->word_list_head;

    int contain = FALSE;
    while(cur != NULL){
        if(cur->words_length == length){
            contain = TRUE;
            break;
        } else{
            cur = cur->next_list;
        }
    }
    if(contain == FALSE){
        insert_list(matrix,length);
    }

    word_list* list_ptr = matrix->word_list_head;
    for (int i = 0; i < matrix->num_list; i++) {
        if (list_ptr->words_length == length) {
            if(list_ptr->word_node_head == NULL){
                insert_node(list_ptr,buffer);
                increase(matrix,list_ptr,1,1,1);
                return ;
            }else if (list_ptr->word_node_head != NULL){
                if (find_in_list(list_ptr, buffer) == FALSE) {
                    insert_node(list_ptr,buffer);
                    increase(matrix,list_ptr,1,1,1);
                    return ;
                } else {
                    increase(matrix,list_ptr,1,0,0);
                    return ;
                }
            }
        }
        list_ptr = list_ptr->next_list;
    }
}

void insert_list(struct word_list_matrix* matrix,int length){
    matrix->num_list += 1;
    struct word_list* new_list = (struct word_list*)malloc(sizeof(struct word_list));
    check_null(new_list,"error when allocating new list");
    initialize_list(new_list,length);
    if(matrix->word_list_head == NULL){
        matrix->word_list_head = new_list;
        matrix->word_list_tail = matrix->word_list_head;
        return ;
    }

    struct word_list* cur = matrix->word_list_head;
    struct word_list* cur_next = cur->next_list;

    while(1){
        if (cur->words_length > length) {
            new_list->next_list = cur;
            matrix->word_list_head = new_list;
            return ;
        }
        if(cur->words_length < length){
            if(cur_next == NULL){
                cur->next_list = new_list;
                matrix->word_list_tail = new_list;
                return ;
            }else{
                if(cur_next->words_length < length){
                    cur = cur_next;
                    cur_next = cur->next_list;
                }else {
                    new_list->next_list = cur_next;
                    cur->next_list = new_list;
                    return ;
                }
            }
        }
    }
}

void insert_node(struct word_list* list_ptr,char* buffer){
    int length = (int)strlen((char *)buffer);
    struct word_node* a_node = (struct word_node*)malloc(sizeof(struct word_node));
    check_null(a_node,"error when allocating new node");
    a_node->word = (char*)malloc((1+length) * sizeof(char));
    check_null(a_node,"error when allocating node->word");
    strcpy(a_node->word, buffer);
    a_node->next_word = NULL;
    a_node->prev_word = NULL;

    if(list_ptr->word_node_head == NULL){
        list_ptr->word_node_head = a_node;
        list_ptr->word_node_tail = list_ptr->word_node_head;
        return ;
    }
    struct word_node* cur = list_ptr->word_node_head;
    struct word_node* cur_next = cur->next_word;
    while(1){
        if (strcmp(a_node->word, cur->word) < 0) {
            a_node->next_word = cur;
            list_ptr->word_node_head = a_node;
            return ;
        }
        if(strcmp(a_node->word, cur->word) > 0){
            if(cur_next == NULL){
                cur->next_word = a_node;
                list_ptr->word_node_tail = a_node;
                return ;
            }else{
                if(strcmp(a_node->word, cur_next->word) > 0){
                    cur = cur_next;
                    cur_next = cur->next_word;
                }else {
                    a_node->next_word = cur_next;
                    cur->next_word = a_node;
                    return ;
                }
            }
        }
    }
}

int find_in_list(struct word_list* list,char *word){
    struct word_node* node_ptr = list->word_node_head;
    if(node_ptr == NULL){
        return FALSE;
    }
    do{
        if (strcmp(node_ptr->word,word) == 0){
            return TRUE;
        }
        node_ptr = node_ptr->next_word;
    } while(node_ptr != NULL);
    return  FALSE;
}

void print_order(struct word_list_matrix* matrix, int mode){
    if(matrix->num_word_read==0){
        return ;
    }
    word_list* list_ptr = matrix->word_list_head;

    while(list_ptr != NULL){
        if (list_ptr->number_word_in_list > 0){
            if(list_ptr->words_length >= 10){
                fprintf(stdout,"Count[%d]",list_ptr->words_length);
            }else{
                fprintf(stdout,"Count[0%d]",list_ptr->words_length);
            }
            if(list_ptr->number_of_word_with_length_read >= 10){
                fprintf(stdout,"=%d;",list_ptr->number_of_word_with_length_read );
            }else{
                fprintf(stdout,"=0%d;",list_ptr->number_of_word_with_length_read );
            }
            print_histogram(list_ptr,mode);
        }
        list_ptr = list_ptr->next_list;
    }
}

void print_histogram(struct word_list* list_ptr, int mode){
    fprintf(stdout," (words: ");
    if(mode == 1){
        struct word_node* cur = list_ptr->word_node_head;
        while(cur != NULL){
            fprintf(stdout,"\"%s\"" , cur->word);
            if(cur->next_word == NULL){
                break;
            }
            if(list_ptr->number_word_in_list >2 && cur->next_word->next_word != NULL){
                fprintf(stdout,", ");
            }else{
                fprintf(stdout," and ");
            }
            cur = cur->next_word;
        }
    }else {
        set_prev(list_ptr);
        struct word_node* cur = list_ptr->word_node_tail;
        while(cur != NULL){
            fprintf(stdout,"\"%s\"" , cur->word);
            if(cur->prev_word == NULL){
                break;
            }
            if(list_ptr->number_word_in_list >2 && cur->prev_word->prev_word != NULL){
                fprintf(stdout,", ");
            }else{
                fprintf(stdout," and ");
            }
            cur = cur->prev_word;
        }
    }
    fprintf(stdout,")\n");
}

void check_null(const void* ptr,char* location){
    if(ptr == NULL){
        printf("%s\n",location);
        exit(Unable_to_allocate_memory);
    }
}

void set_prev(struct word_list* list_ptr) {
    struct word_node* cur = list_ptr->word_node_head;
    struct word_node* cur_next = cur->next_word;
    while(cur_next != NULL){
        cur_next->prev_word = cur;
        cur = cur_next;
        cur_next = cur_next->next_word;
    }
}

void free_all(struct word_list_matrix* matrix){
    free_lists(matrix->word_list_head);
    free(matrix);
}

void free_lists(struct word_list* list){
    if(list == NULL){
        return;
    }
    free_lists(list->next_list);
    if(list->number_of_word_with_length_read > 0){
        free_words(list->word_node_head);
    }
    free(list);
}

void free_words(struct word_node* node){
    if(node == NULL){
        return;
    }
    free_words(node->next_word);
    free(node->word);
    free(node);
}
