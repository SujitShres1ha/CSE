#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "customer_utils.h"
#include "appointments.h"


typedef enum {
  MALE,
  FEMALE,
  NOT_SPECIFY
} Gender;

typedef enum {
  A_POSITIVE,
  A_NEGATIVE,
  B_POSITIVE,
  B_NEGATIVE,
  AB_POSITIVE,
  AB_NEGATIVE,
  O_POSITIVE,
  O_NEGATIVE
} Bloodgroup;

const char* get_blood_group(Bloodgroup);
const char* get_gender(Gender);





void register_user(); 
void customers_record(int customer_id, char name[], int age, Gender, char appointment_center[], Bloodgroup);



int main()
{
  int choice;

  while (1) // Loop to keep the menu running
  {
    printf("--Main Menu--\n");
    printf("1. Register\n");
    printf("2. Donation History\n");
    printf("3. Update Donor\n");
    printf("4. Exit\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      register_user();
      break;
    case 2:
      printf("Donation History feature is under development.\n");
      break;
    case 3:
      printf("Update Donor feature is under development.\n");
      break;
    case 4:
      printf("Exiting the program.\n");
      exit(0);
    default:
      printf("Invalid choice. Please try again.\n");
    }
  }

  return 0;
}

void register_user()
{
  char name[100], gender[10], tattoos, appointment_center[100], blood_group[10];
  int age, center_no;    
  Bloodgroup b;
  Gender g;
  printf("Enter your name: \n");
  scanf("%s", name);

  printf("Enter your age: \n");
  scanf("%d", &age);

  if (age >= 18)
  {
    printf("You are eligible\n");
  }
  else
  {
    printf("You are not eligible\n");
    return;
  }

  repeat1: 
    printf("Enter gender: ");
    scanf("%s",gender);
    char first_char = gender[0];
    
    if (first_char == 'M'){
      g = MALE;
    }
    else if (first_char == 'F'){
      g = FEMALE;
    }
    else if (first_char == 'N'){
      g = NOT_SPECIFY;
    }
    else{
      printf("Invalid gender input.\n");
      goto repeat1;
    }

  repeat2:
    printf("Enter blood group: ");
    scanf("%s",blood_group);
    if (!strcmp(blood_group,"A+")){
      b = A_POSITIVE;
    }
    else if (!strcmp(blood_group,"B+")){
      b = B_POSITIVE;
    }
    else if (!strcmp(blood_group,"O+")){
      b = O_POSITIVE;
    }
    else if (!strcmp(blood_group,"AB+")){
      b = AB_POSITIVE;
    }
    else if (!strcmp(blood_group,"A-")){
      b = A_NEGATIVE;
    }
    else if (!strcmp(blood_group,"B-")){
      b = B_NEGATIVE;
    }
    else if (!strcmp(blood_group,"AB-")){
      b = AB_NEGATIVE;
    }
    else if (!strcmp(blood_group,"O-")){
      b = O_NEGATIVE;
    }
    else
    {
      printf("Invalid blood group input.\n");
      goto repeat2;
    }

  printf("Have you had any piercings or tattoos in the last 6 months? (y/n): ");
  scanf(" %c",&tattoos);
  if (tolower(tattoos) == 'y')
  {
    printf("You are not eligible due to recent tattoos or piercings.\n");
    return;
  }

  printf("Choose an appointment center:\n");
  printf("1. Arlington\n2. FortWorth\n3. Denton\n4. Irving\n");
  scanf("%d", &center_no);

  switch (center_no)
  {
  case 1:
    strcpy(appointment_center, "Arlington");
    break;
  case 2:
    strcpy(appointment_center, "FortWorth");
    break;
  case 3:
    strcpy(appointment_center, "Denton");
    break;
  case 4:
    strcpy(appointment_center, "Irving");
    break;
  default:
    printf("Invalid choice.\n");
    return;
  }

  printf("You have chosen the appointment center: %s\n", appointment_center);

  printf("Choose a slot: \n");
  showAppointments(appointment_center);
  
  int customer_id = generateCustomerId();


  customers_record(customer_id, name, age, g, appointment_center, b);
}

void customers_record(int customer_id, char name[], int age, Gender g, char appointment_center[], Bloodgroup b)
{
  FILE *RECORDS;
  RECORDS = fopen("records.txt", "a");
  if( RECORDS == NULL)
  {
    printf("file didn't open:\n ");
    exit(1);
  }
  fprintf(RECORDS, "\n%d %s %d %s %s %s", customer_id, name, age, get_gender(g), appointment_center, get_blood_group(b));
  fclose(RECORDS);
}

const char* get_blood_group(Bloodgroup b){
   switch (b){
      case A_POSITIVE: return "A+";
      case A_NEGATIVE: return "A-";
      case B_POSITIVE: return "B+";
      case B_NEGATIVE: return "B-";
      case AB_POSITIVE: return "AB+";
      case AB_NEGATIVE: return "AB-";
      case O_POSITIVE: return "O+";
      case O_NEGATIVE: return "O-";
      default: return "Unknown";
   }
}

const char* get_gender(Gender g){
  switch (g){
    case MALE: return "Male";
    case FEMALE: return "Female";
    case NOT_SPECIFY: return "Not Specified";
    default: return "Unknown";
  }
}

