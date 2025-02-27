#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store account details
typedef struct {
    int accountNumber;
    char name[50];
    float balance;
} Account;

// Function prototypes
void createAccount();
void displayAccounts();
void calculateInterest();
void fundTransfer();
void contactSupport();

// File to store account data
const char *fileName = "accounts.dat.txt";

int main() {
    int choice;

    do {
        printf("\n--- Banking System ---\n");
        printf("1. Create Account\n");
        printf("2. Display Accounts\n");
        printf("3. Calculate Interest\n");
        printf("4. Fund Transfer\n");
        printf("5. Contact Support\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                displayAccounts();
                break;
            case 3:
                calculateInterest();
                break;
            case 4:
                fundTransfer();
                break;
            case 5:
                contactSupport();
                break;
            case 6:
                printf("Exiting... Thank you for using the system.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

void createAccount() {
    FILE *file = fopen(fileName, "a"); // Open in append mode for a text file

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Account account;
    printf("Enter Account Number: ");
    scanf("%d", &account.accountNumber);
    printf("Enter Name: ");
    scanf("%s", account.name);
    printf("Enter Initial Balance: ");
    scanf("%f", &account.balance);

    // Use fprintf to write the account data to the text file
    fprintf(file, "Account Number: %d\n", account.accountNumber);
    fprintf(file, "Name: %s\n", account.name);
    fprintf(file, "Balance: %.2f\n", account.balance);
    fprintf(file, "-------------------------\n");

    fclose(file);

    printf("Account created successfully!\n");
}


void displayAccounts() {
    FILE *file = fopen(fileName, "r"); // Open in read mode for a text file
    if (file == NULL) {
        printf("No accounts found!\n");
        return;
    }

    Account account;
    printf("\n--- Account Details ---\n");

    // Read account details in plain text format
    while (fscanf(file, "Account Number: %d\n", &account.accountNumber) == 1) {
        fscanf(file, "Name: %s\n", account.name);              // Name
        fscanf(file, "Balance: %f\n", &account.balance);       // Balance
        fscanf(file, "-------------------------\n");          // Skip the separator line

        // Print account details
        printf("Account Number: %d\n", account.accountNumber);
        printf("Name: %s\n", account.name);
        printf("Balance: %.2f\n", account.balance);
        printf("----------------------\n");
    }

    fclose(file);
}





void calculateInterest() {
    int accNum;
    int months;

    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    printf("Enter Number of Months: ");
    scanf("%d", &months);

    FILE *file = fopen(fileName, "r"); // Open the file in read mode
    if (file == NULL) {
        printf("No accounts found!\n");
        return;
    }

    Account account;
    int found = 0;
    while (fscanf(file, "Account Number: %d\n", &account.accountNumber) == 1) {
        fscanf(file, "Name: %s\n", account.name);
        fscanf(file, "Balance: %f\n", &account.balance);
        fscanf(file, "-------------------------\n");

        // Check if the current account matches the one entered by the user
        if (account.accountNumber == accNum) {
            found = 1;
            // Fixed 3% interest per month
            float interest = account.balance * 3 * months / 100;  // 3% per month
            printf("Interest for account %d for %d months at 3%% interest rate: %.2f\n", 
                   account.accountNumber, months, interest);
            break;
        }
    }

    if (!found) {
        printf("Account not found!\n");
    }

    fclose(file);
}


void fundTransfer() {
    int fromAcc, toAcc;
    float amount;

    printf("Enter Sender's Account Number: ");
    scanf("%d", &fromAcc);
    printf("Enter Receiver's Account Number: ");
    scanf("%d", &toAcc);
    printf("Enter Amount to Transfer: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid transfer amount. Please try again.\n");
        return;
    }

    FILE *file = fopen(fileName, "r+"); // Open in read/write mode for text file
    if (file == NULL) {
        printf("No accounts found!\n");
        return;
    }

    // To store all accounts and modify balances
    Account accounts[100];
    int accountCount = 0;
    int foundFrom = 0, foundTo = 0;
    
    // Read all accounts into memory
    while (fscanf(file, "Account Number: %d\n", &accounts[accountCount].accountNumber) == 1) {
        fscanf(file, "Name: %s\n", accounts[accountCount].name);
        fscanf(file, "Balance: %f\n", &accounts[accountCount].balance);
        fscanf(file, "-------------------------\n");

        // Debugging: Print the account information as we read
        printf("Read Account - Number: %d, Name: %s, Balance: %.2f\n", accounts[accountCount].accountNumber, accounts[accountCount].name, accounts[accountCount].balance);

        if (accounts[accountCount].accountNumber == fromAcc) {
            foundFrom = 1;
        }
        if (accounts[accountCount].accountNumber == toAcc) {
            foundTo = 1;
        }

        accountCount++;
    }

    // Debugging: Check if we found the accounts in memory
    if (foundFrom) {
        printf("Sender Account Found: %d\n", fromAcc);
    } else {
        printf("Sender Account Not Found: %d\n", fromAcc);
    }

    if (foundTo) {
        printf("Receiver Account Found: %d\n", toAcc);
    } else {
        printf("Receiver Account Not Found: %d\n", toAcc);
    }

    if (!foundFrom || !foundTo) {
        printf("One or both accounts not found!\n");
        fclose(file);
        return;
    }

    // Perform the transfer if both accounts were found
    if (foundFrom && foundTo) {
        // Find the sender and receiver in the accounts array
        Account *fromAccount = NULL, *toAccount = NULL;
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].accountNumber == fromAcc) {
                fromAccount = &accounts[i];
            }
            if (accounts[i].accountNumber == toAcc) {
                toAccount = &accounts[i];
            }
        }

        // Check if there's enough balance in the sender's account
        if (fromAccount->balance >= amount) {
            fromAccount->balance -= amount;
            toAccount->balance += amount;

            // Rewind file pointer to the beginning and write updated accounts back to the file
            freopen(fileName, "w", file);  // Open the file in write mode to overwrite content

            for (int i = 0; i < accountCount; i++) {
                fprintf(file, "Account Number: %d\n", accounts[i].accountNumber);
                fprintf(file, "Name: %s\n", accounts[i].name);
                fprintf(file, "Balance: %.2f\n", accounts[i].balance);
                fprintf(file, "-------------------------\n");
            }

            printf("Funds transferred successfully!\n");
        } else {
            printf("Insufficient balance in sender's account!\n");
        }
    }

    fclose(file);
}



// Function to contact support
void contactSupport() {
    printf("\n--- Contact Support ---\n");
    printf("Email: support@bankingsystem.com\n");
    printf("Phone: +91 69420 69420\n");
}