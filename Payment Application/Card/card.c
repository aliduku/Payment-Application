#include "card.h"

/*
This function will ask for the cardholder's name and store it into card data.
Card holder name is 24 characters string max and 20 min.
If the cardholder name is NULL, less than 20 characters or more than 24 will return WRONG_NAME error, else return OK.
*/
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	uint32_t i = 0;
	uint8_t enteredName[MAX_CARD_HOLDER_NAME] = { '\0' };

	printf("Please insert card holder name here. [20 - 24 alphabet characters]: ");

	fseek(stdin, 0, SEEK_END);
	fgets(enteredName, MAX_CARD_HOLDER_NAME, stdin);

	enteredName[strcspn(enteredName, "\n")] = '\0';
	
	while ((enteredName[i] != '\0') && (isalpha(enteredName[i]) || (isblank(enteredName[i]))))
		i++;

	if ((enteredName[0] == '\0') || (enteredName[19] == '\0') || (enteredName[24] != '\0') || (enteredName[i] != '\0'))
	{
		printf("Name format incorrect, please try again.\n\n");
		return WRONG_NAME;
	}

	strcpy_s(cardData->cardHolderName, MAX_CARD_HOLDER_NAME - 1, enteredName);
	printf("Entered Name: %s\n\n", cardData->cardHolderName);
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

	printf("Please insert card expiration date here. [MM/YY]: ");

	fseek(stdin, 0, SEEK_END);
	fgets(cardData->cardExpirationDate, MAX_DATE_SIZE, stdin);

	cardData->cardExpirationDate[strcspn(cardData->cardExpirationDate, "\n")] = '\0';

	month = ((cardData->cardExpirationDate[0] - '0') * 10) + (cardData->cardExpirationDate[1] - '0');
	year = ((cardData->cardExpirationDate[3] - '0') * 10) + (cardData->cardExpirationDate[4] - '0');

	if ((cardData->cardExpirationDate[0] == '\0') || (cardData->cardExpirationDate[4] == '\0') || (cardData->cardExpirationDate[2] != '/') || (month < 1) || (month > 12) || (year < 1) || (year > 99))
	{
		printf("Date format incorrect, please try again.\n\n");
		return WRONG_EXP_DATE;
	}

	printf("Entered Card Expiry Date: %s\n\n", cardData->cardExpirationDate);
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

	fseek(stdin, 0, SEEK_END);
	fgets(enteredPAN, MAX_PAN_SIZE, stdin);

	enteredPAN[strcspn(enteredPAN, "\n")] = '\0';

	if ((enteredPAN[0] == '\0') || (enteredPAN[19] != '\0') || (enteredPAN[15] == '\0'))
	{
		printf("PAN format incorrect, please try again.\n\n");
		return WRONG_PAN;
	}

	strcpy_s(cardData->primaryAccountNumber, MAX_PAN_SIZE - 1, enteredPAN);
	printf("Entered PAN: %s\n\n", cardData->primaryAccountNumber);
	return CARD_OK;
}
