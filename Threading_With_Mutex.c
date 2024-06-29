#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

pthread_mutex_t lock;

#define ARRAY_SIZE 20

typedef struct Account {
	char accountName[4];
	int accountBalance;
} Account;

Account accounts[ARRAY_SIZE];

typedef struct ThreadInfo {
	int num;
	char *selectedAccount;
	char* transaction;
	int amt;
} ThreadInfo;


void* deposit(void* arg) {
	pthread_mutex_lock(&lock);
	ThreadInfo* myData;
	int numOfAccounts, amount;
	char* tempaccname;
	char* trans;

	myData = (ThreadInfo*)arg;
	numOfAccounts = myData->num;
	amount = myData->amt;
	tempaccname = myData->selectedAccount;
	trans = myData->transaction;

	if (strcmp(trans, "deposit") == 0) {
		for (int i = 0; i < numOfAccounts; i++) {
			if (strcmp(tempaccname, accounts[i].accountName) == 0) {
				accounts[i].accountBalance = accounts[i].accountBalance + amount;
			}
		}
	}
	if (strcmp(trans, "withdraw") == 0) {
		for (int i = 0; i < numOfAccounts; i++) {
			if (strcmp(tempaccname, accounts[i].accountName) == 0) {
				if (amount > accounts[i].accountBalance) {
					//ignore
				}
				else {
					accounts[i].accountBalance = accounts[i].accountBalance - amount;
				}
			}
		}
	}
	pthread_mutex_unlock(&lock);
}

/*void* withdraw(void* arg) {
	pthread_mutex_lock(&lock);
	ThreadInfo* myData;
	int numOfAccounts, amount;
	char* tempaccname;

	myData = (ThreadInfo*)arg;
	numOfAccounts = myData->num;
	amount = myData->amt;
	tempaccname = myData->selectedAccount;

	
	for (int i = 0; i < numOfAccounts; i++) {
		if (strcmp(tempaccname, accounts[i].accountName) == 0) {
			if (amount > accounts[i].accountBalance) {
				//ignore
			}
			else {
				accounts[i].accountBalance = accounts[i].accountBalance - amount;
			}
		}
	}
	pthread_mutex_unlock(&lock);
}*/


int main(void) {
	FILE* file;
	char line[100];
	file = fopen("assignment_5_input.txt", "r");
	int numOfAccounts = 0;
	if (pthread_mutex_init(&lock, NULL) != 0) {
		printf("mutex init failed\n");
		return 0;
	}

	while (fgets(line, sizeof(line), file) != NULL) {
		int length = strlen(line);
		if (line[0] == 'A') {
			numOfAccounts++;
		}
	}
	fclose(file);

	for (int i = 0; i < numOfAccounts; i++) { //initialising Account Names
		int num = i + 1;
		char name[3];
		sprintf(name, "%d", num);
		name[2] = '\0';
		accounts[i].accountName[0] = 'A';
		accounts[i].accountName[1] = '\0';
		strcat(accounts[i].accountName, name);
	}

	file = fopen("assignment_5_input.txt", "r");
	while (fgets(line, sizeof(line), file) != NULL) { //initialising Account Balances
		int length = strlen(line);
		if (line[0] == 'A') {
			char* b;
			int index;
			b = strchr(line, 'b');
			index = (int)(b - line);
			int balanceIndex = index + 8;
			char balanceChar[10];
			int j = 0;
			for (int i = balanceIndex; i < length + 1; i++) {
				balanceChar[j] = line[i];
				j++;
			}
			balanceChar[strlen(balanceChar)] = '0';
			int balance = atoi(balanceChar);

			char tempAccountName[4];
			for (int i = 0; i < (index - 1); i++) {
				tempAccountName[i] = line[i];
			}
			tempAccountName[3] = '\0';

			for (int i = 0; i < numOfAccounts; i++) {
				if (strcmp(accounts[i].accountName, tempAccountName) == 0) {
					accounts[i].accountBalance = balance;
				}
			}
		}
	}
	fclose(file);
	//Accounts have been initialised from here on out

	int numOfClients = 0;
	file = fopen("assignment_5_input.txt", "r");
	while (fgets(line, sizeof(line), file) != NULL) { //creating thread Array for clients
		if (line[0] == 'C') {
			numOfClients++;
		}
	}
	fclose(file);

	printf("No. of Accounts: %d\n", numOfAccounts);
	printf("No. of Clients: %d\n", numOfClients);

	pthread_t th[numOfClients]; //thread array
	int currClient = 0;

	file = fopen("assignment_5_input.txt", "r");
	while (fgets(line, sizeof(line), file) != NULL) { //dealing with transactions
		if (line[0] == 'C') {
			for (int i = 0; i < strlen(line); i++) {
				if (line[i] == 'd') {
					if (line[i + 6] == 't') {
						int accountIndex = i + 8;
						char tempAccountName[4];
						int j = 0;
						while (line[accountIndex] != ' ') {
							tempAccountName[j] = line[accountIndex];
							j++;
							accountIndex++;
						}
						tempAccountName[strlen(tempAccountName)] = '\0';

						int amountIndex = accountIndex + 1;
						char depositAmountChar[10];
						j = 0;
						while (line[amountIndex] != ' ' && line[amountIndex] != '\0') {
							depositAmountChar[j] = line[amountIndex];
							depositAmountChar[j + 1] = '\0';
							j++;
							amountIndex++;
						}

						int depositAmount = atoi(depositAmountChar);


						ThreadInfo depositInfo;
						depositInfo.num = numOfAccounts;
						depositInfo.selectedAccount = tempAccountName;
						depositInfo.amt = depositAmount;
						depositInfo.transaction = "deposit";
						pthread_create(&th[currClient], NULL, &deposit, &depositInfo);
						pthread_join(th[currClient], NULL);
					}
				}

				if (line[i] == 'w') {
					if (line[i + 6] == 'a') {
						int accountIndex = i + 9;
						char tempAccountName[4];
						int j = 0;
						while (line[accountIndex] != ' ') {
							tempAccountName[j] = line[accountIndex];
							j++;
							accountIndex++;
						}
						tempAccountName[strlen(tempAccountName)] = '\0';

						int amountIndex = accountIndex + 1;
						char withdrawAmountChar[10];
						j = 0;
						while (line[amountIndex] != ' ' && line[amountIndex] != '\0') {
							withdrawAmountChar[j] = line[amountIndex];
							withdrawAmountChar[j + 1] = '\0';
							j++;
							amountIndex++;
						}

						int withdrawAmount = atoi(withdrawAmountChar);

						/*ThreadInfo withdrawInfo;
						withdrawInfo.num = numOfAccounts;
						withdrawInfo.selectedAccount = tempAccountName;
						withdrawInfo.amt = withdrawAmount;
						withdrawInfo.transaction = "withdraw";
						pthread_create(&th[currClient], NULL, &deposit, &withdrawInfo);
						pthread_join(th[currClient], NULL);*/
					}
				}
			}
		}
		currClient++;
	}
	fclose(file);

	for (int a = 0; a < numOfAccounts; a++) {
		printf("%s balance %d\n", accounts[a].accountName, accounts[a].accountBalance);
	}

	pthread_mutex_destroy(&lock);
}