// bigint.c

#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"


void swapBigInt(BigInt** num1, BigInt** num2) {
    BigInt* temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

int isZero(BigInt* num) {
    if (num->head == NULL) {
        return 1; 
    }
    return 0; 
}


BigInt* createBigIntFromInt(int value) {
    BigInt* num = createBigInt();

    if (value == 0) {
        insertDigit(num, 0);
        return num;
    }

    if (value < 0) {
        num->sign = 1;
        value = -value;
    }

    while (value > 0) {
        int digit = value % 10;
        insertDigit(num, digit);
        value /= 10;
    }

    return num;
}


void freeNode(BigIntNode* node) {
    if (node != NULL) {
        freeNode(node->next);
        free(node);
    }
}


void freeBigInt(BigInt* num) {
    freeNode(num->head);
    free(num);
}

int compareMagnitude(BigInt* num1, BigInt* num2) {

    BigIntNode* ptr1 = num1->head;
    BigIntNode* ptr2 = num2->head;

    while (ptr1 != NULL && ptr2 != NULL) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }

    if (ptr1 != NULL && ptr2 == NULL) {
        return 1; 
    } else if (ptr1 == NULL && ptr2 != NULL) {
        return -1; 
    }

 
    ptr1 = num1->head;
    ptr2 = num2->head;

    while (ptr1 != NULL && ptr2 != NULL) {
        if (ptr1->data < ptr2->data) {
            return -1;
        } else if (ptr1->data > ptr2->data) {
            return 1;
        }
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }

    return 0;
}

BigInt* createBigInt() {
    BigInt* num = (BigInt*)malloc(sizeof(BigInt));
    if (num == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    num->head = NULL;
    num->sign = 0;  
    return num;
}


void destroyBigInt(BigInt* num) {
    BigIntNode* current = num->head;
    while (current != NULL) {
        BigIntNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(num);
}

void insertDigit(BigInt* num, int digit) {
    BigIntNode* newNode = (BigIntNode*)malloc(sizeof(BigIntNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = digit;
    newNode->next = num->head;
    num->head = newNode;
}

void printBigInt(BigInt* num) {
    if (num->head == NULL) {
        printf("0");
    } else {
        if (num->sign) {
            printf("-");
        }
        BigIntNode* current = num->head;
        while (current != NULL) {
            printf("%d", current->data);
            current = current->next;
        }
    }
    printf("\n");
}

BigInt* add(BigInt* num1, BigInt* num2) {
    BigInt* result = createBigInt();
    int carry = 0;


    if (num1->sign == num2->sign) {
        result->sign = num1->sign;
    } else {
   
        if (compareMagnitude(num1, num2) < 0) {
            swapBigInt(&num1, &num2);
            result->sign = num1->sign;
        } else {
            result->sign = num1->sign;
        }
    }

    BigIntNode* ptr1 = num1->head;
    BigIntNode* ptr2 = num2->head;

    while (ptr1 != NULL || ptr2 != NULL) {
        int digit1 = (ptr1 != NULL) ? ptr1->data : 0;
        int digit2 = (ptr2 != NULL) ? ptr2->data : 0;
        
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        sum = sum % 10;

        insertDigit(result, sum);

        if (ptr1 != NULL) ptr1 = ptr1->next;
        if (ptr2 != NULL) ptr2 = ptr2->next;
    }

    if (carry > 0) {
        insertDigit(result, carry);
    }

    return result;
}

BigInt* subtract(BigInt* num1, BigInt* num2) {
    BigInt* result = createBigInt();
    int borrow = 0;

    if (compareMagnitude(num1, num2) >= 0) {
        result->sign = num1->sign;
    } else {
        swapBigInt(&num1, &num2);
        result->sign = (num1->sign == 0) ? 1 : 0; 
    }

    BigIntNode* ptr1 = num1->head;
    BigIntNode* ptr2 = num2->head;

    while (ptr1 != NULL) {
        int digit1 = ptr1->data;
        int digit2 = (ptr2 != NULL) ? ptr2->data : 0;
        
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        insertDigit(result, diff);

        ptr1 = ptr1->next;
        if (ptr2 != NULL) ptr2 = ptr2->next;
    }

    while (result->head->data == 0 && result->head->next != NULL) {
        BigIntNode* temp = result->head;
        result->head = result->head->next;
        free(temp);
    }

    return result;
}

BigInt* multiply(BigInt* num1, BigInt* num2) {
    BigInt* result = createBigInt();
    int shift = 0;

    result->sign = (num1->sign == num2->sign) ? 0 : 1;

    BigIntNode* ptr1 = num1->head;

    while (ptr1 != NULL) {
        int digit1 = ptr1->data;
        BigIntNode* ptr2 = num2->head;
        BigInt* partialResult = createBigInt();

        int carry = 0;
        while (ptr2 != NULL) {
            int digit2 = ptr2->data;
            int product = (digit1 * digit2) + carry;
            carry = product / 10;
            product = product % 10;
            insertDigit(partialResult, product);
            ptr2 = ptr2->next;
        }
        if (carry > 0) {
            insertDigit(partialResult, carry);
        }


        for (int i = 0; i < shift; i++) {
            insertDigit(partialResult, 0);
        }

        result = add(result, partialResult);

        destroyBigInt(partialResult);

        shift++;
        ptr1 = ptr1->next;
    }

    return result;
}

void removeLeadingZeros(BigInt* num) {
    while (num->head != NULL && num->head->data == 0) {
        BigIntNode* temp = num->head;
        num->head = num->head->next;
        free(temp);
    }
    if (num->head == NULL) {
        insertDigit(num, 0);
    }
}

BigInt* divide(BigInt* num1, BigInt* num2) {

    if (isZero(num2)) {
        fprintf(stderr, "Division by zero is not allowed.\n");
        exit(EXIT_FAILURE);
    }

    BigInt* result = createBigInt();
    BigInt* currentDividend = createBigInt();
    BigInt* tempResult = createBigInt();
    BigInt* tempDivisor = createBigInt();


    result->sign = (num1->sign == num2->sign) ? 0 : 1;

    BigIntNode* ptr1 = num1->head;

    while (ptr1 != NULL) {
        insertDigit(currentDividend, ptr1->data);
        removeLeadingZeros(currentDividend);
        ptr1 = ptr1->next;

        int quotient = 0;
        int found = 0;

        while (compareMagnitude(currentDividend, tempDivisor) >= 0) {
            if (tempDivisor->head == NULL) {

                insertDigit(tempDivisor, 1);
            }

            insertDigit(tempDivisor, 1);
            quotient++;
        }


        quotient--;
        if (tempDivisor->head != NULL) {
            removeLeadingZeros(tempDivisor);
        }
        tempDivisor = subtract(tempDivisor, num2);

        insertDigit(tempResult, quotient);

        currentDividend = subtract(currentDividend, multiply(tempDivisor, createBigIntFromInt(quotient)));
        removeLeadingZeros(currentDividend);
    }
    while (tempResult->head != NULL) {
        insertDigit(result, tempResult->head->data);
        BigIntNode* temp = tempResult->head;
        tempResult->head = tempResult->head->next;
        free(temp);
    }

    destroyBigInt(currentDividend);
    destroyBigInt(tempResult);
    destroyBigInt(tempDivisor);

    return result;
}

BigInt* modulus(BigInt* num1, BigInt* num2) {

    if (isZero(num2)) {
        fprintf(stderr, "Modulus by zero is not allowed.\n");
        exit(EXIT_FAILURE);
    }

    BigInt* currentDividend = createBigInt();
    BigInt* tempResult = createBigInt();
    BigInt* tempDivisor = createBigInt();

    BigIntNode* ptr1 = num1->head;

    while (ptr1 != NULL) {
        insertDigit(currentDividend, ptr1->data);
        removeLeadingZeros(currentDividend);
        ptr1 = ptr1->next;

        int quotient = 0;

        while (compareMagnitude(currentDividend, tempDivisor) >= 0) {
            if (tempDivisor->head == NULL) {
                insertDigit(tempDivisor, 1);
            }

            insertDigit(tempDivisor, 1);
            quotient++;
        }

        BigInt* tempProduct = multiply(num2, createBigIntFromInt(quotient));
        currentDividend = subtract(currentDividend, tempProduct);
        removeLeadingZeros(currentDividend);
        destroyBigInt(tempProduct);

        insertDigit(tempResult, quotient);
    }

    destroyBigInt(tempDivisor);

    BigInt* result = createBigInt();
    while (currentDividend->head != NULL) {
        insertDigit(result, currentDividend->head->data);
        BigIntNode* temp = currentDividend->head;
        currentDividend->head = currentDividend->head->next;
        free(temp);
    }

    destroyBigInt(currentDividend);
    destroyBigInt(tempResult);

    result->sign = num1->sign;

    return result;
}

