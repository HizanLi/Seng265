#!/bin/env python3

#Linxin Li SENG265 Assignment#2

FILE_NOT_FOUND = 1
BAD_ARGS = 2
MAX_FILESIZE = 5000
MAX_WORD_CNT = 750
MAX_WORD_LEN = 35 

import sys

def main():
    infile = False
    sort = False
    print_words = False
    index_input_file = -1
    if "--infile" in sys.argv:
        infile = True
    if "--sort" in sys.argv:
        sort = True
    if "--print-words" in sys.argv:
        print_words = True
    try:
        if (infile == False) and (sort == False) and (print_words == False):
            print(sys.argv[0] + ": missing '--infile <filename> [--sort] [--print-words]'\n",end="")
            return BAD_ARGS

        for index in range(len(sys.argv)):
            if "txt" in sys.argv[index]:
                index_input_file = index
                break
            elif index >= (len(sys.argv)-1):
                print(sys.argv[0]+": missing '--infile <filename> [--sort] [--print-words]'\n",end="")
                return BAD_ARGS
    except:
        print("error: missing '--infile <filename> [--sort] [--print-words]'\n",end="")
        return BAD_ARGS

    if(infile==True and sort==True and print_words==True):
        matrix = read_file(sys.argv[index_input_file])
        if matrix == -1 or matrix == 1:
            return
        else:
            part_c(matrix)

    elif(infile==True and sort==True):
        matrix = read_file(sys.argv[index_input_file])
        if matrix == -1:
            print("Median word length: 0.0")
            return
        elif matrix == 1:
            return
        else:
            part_b(matrix)

    elif(infile==True):
        matrix = read_file(sys.argv[index_input_file])
        if matrix == -1 or matrix == 1:
            return
        else:
            part_a(matrix)

class matrix:
    def __init__(self, words, num_word_read, matrix_of_wards):
        self.words = []
        self.words = words
        self.num_word_read = []
        self.num_word_read = num_word_read
        self.matrix_of_wards = []
        self.matrix_of_wards = matrix_of_wards
        self.frequency = []
        self.matrix_of_wards_differ = []
        self.frequency_differ = []

    def get_words(self):
        return self.words

    def get_matrix_of_wards(self):
        return self.matrix_of_wards

    def get_num_word_read(self):
        return self.num_word_read

    def get_frequency(self):
        return self.frequency

    def get_matrix_of_wards_differ(self):
        return self.matrix_of_wards_differ

    def get_frequency_differ(self):
        return self.frequency_differ

    def set_frequency(self, frequency):
        self.frequency = frequency[:]

    def set_matrix_of_wards_differ(self, matrix_of_wards_differ):
        self.matrix_of_wards_differ = matrix_of_wards_differ[:]

    def set_frequency_differ(self, frequency_differ):
        self.frequency_differ = frequency_differ[:]

    def print_matrix_of_wards_differ(self, index):
        count = 0
        if len(self.matrix_of_wards_differ[index]) != 0:
            self.matrix_of_wards_differ[index].sort()
            print(" (words: ", end="")
            for word in self.matrix_of_wards_differ[index]:
                print("\""+word+"\"",end="")
                if (len(self.matrix_of_wards_differ[index]) - count) == 2:
                    print(" and ",end="")
                    count += 1
                elif (len(self.matrix_of_wards_differ[index]) - count) > 2:
                    print(", ",end="")
                    count += 1
        print(")\n",end="")


# Read all word from txt file and store into words (all punctuation removed)
def read_file(path):
    words = []
    num_word_read = 0
    punctuations = '''!()-[]{};:'"\,<>./?@#$%^&*_~'''
    try:
        f = open(path, "r")
        for sentence in f:
            sentence.strip()
            sentence = sentence.split()
            for word in sentence:
                for character in word:
                    if character in punctuations:
                        word = word.replace(character, "")
                words.append(word)
                num_word_read = num_word_read + 1
        f.close()
    except:
        print("unable to open '"+path+" for reading\n",end="")
        return 1
    if num_word_read == 0:
        return -1

    matrix_of_wards = []
    for index in range(MAX_WORD_LEN + 1):
        matrix_of_wards.append([])

    for index in range(num_word_read):
        word_length = len(words[index])
        matrix_of_wards[word_length].append(words[index])

    store = matrix(words, num_word_read, matrix_of_wards)
    store = calculate_frequency(store)
    store = remove_same_word(store)
    store = calculate_frequency_differ(store)
    return store
# end of read_file function


def calculate_frequency(matrix):
    matrix_of_wards = matrix.get_matrix_of_wards()
    frequency = [0] * (1 + MAX_WORD_LEN)
    for index in range(len(matrix_of_wards)):
        if len(matrix_of_wards[index]) > 0:
            frequency[index] = len(matrix_of_wards[index])
    matrix.set_frequency(frequency)
    return matrix
# end of calculate_frequency function


def calculate_frequency_differ(matrix):
    matrix_of_wards = matrix.get_matrix_of_wards_differ()
    frequency = [0] * (1 + MAX_WORD_LEN)
    for index in range(len(matrix_of_wards)):
        if len(matrix_of_wards[index]) > 0:
            frequency[index] = len(matrix_of_wards[index])
    matrix.set_frequency_differ(frequency)
    return matrix
# end of calculate_frequency_differ function


def remove_same_word(matrix):
    abc = matrix.get_matrix_of_wards()
    matrix_of_wards = abc[:]
    words_at_index = []
    for index in range(1, MAX_WORD_LEN + 1):
        for word in matrix_of_wards[index]:
            if word not in words_at_index:
                words_at_index.append(word)
        matrix_of_wards[index] = words_at_index
        words_at_index = []
    matrix.set_matrix_of_wards_differ(matrix_of_wards)
    return matrix
# end of remove_same_word function


def part_a(matrix):
    matrix_of_wards = matrix.get_matrix_of_wards()
    for index in range(1, 1 + MAX_WORD_LEN):
        length = len(matrix_of_wards[index])
        if length > 0:
            if (index >= 10):
                print("Count["+str(index)+"]=",end="")
            else:
                print("Count[0"+str(index)+"]=",end="")
            if (length >= 10):
                print(str(length)+";",end="\n")
            else:
                print("0"+str(length)+";",end="\n")
# end of part_a function


def part_b(matrix):
    matrix_of_wards = matrix.get_matrix_of_wards()
    index_order_index = 0
    order_index = [0] * (1 + MAX_WORD_LEN)
    order_frequency = matrix.get_frequency()
    order_frequency.sort()
    for i in range(MAX_WORD_LEN, 0, -1):
        if order_frequency[i] == 0:
            break
        for index in range(1, MAX_WORD_LEN + 1):
            if len(matrix_of_wards[index]) == order_frequency[i]:
                length = len(matrix_of_wards[index])
                order_index[index_order_index] = index
                index_order_index += 1
                if index >= 10:
                    print("Count["+str(index)+"]=",end="")
                else:
            	    print("Count[0"+str(index)+"]=",end="")
                if length >= 10:
            	    print(str(length)+";",end="\n")
                else:
            	    print("0"+str(length)+";",end="\n")
                matrix_of_wards[index] = []
    order_index.sort()
    median = find_median(order_index)
    print("Median word length: ", end="")
    print(median)
# end of part_b


def find_median(order):
    median = 0
    arr = order[:]
    while 0 in arr:
        del arr[0]
    index_median = int(len(arr) / 2)
    if len(arr) % 2 == 0:
        median = float((arr[index_median] + arr[index_median - 1]) / 2)
        return median
    elif len(arr) % 2 == 1:
        median = float(arr[index_median])
        return median
#end of find_median


def part_c(matrix):
    matrix_of_wards = matrix.get_matrix_of_wards()
    order_index = [0] * (1 + MAX_WORD_LEN)
    order_frequency = matrix.get_frequency()
    order_frequency.sort()
    for i in range(MAX_WORD_LEN, 0, -1):
        if order_frequency[i] == 0:
            break
        for index in range(1, MAX_WORD_LEN + 1):
            if len(matrix_of_wards[index]) == order_frequency[i]:
                length = len(matrix_of_wards[index])
                if index >= 10:
            	    print("Count["+str(index)+"]=",end="")
                else:
            	    print("Count[0"+str(index)+"]=",end="")
                if length >= 10:
            	    print(str(length)+";",end="")
                else:
            	    print("0"+str(length)+";",end="")
                matrix.print_matrix_of_wards_differ(index)
                matrix_of_wards[index] = []
#end of part_c


# Do not change this
if __name__ == "__main__":
    main()

    
