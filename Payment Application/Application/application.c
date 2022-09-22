#include "application.h"

/*
This function will start the application.
*/
void appStart(void)
{
	ST_cardData_t cardData;
	EN_cardError_t cardErrorFlag;
	ST_terminalData_t termData;
	EN_terminalError_t terminalErrorFlag;
	ST_transaction_t transactionData;

	/******************************Card Module******************************/

	cardErrorFlag = getCardHolderName(&cardData);
	while (cardErrorFlag == WRONG_NAME)
		cardErrorFlag = getCardHolderName(&cardData);

	cardErrorFlag = getCardPAN(&cardData);
	while (cardErrorFlag == WRONG_PAN)
		cardErrorFlag = getCardPAN(&cardData);

	cardErrorFlag = getCardExpiryDate(&cardData);
	while (cardErrorFlag == WRONG_EXP_DATE)
		cardErrorFlag = getCardExpiryDate(&cardData);

	transactionData.cardHolderData = cardData;

	/****************************Terminal Module****************************/

	terminalErrorFlag = getTransactionAmount(&termData);
	while (terminalErrorFlag == INVALID_AMOUNT)
		terminalErrorFlag = getTransactionAmount(&termData);

	terminalErrorFlag = setMaxAmount(&termData);
	while (terminalErrorFlag == INVALID_MAX_AMOUNT)
		terminalErrorFlag = setMaxAmount(&termData);
	
	getTransactionDate(&termData);

	transactionData.terminalData = termData;

	/*****************************Server Module*****************************/

	terminalErrorFlag = isValidCardPAN(cardData);
	if (terminalErrorFlag == INVALID_CARD)
	{
		transactionData.transState = DECLINED_INVALID_CARD;
		transactionData.transState = recieveTransactionData(&transactionData);
		return;
	}

	terminalErrorFlag = isCardExpired(cardData, termData);
	if (terminalErrorFlag == EXPIRED_CARD)
	{
		transactionData.transState = DECLINED_EXPIRED_CARD;
		transactionData.transState = recieveTransactionData(&transactionData);
		return;
	}

	terminalErrorFlag = isBelowMaxAmount(termData);
	if (terminalErrorFlag == EXCEED_MAX_AMOUNT)
	{
		transactionData.transState = DECLINED_EXCEED_MAX_AMOUNT;
		transactionData.transState = recieveTransactionData(&transactionData);
		return;
	}

	retrieveAccountsDatabase();
	transactionData.transState = recieveTransactionData(&transactionData);
}

/*
This is the main function which runs the application forever.
*/
void main()
{
	while (1)
	{
		printf("\nStarted Payment Application\n\n");
		appStart();
		printf("\nEnded Payment Application\n");
	}
}