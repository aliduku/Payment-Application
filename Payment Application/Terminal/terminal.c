#include "terminal.h"

/*
This function will ask for the transaction data and store it in terminal data.
Transaction date is 10 characters string in the format DD/MM/YYYY, e.g 25/06/2022.
If the transaction date is NULL, less than 10 characters or wrong format will return WRONG_DATE error, else return OK.
*/
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    struct tm newtime;
    time_t now = time(0);
    localtime_s(&newtime, &now);
    uint8_t systemDate[MAX_SYSTEM_DATE_SIZE] = { '\0' };
    sprintf_s(systemDate, MAX_SYSTEM_DATE_SIZE, "%d/%02d/%02d", newtime.tm_mday, newtime.tm_mon + 1, newtime.tm_year + 1900);
    strcpy_s(termData->transactionDate, MAX_SYSTEM_DATE_SIZE, systemDate);
    printf("Current System Date: %s\n", termData->transactionDate);
    return TERMINAL_OK;
}

/*
This function compares the card expiry date with the transaction date.
If the card expiration date is before the transaction date will return EXPIRED_CARD, else return OK.
*/
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    uint32_t cardExpirationMonth = ((cardData.cardExpirationDate[0] - '0') * 10) + (cardData.cardExpirationDate[1] - '0');
    uint32_t transactionMonth = ((termData.transactionDate[3] - '0') * 10) + (termData.transactionDate[4] - '0');
    uint32_t cardExpirationYear = ((cardData.cardExpirationDate[3] - '0') * 10) + ((cardData.cardExpirationDate[4] - '0') + 2000);
    uint32_t transactionYear = ((termData.transactionDate[6] - '0') * 1000) + ((termData.transactionDate[7] - '0') * 100) + ((termData.transactionDate[8] - '0') * 10) + (termData.transactionDate[9] - '0');
    if ((transactionYear > cardExpirationYear) || ((transactionYear == cardExpirationYear) && (transactionMonth > cardExpirationMonth)))
    {
        return EXPIRED_CARD;
    }
    return TERMINAL_OK;
}

/*
This function asks for the transaction amount and saves it into terminal data.
If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else return OK.
*/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    uint8_t enteredTransactionAmountString[10] = { '\0' };
    printf("Please insert Transaction Amount here: ");
    fgets(enteredTransactionAmountString, 10, stdin);
    fseek(stdin, 0, SEEK_END);
    enteredTransactionAmountString[strcspn(enteredTransactionAmountString, "\n")] = '\0';
    uint32_t i = 0;
    while ((enteredTransactionAmountString[i] != '\0') && (isdigit(enteredTransactionAmountString[i])))
        i++;
    if ((enteredTransactionAmountString[0] == '\0') || (enteredTransactionAmountString[i] != '\0') || (strtof(enteredTransactionAmountString, NULL) <= 0))
    {
        printf("Please enter a valid amount.\n");
        return INVALID_AMOUNT;
    }
    termData->transAmount = strtof(enteredTransactionAmountString, NULL);
    return TERMINAL_OK;
}

/*
This function compares the transaction amount with the terminal max amount.
If the transaction amount is larger than the terminal max amount will return EXCEED_MAX_AMOUNT, else return OK.
*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t termData)
{
    if (termData.transAmount > termData.maxTransAmount)
    {
        return EXCEED_MAX_AMOUNT;
    }
    return TERMINAL_OK;
}

/*
This function checks if the PAN is a Luhn number or not.
If PAN is not a Luhn number will return INVALID_CARD, else return OK.
*/
EN_terminalError_t isValidCardPAN(ST_cardData_t cardData)
{
    uint32_t sum = 0;
    uint32_t second = 0;
    uint32_t numberOfDigits = (uint32_t)(strlen (cardData.primaryAccountNumber));
    for (int32_t i = numberOfDigits - 1; i >= 0; i--)
    {
        uint32_t digit = cardData.primaryAccountNumber[i] - '0';
        if (second)
            digit *= 2;
        sum += digit / 10;
        sum += digit % 10;
        second = !second;
    }
    if (sum % 10 != 0)
        return INVALID_CARD;

    return TERMINAL_OK;
}

/*
This function sets the maximum allowed amount into terminal data.
Transaction max amount is a float number.
If transaction max amount less than or equal to 0 will return INVALID_MAX_AMOUNT error, else return OK.
*/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
    uint8_t enteredMaxTransactionAmountString[10] = { '\0' };
    printf("Please insert Max Transaction Amount here: ");
    fseek(stdin, 0, SEEK_END);
    fgets(enteredMaxTransactionAmountString, 10, stdin);
    fseek(stdin, 0, SEEK_END);
    enteredMaxTransactionAmountString[strcspn(enteredMaxTransactionAmountString, "\n")] = '\0';
    uint32_t i = 0;
    while ((enteredMaxTransactionAmountString[i] != '\0') && (isdigit(enteredMaxTransactionAmountString[i])))
        i++;
    if ((enteredMaxTransactionAmountString[i] != '\0') || (strtof(enteredMaxTransactionAmountString, NULL) <= 0))
    {
        printf("Please enter a valid amount.\n");
        return INVALID_MAX_AMOUNT;
    }
    termData->maxTransAmount = strtof(enteredMaxTransactionAmountString, NULL);
    return TERMINAL_OK;
}
