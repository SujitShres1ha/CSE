
#ifndef CUSTOMER_UTILS_H
#define CUSTOMER_UTILS_H

int generateCustomerId();
char **getExistingCustomerIds(int *size);
void freeCustomerIds(char **ids, int count);

#endif
