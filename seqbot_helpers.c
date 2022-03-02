#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "seqbot_helpers.h"

/* Return the melting temperature of sequence, or -1 if the sequence is invalid.
 * The melting temperature formula is given in the handout.
 * An invalid sequence is a sequence of length 0, or a sequence that contains
 * characters other than 'A', 'C', 'G', 'T'.
 */
int calculate_melting_temperature(char *sequence, int sequence_length){
    if (sequence_length == 0){
        return -1;
    }

    int n_A = 0;
    int n_C = 0;
    int n_G = 0;
    int n_T = 0;
    int T = 0;

    // TODO: complete this function
    for (int i = 0; i < sequence_length; i++){
        if (sequence[i] == 'A'){
            n_A += 1;
        }
        else if (sequence[i] == 'C'){
            n_C += 1;
        }
        else if (sequence[i] == 'G'){
            n_G += 1;
        }
        else if (sequence[i] == 'T'){
            n_T += 1;
        }
        else {
            return -1;
        } 
    }
    T = (n_A + n_T) * 2 + (n_C + n_G) * 4;
    return T;
}

/* Prints the instructions to make a molecule from sequence.
 * If an invalid character is found in sequence print
 * "INVALID SEQUENCE" and return immediately
 */
void print_instructions(char *sequence, int sequence_length){

    if (calculate_melting_temperature(sequence, sequence_length) == -1){
        printf("INVALID SEQUENCE\n");
    }

    printf("START\n");

    char curr = sequence[0];
    int counter = 1;

    if (sequence_length > 1){

        for (int i = 1; i < sequence_length; i++){

            if (curr == sequence[i]){
                counter += 1;
            }
            else{
                printf ("WRITE %c %d\n", curr, counter);
                curr = sequence[i];
                counter = 1;
            }
        }
    }
    
    printf ("WRITE %c %d\n", curr, counter);
    printf("SET_TEMPERATURE %d\n", calculate_melting_temperature(sequence, sequence_length));
    printf("END\n");  
}

/* Print to standard output all of the sequences of length k.
 * The format of the output is "<length> <sequence> 0" to 
 * correspond to the input format required by generate_molecules_from_file()
 * 
 * Reminder: you are not allowed to use string functions in these files.
 */


// a helper function: recursive

void gam_helper(char arr[], char DNA[], int k, int length){
    // at the end of array
    if (length == k){

        printf("%d ", k);

        for (int i = 0; i < k; i++){
            printf("%c", arr[i]);
        }

        printf(" %d\n", 0);
        return;
    }

    for(int j = 0; j < 4; j++){
        arr[length] = DNA[j];
        gam_helper(arr, DNA, k, length + 1);
    }
}


void generate_all_molecules(int k){
    // TODO: complete this function
    char DNA[4] = {'A', 'C', 'G', 'T'};
    char arr[k]; // stores all the combinations

    gam_helper(arr, DNA, k, 0);
    
}


/* Print the instructions for each of the sequences found in filename according
 * to the mode provided.
 * filename contains one sequence per line, and the format of each line is
 * "<length> <sequence> <mode>" where 
 *     - <length> is the number of characters in the sequence 
 *     - <sequence> is the array of DNA characters
 *     - <mode> is either 0, 1, 2, or 3 indicating how the <sequence> should 
 *              be modified before printing the instrutions. The modes have the 
 *              following meanings:
 *         - 0  - print instructions for sequence (unmodified)
 *         - 1  - print instructions for the the complement of sequence
 *         - 2  - print instructions for the reverse of sequence
 *         - 3  - print instructions for sequence where it is complemented 
 *                and reversed.
 * 
 * Error checking: If any of the following error conditions occur, the function
 * immediately prints "INVALID SEQUENCE" to standard output and exits the 
 * program with a exit code of 1.
 *  - length does not match the number of characters in sequence
 *  - length is not a positive number
 *  - sequence contains at least one invalid character
 *  - mode is not a number between 0 and 3 inclusive
 * 
 * You do not need to verify that length or mode are actually integer numbers,
 * only that they are in the correct range. It is recommended that you use a 
 * fscanf to read the numbers and fgetc to read the sequence characters.
 */

void print_complement(char *sequence, int sequence_length){

    char complement_sequence[sequence_length];

    for (int i = 0; i < sequence_length; i++){
        if (sequence[i] == 'A'){
            complement_sequence[i] = 'T';
        }
        else if (sequence[i] == 'C'){
            complement_sequence[i] = 'G';
        }
        else if (sequence[i] == 'G'){
            complement_sequence[i] = 'C';
        }
        else if (sequence[i] == 'T'){
            complement_sequence[i] = 'A';
        }  
    }

    print_instructions(complement_sequence, sequence_length);
}


void print_reverse(char *sequence, int sequence_length){

    char reverse_sequence[sequence_length];

    for (int i = 0; i < sequence_length; i++){
        reverse_sequence[i] = sequence[sequence_length - 1 - i];
    }

    print_instructions(reverse_sequence, sequence_length);
}


// cr represents complemented and reversed
void print_cr(char *sequence, int sequence_length){

    char c_sequence[sequence_length];
    char cr_sequence[sequence_length];

    for (int i = 0; i < sequence_length; i++){
        if (sequence[i] == 'A'){
            c_sequence[i] = 'T';
        }
        else if (sequence[i] == 'C'){
            c_sequence[i] = 'G';
        }
        else if (sequence[i] == 'G'){
            c_sequence[i] = 'C';
        }
        else if (sequence[i] == 'T'){
            c_sequence[i] = 'A';
        }   
    }

    for (int i = 0; i < sequence_length; i++){
        cr_sequence[i] = c_sequence[sequence_length - 1 - i];
    }

    print_instructions(cr_sequence, sequence_length);
}


void generate_molecules_from_file(char* filename){
   
    // open the file
    FILE *fp;
    fp = fopen(filename, "r"); 

    // check the error
    if (fp == NULL){
        exit(1);
    }
    
    char sequence[100];  
    int length;
    int mode;  

    while (fscanf(fp, "%d %s %d", &length, sequence, &mode) == 3){  
            if (mode == 0) {  
                print_instructions(sequence, length);  
            }   
            else if (mode == 1) {  
                print_complement(sequence, length);  
            }
            else if (mode == 2) {  
                print_reverse(sequence, length);  
            } 
            else if (mode == 3) {  
                print_cr(sequence, length);  
            } 
            else {  
                printf("INVALID SEQUENCE\n");  
                exit(1);  
            }  
        } 

    fclose(fp);  
 
}
