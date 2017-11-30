/*
Jacob Reed
TCSS 333 Summer 2017
HW4 */
#include <stdio.h>
#include <string.h>

#define MAXCUSTOMERS 20
#define MAXITEMS 10
#define MAXNAMELEN 29

typedef struct {
    char itemName[MAXNAMELEN];
    int quantity;
    double price;
} Item;

typedef struct {
    char name[MAXNAMELEN];
    Item items[MAXITEMS];
	  int itemCount;
    double totalVal;
} Customer;

//Checks if customer already exists in struct array and returns 1 if true.
int custCheck(char* custName, Customer customers[]) {
    int i, result = 0;
	  for(i =  0; i < MAXCUSTOMERS; i++) {
		if(strcmp(custName, customers[i].name) == 0) {
			result = 1;
		} 
	}
	return result;
}

//returns index of customer name, -1 if not found.
int custIndex(char* custName, Customer customers[]) {
    int i, result = -1;
	  for(i =  0; i < MAXCUSTOMERS; i++) {
		if(strcmp(custName, customers[i].name) == 0) {
			result = i;
		} 
	}
	return result;
}

//Reads input file and adds everything to array of structs
int readInputFile(Customer customers[]) {
    FILE* input = fopen("hw4input.txt", "r");
    int cont = 1, custCount = 0;
    char tempName[MAXNAMELEN];
    int tempQuantity;
    char tempItem[MAXNAMELEN];
    double tempPrice;
    
    //Scan until EOF
    while(fscanf(input, "%s %d %s $%lf", tempName, &tempQuantity, tempItem, &tempPrice) != EOF) {
        if(custCheck(tempName, customers)) {
            int i = custIndex(tempName, customers);
            strcpy(customers[i].items[customers[i].itemCount].itemName, tempItem);
            customers[i].items[customers[i].itemCount].quantity = tempQuantity;
            customers[i].items[customers[i].itemCount].price = tempPrice;
            customers[i].itemCount++;
        } else { //Else add new customer with item
            strcpy(customers[custCount].name, tempName);
            strcpy(customers[custCount].items[customers[custCount].itemCount].itemName, tempItem);
            customers[custCount].items[customers[custCount].itemCount].quantity = tempQuantity;
            customers[custCount].items[customers[custCount].itemCount].price = tempPrice;
            customers[custCount].itemCount++;
            custCount++;
		      }	
    }
    
    fclose(input);
    return custCount;
}    

//Outputs hw4time.txt
void writeTimeFile(Customer customers[], int *custCount) {
    FILE* timeOut = fopen("hw4time.txt", "w");
    int i, j;
	  for(i =  0; i < *custCount; i++) {
		fprintf(timeOut, "%s\n", customers[i].name);
    for(j = 0; j < customers[i].itemCount; j++) {
      fprintf(timeOut, "%s %d $%.2lf\n", customers[i].items[j].itemName, customers[i].items[j].quantity, customers[i].items[j].price);
    }
    fprintf(timeOut, "\n");
	}
    fclose(timeOut);
  
}

//Outputs hw4money.txt
void writeMoneyFile(Customer customers[], int *custCount) {
    FILE* moneyOut = fopen("hw4money.txt", "w");
    int i, j;
	  for(i =  0; i < *custCount; i++) {
		fprintf(moneyOut, "%s, Total Order = $%.2lf\n", customers[i].name, customers[i].totalVal);
    for(j = 0; j < customers[i].itemCount; j++) {
      fprintf(moneyOut, "%s %d $%.2lf, Item Value = $%.2lf\n", customers[i].items[j].itemName, customers[i].items[j].quantity, customers[i].items[j].price, (customers[i].items[j].quantity * customers[i].items[j].price));
    }
    fprintf(moneyOut, "\n");
	}
    fclose(moneyOut);
  
}

//Sets customers total value based on total prices of items combined
void setCustTotal(Customer customers[], int *custCount) {
    int i, j;
    for(i = 0; i < *custCount; i++) {
        double totalValue;
        for(j = 0; j < customers[i].itemCount; j++) {
            totalValue += customers[i].items[j].price * customers[i].items[j].quantity;
        }
        customers[i].totalVal = totalValue;
        totalValue = 0;
    }
}

//Sorts customers based on total order value
void sortCustomers(Customer customers[], int *custCount) {
    int i, j;
    for(i = 0; i < *custCount; i++) {
        for(j = 0; j < *custCount-1; j++) {
            if(customers[j].totalVal < customers[j+1].totalVal) {
                Customer temp = customers[j];
                customers[j] = customers[j+1];
                customers[j+1] = temp;
            }
       } 
    }
}

//Sorts items based on unit price * quantity
void sortItems(Customer customers[], int *custCount) {
    int i, j, k;
    for(i = 0; i < *custCount; i++) {
        for(j = 0; j < customers[i].itemCount; j++) {
            for(k = 0; k < customers[i].itemCount - 1; k++) {
                double tempTotal = (customers[i].items[k].price * customers[i].items[k].quantity);
                double nextTempTotal = (customers[i].items[k+1].price * customers[i].items[k+1].quantity);
                if(tempTotal < nextTempTotal) {
                  Item temp = customers[i].items[k];
                  customers[i].items[k] = customers[i].items[k+1];
                  customers[i].items[k+1] = temp;
                }
            }
        }
    }
}

void main(void) {
    //Vars
    int custCount;
    Customer customers[MAXCUSTOMERS];
    //reads input file and returns number of customers
    custCount = readInputFile(customers); 
    //Write hw4time.txt
    writeTimeFile(customers, &custCount);
    //Sets the total value of customer
    setCustTotal(customers, &custCount);
    //Sorts items based on total price
    sortItems(customers, &custCount);
    //Sorts customers based on total order value
    sortCustomers(customers, &custCount);
    //Write hw4money.txt
    writeMoneyFile(customers, &custCount);
}