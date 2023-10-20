#ifndef BIGINT_H
#define BIGINT_H

typedef struct BigIntNode {
    int data;
    struct BigIntNode* next;
} BigIntNode;

typedef struct {
    BigIntNode* head;
    int sign;  
} BigInt;


BigInt* createBigInt();
void destroyBigInt(BigInt* num);
void insertDigit(BigInt* num, int digit);
void printBigInt(BigInt* num);
BigInt* add(BigInt* num1, BigInt* num2);
BigInt* subtract(BigInt* num1, BigInt* num2);
BigInt* multiply(BigInt* num1, BigInt* num2);
BigInt* divide(BigInt* num1, BigInt* num2);
BigInt* modulus(BigInt* num1, BigInt* num2);  
int compareMagnitude(BigInt* num1, BigInt* num2);
int isZero(BigInt* num);
BigInt* createBigIntFromInt(int value);
void freeNode(BigIntNode* node);
void freeBigInt(BigInt* num);

#endif  