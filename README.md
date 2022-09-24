# Payment Application

Payment systems are now available everywhere and everyone interacts with these systems every day.

There are different types of transactions you can make, SALE, REFUND, Pre-Authorization, and VOID.

SALE: means to buy something and its price will be deducted from your bank account.
REFUND: this means that you will return something and wants your money back to your bank account.
Pre-Authorization: means holding an amount of money from your account, e.g Hotel reservation.
VOID: this means canceling the transaction, e.g if the seller entered the wrong amount.


This project implements the SALE transaction only by simulating the card, terminal(ATM), and the server.

## Application Flowchart

![image](https://user-images.githubusercontent.com/102156303/192086083-9c5f72f4-6c7e-4a5a-9da7-1e0083fb75d0.png)

## Modules

### Card Module Functions

getCardHolderName(): This function will ask for the cardholder's name and store it into card data.
getCardExpiryDate(): This function will ask for the card expiry date and store it in card data.
getCardPAN(): This function will ask for the card's Primary Account Number and store it in card data.

### Terminal Module Functions

getTransactionDate(): This function will get the system's current date and store it into terminal data.
isCardExpired(): This function compares the card expiry date with the transaction date.
getTransactionAmount(): This function asks for the transaction amount and saves it into terminal data.
setMaxAmount(): This function sets the maximum allowed amount into terminal data.
isBelowMaxAmount(): This function compares the transaction amount with the terminal max amount.
isValidCardPAN(): This function checks if the PAN is a Luhn number or not.

### Server Module Functions

retrieveAccountsDatabase(): This function will fill in the global array of ST_accountsDB_t with the valid accounts database.
updateAccountsDatabase(): This function will update in the global array of ST_accountsDB_t with the valid accounts database.
recieveTransactionData(): This function will take all transaction data and validate its data.
isValidAccount(): This function will take card data and validate the PAN.
isAmountAvailable(): This function will take terminal data and validate the amount.
saveTransaction(): This function will save all transaction data into the transactions database.

### Application Module Functions

appStart(): This function will start the application.
main(): This is the main function which runs the application forever.
