#ifndef SERVER_H
#define SERVER_H

#include <windows.h>

#include "../Terminal/terminal.h"

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR, DECLINED_EXPIRED_CARD, DECLINED_EXCEED_MAX_AMOUNT, DECLINED_INVALID_CARD
}EN_transState_t;

typedef enum EN_serverError_t
{
	SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

uint32_t foundPANIndex;
ST_accountsDB_t accountsDatabase[255];
ST_transaction_t transactionsDatabase[255];

EN_transState_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t retrieveAccountsDatabase(void);
EN_serverError_t updateAccountsDatabase(void);
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction_t* transData);

#endif
