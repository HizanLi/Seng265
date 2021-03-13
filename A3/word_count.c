//Linxin Li
#include "word_count.h"


int main(int argc, char *argv[]) {
    int infile = FALSE;
    int sort = FALSE;
    int index_input_file = -1;

    if (argc < 2) {
        fprintf(stderr, "missing '--infile <filename> [--sort]'\n");
        exit(BAD_ARGS);
    }
    //Find mode from input argv[]
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--infile") == 0) {
            infile = TRUE;
        } else if (strcmp(argv[i], "--sort") == 0) {
            sort = TRUE;
        }
    }
    //If no mode opis found return BAD_ARGS
    if (infile == FALSE) {
        fprintf(stderr, "missing '--infile <filename> [--sort]'\n");
        exit( BAD_ARGS);
    }
    //Find index at which the file is stored.
    int count = 0;
    for (int i = 0; i < argc; i++) {
        if (strstr(argv[i], ".txt") == NULL) {
            count++;
        } else {
            index_input_file = i;
        }
        if (count >= argc) {
            fprintf(stderr, "missing '--infile <filename> [--sort]'\n");
            exit(BAD_ARGS);
        }
    }

    if (infile == TRUE && sort == TRUE) {
        struct word_list_matrix *matrix = (struct word_list_matrix *) malloc(sizeof(struct word_list_matrix));
        check_null(matrix, "error when allocating matrix");
        initialize_matrix(matrix);
        int result = read_file(argv[index_input_file], matrix);
        if (result == -1) {
            free(matrix);
            return 0;
        }
        print_order(matrix, 0);
        free_all(matrix);
    } else if (infile == TRUE) {
        struct word_list_matrix *matrix = (struct word_list_matrix *) malloc(sizeof(struct word_list_matrix));
        check_null(matrix, "error when allocating matrix");
        initialize_matrix(matrix);
        int result = read_file(argv[index_input_file], matrix);
        if (result == -1) {
            free(matrix);
            return 0;
        }
        print_order(matrix, 1);
        free_all(matrix);
    }
    return 0;
}
