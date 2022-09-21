#include "card.h"

/*
This function will ask for the cardholder's name and store it into card data.
Card holder name is 24 characters string max and 20 min.
If the cardholder name is NULL, less than 20 characters or more than 24 will return WRONG_NAME error, else return OK.
*/
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	uint8_t enteredName[MAX_CARD_HOLDER_NAME] = { '\0' };
	printf("Please insert card holder name here. [20 - 24 alphabet characters]: ");
	fseek(stdin, 0, SEEK_END);
	fgets(enteredName, MAX_CARD_HOLDER_NAME, stdin);
	fseek(stdin, 0, SEEK_END);
	enteredName[strcspn(enteredName, "\n")] = '\0';
	uint32_t i = 0;
	while ((enteredName[i] != '\0') && (isalpha(enteredName[i]) || (isblank(enteredName[i]))))
		i++;
	if ((enteredName[0] == '\0') || (enteredName[19] == '\0') || (enteredName[i] != '\0'))
	{
		printf("Name format incorrect, please try again.\n");
		return WRONG_NAME;
	}
	strcpy_s(cardData->cardHolderName, MAX_CARD_HOLDER_NAME, enteredName);
	printf("Entered Name: %s\n", cardData->cardHolderName);
	return CARD_OK;
}

/*
This function will ask for the card expiry date and store it in card data.
Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25".
If the card expiry date is NULL, less or more than 5 characters, or has the wrong format will return WRONG_EXP_DATE error, else return OK.
*/
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	uint32_t month, year;
	uint8_t enteredExpirationDate[MAX_DATE_SIZE] = { '\0' };
	printf("Please insert card expiration date here. [MM/YY]: ");
	fgets(enteredExpirationDate, MAX_DATE_SIZE, stdin);
	fseek(stdin, 0, SEEK_END);
	enteredExpirationDate[strcspn(enteredExpirationDate, "\n")] = '\0';
	month = ((enteredExpirationDate[0] - '0') * 10) + (enteredExpirationDate[1] - '0');
	year = ((enteredExpirationDate[3] - '0') * 10) + (enteredExpirationDate[4] - '0');
	if ((enteredExpirationDate[0] == '\0') || (enteredExpirationDate[4] == '\0') || (enteredExpirationDate[2] != '/') || (month < 1) || (month > 12) || (year < 1) || (year > 99))
	{
		printf("Date format incorrect, please try again.\n");
		return WRONG_EXP_DATE;
	}
	strcpy_s(cardData->cardExpirationDate, MAX_DATE_SIZE, enteredExpirationDate);
	printf("Entered Expiration Date: %s\n", cardData->cardExpirationDate);
	return CARD_OK;
}

/*
This function will ask for the card's Primary Account Number and store it in card data.
PAN is 20 characters alphanumeric only string 19 character max, and 16 character min.
If the PAN is NULL, less than 16 or more than 19 characters, will return WRONG_PAN error, else return OK.
*/
getCardPAN(ST_cardData_t* cardData)
{
	uint8_t enteredPAN[MAX_PAN_SIZE] = { '\0' };
	printf("Please insert your PAN here. [16 - 19 characters]: ");
	fgets(enteredPAN, MAX_PAN_SIZE, stdin);
	fseek(stdin, 0, SEEK_END);
	enteredPAN[strcspn(enteredPAN, "\n")] = '\0';
	if ((enteredPAN[0] == '\0') || (enteredPAN[15] == '\0'))
	{
		printf("PAN format incorrect, please try again.\n");
		return WRONG_PAN;
	}
	strcpy_s(cardData->primaryAccountNumber, MAX_PAN_SIZE, enteredPAN);
	printf("Entered PAN: %s\n", cardData->primaryAccountNumber);
	return CARD_OK;
}