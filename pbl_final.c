#include <stdio.h>
#include <stdlib.h>

int transactions[100][3];
int transactionCount = 0;
int peopleCount = 0;

struct Node {
    int person;
    int amount;
    struct Node* next;
};

struct Node* pq = NULL;

void insert(int person, int amount) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->person = person;
    newNode->amount = amount;
    newNode->next = NULL;

    if (pq == NULL || pq->amount > amount) {
        newNode->next = pq;
        pq = newNode;
        return;
    }

    struct Node* temp = pq;
    while (temp->next != NULL && temp->next->amount < amount) {
        temp = temp->next;
    }

    newNode->next = temp->next;
    temp->next = newNode;
}

void deleteNode(int person) {
    if (pq == NULL) return;
    if (pq->person == person) {
        struct Node* temp = pq;
        pq = pq->next;
        free(temp);
        return;
    }

    struct Node* temp = pq;
    while (temp->next != NULL && temp->next->person != person) {
        temp = temp->next;
    }

    if (temp->next != NULL) {
        struct Node* temp2 = temp->next;
        temp->next = temp2->next;
        free(temp2);
    }
}

void getMinMax(struct Node** min, struct Node** max) {
    if (pq == NULL) return;
    *min = pq;
    struct Node* temp = pq;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    *max = temp;
}

void minimizeCashFlow() {
    if (transactionCount == 0) {
        printf("\nNo transactions available for optimization.\n");
        return;
    }

    int netAmount[peopleCount]; 
    for (int i = 0; i < peopleCount; i++) {
        netAmount[i] = 0;
    }

    for (int i = 0; i < transactionCount; i++) {
        netAmount[transactions[i][0]] -= transactions[i][2];
        netAmount[transactions[i][1]] += transactions[i][2];
    }

    pq = NULL;

    for (int i = 0; i < peopleCount; i++) {
        if (netAmount[i] != 0) {
            insert(i, netAmount[i]);
        }
    }

    printf("\nOptimized Transactions to Minimize Cash Flow:\n");

    while (pq) {
        struct Node *minNode, *maxNode;
        getMinMax(&minNode, &maxNode);

        int minPerson = minNode->person;
        int maxPerson = maxNode->person;
        int minAmount = abs(minNode->amount);
        int maxAmount = maxNode->amount;

        int settledAmount;
        if (minAmount < maxAmount) {
            settledAmount = minAmount;
        } else {
            settledAmount = maxAmount;
        }

        printf("Person %d pays Person %d amount %d\n", minPerson, maxPerson, settledAmount);

        deleteNode(minPerson);
        deleteNode(maxPerson);

        if (minAmount < maxAmount) {
            insert(maxPerson, maxAmount - settledAmount);
        } else if (minAmount > maxAmount) {
            insert(minPerson, -(minAmount - settledAmount));
        }
    }
}

void displayTransactions() {
    if (transactionCount == 0) {
        printf("\nNo transactions recorded yet.\n");
        return;
    }

    printf("\nRecorded Transactions:\n");
    for (int i = 0; i < transactionCount; i++) {
        printf("Person %d pays Person %d amount %d\n", transactions[i][0], transactions[i][1], transactions[i][2]);
    }
}

void addTransaction() {
    if (transactionCount >= 100) {
        printf("\nTransaction limit reached! Cannot add more.\n");
        return;
    }

    int debtor, creditor, amount;
    printf("Enter debtor ID: ");
    scanf("%d", &debtor);
    printf("Enter creditor ID: ");
    scanf("%d", &creditor);
    printf("Enter amount: ");
    scanf("%d", &amount);

    transactions[transactionCount][0] = debtor;
    transactions[transactionCount][1] = creditor;
    transactions[transactionCount][2] = amount;
    transactionCount++;

    if (debtor >= peopleCount) peopleCount = debtor + 1;
    if (creditor >= peopleCount) peopleCount = creditor + 1;

    printf("Transaction recorded successfully!\n");
}

int main() {
    int choice;

    do {
        printf("\n==== Cash Flow Minimizer Menu ====\n");
        printf("1. Add Transaction\n");
        printf("2. Display Transactions\n");
        printf("3. Minimize Cash Flow\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTransaction();
                break;
            case 2:
                displayTransactions();
                break;
            case 3:
                minimizeCashFlow();
                break;
            case 4:
                printf("\nExiting program. Thank you!\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}
