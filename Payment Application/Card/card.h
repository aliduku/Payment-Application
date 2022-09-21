#ifndef CARD_H
#define CARD_H

#define MAX_CARD_HOLDER_NAME 25
#define MAX_DATE_SIZE 6
#define MAX_PAN_SIZE 20

#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;

typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
	CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);

#endif
