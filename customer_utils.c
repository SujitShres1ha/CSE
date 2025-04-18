#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

char **getExistingCustomerIds(int *size);
int generateCustomerId();
bool isDuplicate(char **ids, int id, int size);

int generateCustomerId() {
    srand(time(NULL));
    int size = 0; // Initialize size
    char **ids = getExistingCustomerIds(&size); // Pass address of size
    int customer_id;

    while (1) {
        customer_id = (rand() % (9999 - 1000 + 1)) + 1000; // Generate 4-digit random ID
        if (!isDuplicate(ids, customer_id, size)) {
            break;
        }
    }

    // Free allocated memory for ids
    for (int i = 0; i < size; i++) {
        free(ids[i]);
    }
    free(ids);

    return customer_id;
}

bool isDuplicate(char **ids, int id, int size) {
    char idStr[12];
    sprintf(idStr, "%d", id); // Convert integer id to string

    for (int i = 0; i < size; i++) {
        if (strcmp(ids[i], idStr) == 0) { // Compare strings
            return true;
        }
    }
    return false;
}

char **getExistingCustomerIds(int *size) {
    char **ids = malloc(sizeof(char *) * 10);
    if (ids == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    FILE *RECORD = fopen("records.txt", "r");
    if (RECORD == NULL) {
        perror("Failed to open records.txt");
        *size = 0;
        return ids; // Return empty array
    }

    int counter = 0;
    char line[100];
    while (fgets(line, sizeof(line), RECORD)) {
        char str[10] = "";

        for (int i = 0; i < strlen(line); i++) {
            if (isspace(line[i])) {
                break;
            }
            char temp[2] = {line[i], '\0'};
            strcat(str, temp);
        }

        ids[counter] = malloc(strlen(str) + 1);
        if (ids[counter] == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        strcpy(ids[counter], str);
        counter++;
    }
    fclose(RECORD);
    *size = counter;
    return ids;
}