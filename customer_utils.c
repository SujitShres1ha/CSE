#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "appointments.h"

char **getExistingCustomerIds(int *size);
int generateCustomerId();
bool isDuplicate(char **ids, int id, int size);
void print_customer_info(int customer_id, char name[], int age, const char gender[], const char blood_group[]);
void update_customer_info(int customer_id);

void update_customer_info(int customer_id)
{
  int size;
  char **ids = getExistingCustomerIds(&size);
  if (!isDuplicate(ids, customer_id, size))
  {
    print_error_message("Enter a valid customer ID.");
    return;
  }

  FILE *RECORD = fopen("records.txt", "r");
  FILE *TEMP = fopen("temp_appointments.txt", "w");

  if (RECORD == NULL)
  {
    perror("Failed to open records.txt");
    return;
  }
  if (TEMP == NULL)
  {
    perror("Failed to create temporary file");
    fclose(RECORD);
    return;
  }

  char line[200];
  char name[50];
  int age;
  char gender[20];
  char blood_group[10];

  while (fgets(line, sizeof(line), RECORD))
  {
    int id;
    sscanf(line, "%d %s %d %s %s", &id, name, &age, gender, blood_group);

    if (id == customer_id)
    {
      // Found the customer to update
      printf("\033[1;33mCurrent Donor Information:\n\033[0m");
      print_customer_info(id, name, age, gender, blood_group);

      printf("\033[1;36mEnter new name (or press enter to keep current): \033[0m");
      getchar(); // consume leftover newline
      char new_name[50];
      fgets(new_name, sizeof(new_name), stdin);
      new_name[strcspn(new_name, "\n")] = 0;
      if (strlen(new_name) > 0)
        strcpy(name, new_name);

      printf("\033[1;36mEnter new age (or 0 to keep current): \033[0m");
      int new_age;
      scanf("%d", &new_age);
      getchar(); // consume newline
      if (new_age > 0)
        age = new_age;

    repeat_gender:
      printf("\033[1;36mEnter new gender (M/F/N or enter to keep current): \033[0m");
      char new_gender[10];
      fgets(new_gender, sizeof(new_gender), stdin);
      new_gender[strcspn(new_gender, "\n")] = 0;
      if (strlen(new_gender) > 0)
      {
        if (new_gender[0] == 'M')
          strcpy(gender, "Male");
        else if (new_gender[0] == 'F')
          strcpy(gender, "Female");
        else if (new_gender[0] == 'N')
          strcpy(gender, "Unspecified");
        else
        {
          printf("\033[1;31mInvalid gender input.\033[0m\n");
          goto repeat_gender;
        }
      }

    repeat_blood:
      printf("\033[1;36mEnter new blood group (or press enter to keep current): \033[0m");
      char new_blood_group[10];
      fgets(new_blood_group, sizeof(new_blood_group), stdin);
      new_blood_group[strcspn(new_blood_group, "\n")] = 0;
      if (strlen(new_blood_group) > 0)
      {
        if (!strcmp(new_blood_group, "A+") || !strcmp(new_blood_group, "A-") ||
            !strcmp(new_blood_group, "B+") || !strcmp(new_blood_group, "B-") ||
            !strcmp(new_blood_group, "AB+") || !strcmp(new_blood_group, "AB-") ||
            !strcmp(new_blood_group, "O+") || !strcmp(new_blood_group, "O-"))
        {
          strcpy(blood_group, new_blood_group);
        }
        else
        {
          printf("\033[1;31mInvalid blood group input.\033[0m\n");
          goto repeat_blood;
        }
      }

      // Write updated info to TEMP
      fprintf(TEMP, "%d %s %d %s %s\n", id, name, age, gender, blood_group);

      printf("\033[1;32m");
      printf("╔════════════════════════════════════════════════════════════╗\n");
      printf("║         Donor information updated successfully!            ║\n");
      printf("╚════════════════════════════════════════════════════════════╝\n");
      printf("\033[0m");

      printf("\033[1;33mUpdated Donor Information:\n\033[0m");
      print_customer_info(id, name, age, gender, blood_group);
    }
    else
    {
      // Write the original line unchanged
      fputs(line, TEMP);
    }
  }

  fclose(RECORD);
  fclose(TEMP);

  remove("records.txt");
  rename("temp_appointments.txt", "records.txt");

  for (int i = 0; i < size; i++)
  {
    free(ids[i]);
  }
  free(ids);
}

int generateCustomerId()
{
  srand(time(NULL));
  int size = 0;                               // Initialize size
  char **ids = getExistingCustomerIds(&size); // Pass address of size
  int customer_id;

  while (1)
  {
    customer_id = (rand() % (9999 - 1000 + 1)) + 1000; // Generate 4-digit random ID
    if (!isDuplicate(ids, customer_id, size))
    {
      break;
    }
  }

  // Free allocated memory for ids
  for (int i = 0; i < size; i++)
  {
    free(ids[i]);
  }
  free(ids);

  return customer_id;
}

bool isDuplicate(char **ids, int id, int size)
{
  char idStr[12];
  sprintf(idStr, "%d", id); // Convert integer id to string

  for (int i = 0; i < size; i++)
  {
    if (strcmp(ids[i], idStr) == 0)
    { // Compare strings
      return true;
    }
  }
  return false;
}
void print_customer_info(int customer_id, char name[], int age, const char gender[], const char blood_group[])
{
  printf("ID: %d\n", customer_id);
  printf("Name: %s\n", name);
  printf("Age: %d\n", age);
  printf("Gender: %s\n", gender);
  printf("Blood Group: %s\n", blood_group);
}
char **getExistingCustomerIds(int *size)
{
  char **ids = malloc(sizeof(char *) * 10);
  if (ids == NULL)
  {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  FILE *RECORD = fopen("records.txt", "r");
  if (RECORD == NULL)
  {
    perror("Failed to open records.txt");
    *size = 0;
    return ids; // Return empty array
  }

  int counter = 0;
  char line[100];
  while (fgets(line, sizeof(line), RECORD))
  {
    char str[10] = "";
    int i = 0;
    while (i < sizeof(str) - 1 && i < strlen(line) && !isspace(line[i]))
    {
      str[i] = line[i];
      i++;
    }
    str[i] = '\0';
    ids[counter] = malloc(strlen(str) + 1);
    if (ids[counter] == NULL)
    {
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