
#ifndef CUSTOMER_UTILS_H
#define CUSTOMER_UTILS_H

int generateCustomerId();
char **getExistingCustomerIds(int *size);
void freeCustomerIds(char **ids, int count);
void print_customer_info(int customer_id, char name[], int age, const char gender[], const char blood_group[]);
void update_customer_info(int customer_id);

#endif
