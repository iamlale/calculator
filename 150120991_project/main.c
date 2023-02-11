
/*
 *
  Lale Hüseyn 150120991
  Fall 2021 CSE 2225.1.1 Project 1
*
*/

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int digit;
    struct Node *nextPtr;
};
typedef struct Node node;

/**
 * Inserts the given value into the beginning of the given linked list.
 * @param startPtr node** - A pointer that points to the address of the
 * start of the linked list that value will be inserted.
 * @param value int - A value that will be inserted
 */
void push(node **startPtr, int value);

/**
 * Inserts the given value at the end of the given linked list.
 * @param startPtr node** - A pointer that points to the address of the
 * start of the linked list that value will be inserted.
 * @param value - A value that will be appended
 */
void append(node **startPtr, int value);

/**
 * Adds the given 2 linked list and returns the start pointer of
 * the linked list that contains result.
 * @param first A pointer that points to first operand
 * @param second A pointer that points to second operand
 * @return node* - Start pointer that points to the linked list that contains result.
 */
node *addLL(node *first, node *second);

/**
 * Multiplies the given 2 linked lists and returns the start pointer of
 * the linked list that contains result.
 * @param multiplicand A pointer that points to multiplicand
 * @param multiplier A pointer that points to multiplier
 * @return node* - Start pointer that points to the linked list that contains result.
 */
node *multiply(node *multiplicand, node *multiplier);

/**
 * Prints the given linked list into the given file.
 * @param outputFile Name of the output file (eg="output.txt")
 * @param startPtr Start pointer of the linked list that will be printed
 */
void fprintLL(FILE *outputFile, node *startPtr);

/**
 * It takes the input file and assigns the first number to
 * multiplicand and second number to multiplier. There must be '*'
 * between numbers to identify between them.
 * @param fileName Name of the input file (eg="input.txt")
 * @param multiplicand A pointer that points to address of multiplicand
 * @param multiplier A pointer that points to address multiplier
 */
void readFromFile(char *fileName, node **multiplicand, node **multiplier);

/**
 * It writes both operand and the result to the given file.
 * \ExampleOutput
 * Multiplicand: 2147483647145 <br>
 * Multiplier: 214748364798567 <br>
 * Result: 461168601656051594429641215
 * @param fileName Name of the output file (eg="output.txt")
 * @param multiplicand A pointer that points to multiplicand
 * @param multiplier A pointer that points to multiplier
 * @param result A pointer that points to result
 */
void writeToFile(char *fileName, node *multiplicand, node *multiplier, node *result);


int main() {

    node *multiplicandStartPtr = NULL;
    node *multiplierStartPtr = NULL;
    node *resultStartPtr = NULL;
    readFromFile("input.txt", &multiplicandStartPtr, &multiplierStartPtr);
    resultStartPtr = multiply(multiplicandStartPtr, multiplierStartPtr);

    /*
    for console checking

    printf("Multiplicand: ");
    printLL(multiplicandStartPtr);
    printf("\nMultiplier: ");
    printLL(multiplierStartPtr);
    printf("\nResult: ");
    printLL(resultStartPtr);

    */

    writeToFile("output.txt", multiplicandStartPtr, multiplierStartPtr, resultStartPtr);
    return 0;
}

// take the input file and make first number multiplicand
// and second number multiplier
void readFromFile(char *fileName, node **multiplicand, node **multiplier) {

    int c = 0;
    FILE *file;
    file = fopen(fileName, "r");
    if (file) {
        while (c != EOF) {
            node *temp_list = NULL;
            while ((c = getc(file)) != EOF && c != '*') {
                push(&temp_list, c - '0');
            }
            if (*multiplicand == NULL)
                *multiplicand = temp_list;
            else
                *multiplier = temp_list;
        }
        fclose(file);
    }
}

// multiplies the given two linked lists and return the result list
node *multiply(node *multiplicand, node *multiplier) {
    int carry = 0;
    node *currentPtr1 = multiplicand;
    node *currentPtr2 = multiplier;

    node *resultNode = NULL;

    int index = 0;
    while (currentPtr2 != NULL) {
        node *tempNode = NULL;

        while (currentPtr1 != NULL) {

            int tempDigit = currentPtr1->digit * currentPtr2->digit;
            tempDigit += carry;
            carry = tempDigit / 10;
            tempDigit = tempDigit % 10;
            append(&tempNode, tempDigit);
            currentPtr1 = currentPtr1->nextPtr;

        }
        for (int i = 0; i < index; i++) {

            push(&tempNode, 0);
        }

        // increase the index when the multiplicand reach the null
        index++;

        if (carry != 0) {
            append(&tempNode, carry);
            carry = 0;
        }
        resultNode = addLL(resultNode, tempNode);
        currentPtr2 = currentPtr2->nextPtr;
        currentPtr1 = multiplicand;
    }

    return resultNode;

}

// add the given two linked lists and returns the start pointer
// of the linked list containing result
node *addLL(node *first, node *second) {
    // res is head node of the result list
    node *res = NULL;
    node *temp, *prev = NULL;
    int carry = 0, sum;

    while (first != NULL || second != NULL) {

        sum = carry;
        if (first)
            sum += first->digit;
        if (second)
            sum += second->digit;

        // Update carry for next calculation
        carry = (sum >= 10) ? 1 : 0;

        // Update sum if it is greater than 10
        sum = sum % 10;

        temp = malloc(sizeof(node));
        temp->digit = sum;
        temp->nextPtr = NULL;


        if (res == NULL)
            res = temp;
        else
            prev->nextPtr = temp;

        // Set previous node for next insertion
        prev = temp;

        if (first)
            first = first->nextPtr;
        if (second)
            second = second->nextPtr;
    }

    if (carry > 0) {
        node *temp1 = malloc(sizeof(node));
        temp1->digit = carry;
        temp1->nextPtr = NULL;
        temp->nextPtr = temp1;
    }

    return res;
}

// write both operand and the result to the given file.
void writeToFile(char *fileName, node *multiplicand, node *multiplier, node *result) {
    FILE *outputFile;
    outputFile = fopen(fileName, "w");
    if (outputFile) {
        fprintf(outputFile, "Multiplicand: ");
        fprintLL(outputFile, multiplicand);
        fprintf(outputFile, "\nMultiplier: ");
        fprintLL(outputFile, multiplier);
        fprintf(outputFile, "\nResult: ");
        fprintLL(outputFile, result);
    }
}

// print the given linked list into the given file
void fprintLL(FILE *outputFile, node *startPtr) {

    node *currentPtr = startPtr;
    // write reverse (e.g 5->4->3->2->1 to 12345)
    if (currentPtr == NULL)
        return;
    fprintLL(outputFile, currentPtr->nextPtr);
    fprintf(outputFile, "%d", currentPtr->digit);
}

// append the given value to the end of the linked list
void append(node **startPtr, int value) {

    node *currentPtr = *startPtr;
    node *temp = malloc(sizeof(node));
    temp->digit = value;
    temp->nextPtr = NULL;

    if (*startPtr == NULL) {
        *startPtr = temp;
        return;
    }
    while (currentPtr->nextPtr != NULL) {
        currentPtr = currentPtr->nextPtr;
    }
    currentPtr->nextPtr = temp;
}

// insert the value into the start point of the given linked list
void push(node **startPtr, int value) {
    node *new_node = malloc(sizeof(node));
    new_node->digit = value;
    if (*startPtr == NULL) {
        new_node->nextPtr = NULL;
        *startPtr = new_node;
        return;
    }
    new_node->nextPtr = *startPtr;
    *startPtr = new_node;
}

//end
