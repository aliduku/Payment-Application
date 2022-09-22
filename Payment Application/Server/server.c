#include "server.h"

/*
This function will fill in the global array of ST_accountsDB_t with the valid accounts database
This array has a maximum of 255 element/account data
*/
EN_serverError_t retrieveAccountsDatabase(void)
{
	FILE* accountsDatabaseFile;
	errno_t accountsDatabaseFileErrorCode;
	accountsDatabaseFileErrorCode = fopen_s(&accountsDatabaseFile, "./Server/Accounts_DB.txt", "r");
	if (accountsDatabaseFileErrorCode)
		return INTERNAL_SERVER_ERROR;

	uint8_t oneChar;
	uint32_t i = 0, linesCount = 0;

	while (!feof(accountsDatabaseFile))
	{
		oneChar = fgetc(accountsDatabaseFile);
		if (oneChar == '\n')
			linesCount++;
	}

	fseek(accountsDatabaseFile, 0L, SEEK_SET);
	while (fscanf_s(accountsDatabaseFile, "%f %s", &(accountsDatabase[i].balance), accountsDatabase[i].primaryAccountNumber, MAX_PAN_SIZE) && i <= linesCount)
		i++;

	fclose(accountsDatabaseFile);
	return SERVER_OK;
}

/*
This function will update in the global array of ST_accountsDB_t with the valid accounts database
This array has a maximum of 255 element/account data
*/
EN_serverError_t updateAccountsDatabase(void)
{
	FILE* accountsDatabaseFile;
	errno_t accountsDatabaseFileErrorCode;
	accountsDatabaseFileErrorCode = fopen_s(&accountsDatabaseFile, "./Server/Accounts_DB.txt", "w");
	if (accountsDatabaseFileErrorCode)
		return SAVING_FAILED;

	for (uint32_t i = 0; i < 255; i++) 
	{
		fprintf(accountsDatabaseFile, "%d\t\t%s", (uint32_t)accountsDatabase[i].balance, accountsDatabase[i].primaryAccountNumber);
		if (accountsDatabase[i+1].primaryAccountNumber[0] == '\0') 
			break;
		fprintf(accountsDatabaseFile, "\n");
	}

	fclose(accountsDatabaseFile);
	return SERVER_OK;
}

/*
This function will take all transaction data and validate its data.
It checks the account details and amount availability.
If the account does not exist return DECLINED_STOLEN_CARD, if the amount is not available will return DECLINED_INSUFFECIENT_FUND, if a transaction can't be saved will return INTERNAL_SERVER_ERROR and will not save the transaction, else returns APPROVED.
It will update the database with the new balance.
*/
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;

	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

	if (transData->transState == DECLINED_INVALID_CARD)
	{
		printf("\nDeclined because Luhn check failed.\n");
		SetConsoleTextAttribute(hConsole, saved_attributes);

		if (saveTransaction(transData) == SAVING_FAILED)
			return INTERNAL_SERVER_ERROR;
		return DECLINED_INVALID_CARD;
	}

	if (transData->transState == DECLINED_EXPIRED_CARD)
	{
		printf("\nDeclined due to expired card.\n");
		SetConsoleTextAttribute(hConsole, saved_attributes);

		if (saveTransaction(transData) == SAVING_FAILED)
			return INTERNAL_SERVER_ERROR;
		return DECLINED_EXPIRED_CARD;
	}

	if (transData->transState == DECLINED_EXCEED_MAX_AMOUNT)
	{
		printf("\nDeclined because maximum amount is exceeded.\n");
		SetConsoleTextAttribute(hConsole, saved_attributes);

		if (saveTransaction(transData) == SAVING_FAILED)
			return INTERNAL_SERVER_ERROR;
		return DECLINED_EXCEED_MAX_AMOUNT;
	}

	if (isValidAccount(&(transData->cardHolderData)) == ACCOUNT_NOT_FOUND) 
	{
		transData->transState = DECLINED_STOLEN_CARD;

		printf("\nDeclined because this is a stolen/invalid card (PAN is not in our Database).\n");
		SetConsoleTextAttribute(hConsole, saved_attributes);

		if (saveTransaction(transData) == SAVING_FAILED) 
			return INTERNAL_SERVER_ERROR;
		return DECLINED_STOLEN_CARD;
	}

	if (isAmountAvailable(&(transData->terminalData)) == LOW_BALANCE) 
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;

		printf("\nDeclined due to insuffecient funds.\n");
		SetConsoleTextAttribute(hConsole, saved_attributes);

		if (saveTransaction(transData) == SAVING_FAILED)
			return INTERNAL_SERVER_ERROR;
		return DECLINED_INSUFFECIENT_FUND;
	}

	transData->transState = APPROVED;
	accountsDatabase[foundPANIndex].balance -= transData->terminalData.transAmount;

	if ((saveTransaction(transData) == SAVING_FAILED) || (updateAccountsDatabase() == SAVING_FAILED))
	{
		printf("\nServer Error!\n");
		return INTERNAL_SERVER_ERROR;
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	printf("\nTransaction is Approved.\n\n");

	SetConsoleTextAttribute(hConsole, saved_attributes);
	printf("Your new balance is %d\n", (uint32_t)(accountsDatabase[foundPANIndex].balance));

	return APPROVED;
}

/*
This function will take card data and validate these data.
It checks if the PAN exists or not in the server's database.
If the PAN doesn't exist will return DECLINED_STOLEN_CARD, else will return OK
*/
EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	for (uint8_t i = 0; i < 255; i++)
	{
		if (accountsDatabase[i].primaryAccountNumber[0] == '\0') 
			break;

		if (!strcmp(accountsDatabase[i].primaryAccountNumber, cardData->primaryAccountNumber))
		{
			foundPANIndex = i;
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}

/*
This function will take terminal data and validate these data.
It checks if the transaction's amount is available or not.
If the transaction amount is greater than the balance in the database will return LOW_BALANCE, else will return OK
*/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (accountsDatabase[foundPANIndex].balance < termData->transAmount)
		return LOW_BALANCE;
	return SERVER_OK;
}

/*
This function will take all transaction data into the transactions database.
It gives a sequence number to a transaction, this number is incremented once a transaction is processed into the server.
It saves any type of transaction, APPROVED or DECLINED, with the specific reason for declining/transaction state.
If transaction can't be saved will return SAVING_FAILED, else will return OK
*/
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	FILE* transactionsCountFile, * transactionsDatabaseFile;
	errno_t transactionsCountFileErrorCode, transactiosDatabaseFileErrorCode;
	transactionsCountFileErrorCode = fopen_s(&transactionsCountFile, "./Server/Transactions_Count.txt", "r+");
	if (transactionsCountFileErrorCode)
		return SAVING_FAILED;

	fscanf_s(transactionsCountFile, "%d", &(transData->transactionSequenceNumber));
	fseek(transactionsCountFile, 0L, SEEK_SET);
	fprintf(transactionsCountFile, "%d", ++(transData->transactionSequenceNumber));
	fclose(transactionsCountFile);

	transactiosDatabaseFileErrorCode = fopen_s(&transactionsDatabaseFile, "./Server/Transactions_DB.txt", "a");
	if (transactiosDatabaseFileErrorCode)
		return SAVING_FAILED;

	fprintf(transactionsDatabaseFile, "\nTransaction Sequence Number: %d\n", transData->transactionSequenceNumber);
	fprintf(transactionsDatabaseFile, "* Card Holder Name: %s\n", transData->cardHolderData.cardHolderName);
	fprintf(transactionsDatabaseFile, "* Primary Account Number: %s\n", transData->cardHolderData.primaryAccountNumber);
	fprintf(transactionsDatabaseFile, "* Card Expiration Date: %s\n", transData->cardHolderData.cardExpirationDate);
	fprintf(transactionsDatabaseFile, "* Transaction Amount: %d\n", (uint32_t)transData->terminalData.transAmount);
	fprintf(transactionsDatabaseFile, "* Max Transaction Amount: %d\n", (uint32_t)transData->terminalData.maxTransAmount);
	fprintf(transactionsDatabaseFile, "* Transaction Date: %s\n", transData->terminalData.transactionDate);

	switch (transData->transState)
	{
	case APPROVED:
		fprintf(transactionsDatabaseFile, "* Transaction State: Approved\n");
		break;

	case DECLINED_INSUFFECIENT_FUND:
		fprintf(transactionsDatabaseFile, "* Transaction State: Declined due to insuffecient funds\n");
		break;

	case DECLINED_STOLEN_CARD:
		fprintf(transactionsDatabaseFile, "* Transaction State: Declined because this is a stolen/invalid card (PAN is not in our Database).\n");
		break;

	case DECLINED_EXPIRED_CARD:
		fprintf(transactionsDatabaseFile, "* Transaction State: Declined due to expired card.\n");
		break;

	case DECLINED_EXCEED_MAX_AMOUNT:
		fprintf(transactionsDatabaseFile, "* Transaction State: Declined because maximum amount is exceeded.\n");
		break;

	case DECLINED_INVALID_CARD:
		fprintf(transactionsDatabaseFile, "* Transaction State: Declined because Luhn check failed.\n");
		break;
	}

	fclose(transactionsDatabaseFile);
	return SERVER_OK;
}
