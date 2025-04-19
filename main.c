#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include "customer_utils.h"
#include "appointments.h"

typedef enum
{
  MALE,
  FEMALE,
  NOT_SPECIFY
} Gender;

typedef enum
{
  A_POSITIVE,
  A_NEGATIVE,
  B_POSITIVE,
  B_NEGATIVE,
  AB_POSITIVE,
  AB_NEGATIVE,
  O_POSITIVE,
  O_NEGATIVE
} Bloodgroup;

const char *get_blood_group(Bloodgroup);
const char *get_gender(Gender);

// Decorative pattern functions
void print_header()
{
  printf("\033[1;31m"); // Red text, bold
  printf("\n");
  printf("╔════════════════════════════════════════════════════════════╗\n");
  printf("║                                                            ║\n");
  printf("║             BLOOD DONATION MANAGEMENT SYSTEM               ║\n");
  printf("║                                                            ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n");
  printf("\033[0m"); // Reset color
}

void print_menu()
{
  printf("\033[1;36m"); // Cyan text, bold
  printf("\n");
  printf("╔════════════════════════ MAIN MENU ═════════════════════════╗\n");
  printf("║                                                            ║\n");
  printf("║  1. Register                                               ║\n");
  printf("║  2. Schedule your appointment                              ║\n");
  printf("║  3. Change your appointment                                ║\n");
  printf("║  4. Update Donor                                           ║\n");
  printf("║  5. Exit                                                   ║\n");
  printf("║                                                            ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n");
  printf("\033[0m"); // Reset color
}

void print_footer()
{
  printf("\033[1;33m"); // Yellow text, bold
  printf("\n");
  printf("╔════════════════════════════════════════════════════════════╗\n");
  printf("║  Your donation can save up to three lives. Thank you!      ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n");
  printf("\033[0m"); // Reset color
}

void print_prompt()
{
  printf("\033[1;32m"); // Green text, bold
  printf("\n");
  printf("   ➤ Enter your choice: ");
  printf("\033[0m"); // Reset color
}

void print_exit_message()
{
  printf("\033[1;35m"); // Magenta text, bold
  printf("\n");
  printf("╔════════════════════════════════════════════════════════════╗\n");
  printf("║         Thank you for using our services!                  ║\n");
  printf("║                 Have a great day!                          ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n");
  printf("\033[0m"); // Reset color
}

void print_registration_header()
{
  printf("\033[1;34m"); // Blue text, bold
  printf("\n");
  printf("╔════════════════════════════════════════════════════════════╗\n");
  printf("║                DONOR REGISTRATION FORM                     ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n");
  printf("\033[0m"); // Reset color
}

void loading_animation()
{
  printf("\nProcessing");
  for (int i = 0; i < 5; i++)
  {
    printf(".");
    fflush(stdout);
    usleep(300000); // 300ms delay
  }
  printf("\n");
}

void register_user();
void customers_record(int customer_id, char name[], int age, Gender, Bloodgroup);

int main()
{
  int choice;

  while (1) // Loop to keep the menu running
  {
    system("clear"); // Clear screen (use "cls" for Windows)

    print_header();
    print_menu();
    print_footer();
    print_prompt();

    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      register_user();
      printf("\nPress Enter to continue...");
      getchar(); // Consume newline
      getchar(); // Wait for Enter
      break;
    case 2:
    {
      int id;
      printf("\033[1;32m   ➤ Enter your customer ID: \033[0m");
      scanf("%d", &id);
      loading_animation();
      schedule_appointment(id);
      printf("\nPress Enter to continue...");
      getchar(); // Consume newline
      getchar(); // Wait for Enter
      break;
    }
    case 3:
    {
      int id;
      printf("\033[1;32m   ➤ Enter your customer ID: \033[0m");
      scanf("%d", &id);
      loading_animation();
      change_appointment(id);
      printf("\nPress Enter to continue...");
      getchar(); // Consume newline
      getchar(); // Wait for Enter
      break;
    }
    case 4:
    {
      int id;
      printf("\033[1;32m   ➤ Enter your customer ID: \033[0m");
      scanf("%d", &id);
      loading_animation();
      update_customer_info(id);
      printf("\nPress Enter to continue...");
      getchar(); // Consume newline
      getchar(); // Wait for Enter
      break;
    }
    case 5:
      print_exit_message();
      exit(0);
    default:
      printf("\033[1;31m   ⚠ Invalid choice. Please try again.\033[0m\n");
      printf("\nPress Enter to continue...");
      getchar(); // Consume newline
      getchar(); // Wait for Enter
    }
  }

  return 0;
}

void register_user()
{
  print_registration_header();

  char name[100], gender[10], tattoos, blood_group[10];
  int age;
  Bloodgroup b;
  Gender g;
  printf("\033[1;36mEnter your name: \033[0m");
  scanf("%s", name);

  printf("\033[1;36mEnter your age: \033[0m");
  scanf("%d", &age);

  if (age >= 18)
  {
    printf("\033[1;32mYou are eligible\033[0m\n");
  }
  else
  {
    printf("\033[1;31mYou are not eligible\033[0m\n");
    return;
  }

repeat1:
  printf("\033[1;36mEnter gender (M: Male | F: Female | N: Not Specify): \033[0m");
  scanf("%s", gender);
  char first_char = gender[0];

  if (first_char == 'M')
  {
    g = MALE;
  }
  else if (first_char == 'F')
  {
    g = FEMALE;
  }
  else if (first_char == 'N')
  {
    g = NOT_SPECIFY;
  }
  else
  {
    printf("\033[1;31mInvalid gender input.\033[0m\n");
    goto repeat1;
  }

repeat2:
  printf("\033[1;36mEnter blood group: \033[0m");
  scanf("%s", blood_group);
  if (!strcmp(blood_group, "A+"))
  {
    b = A_POSITIVE;
  }
  else if (!strcmp(blood_group, "B+"))
  {
    b = B_POSITIVE;
  }
  else if (!strcmp(blood_group, "O+"))
  {
    b = O_POSITIVE;
  }
  else if (!strcmp(blood_group, "AB+"))
  {
    b = AB_POSITIVE;
  }
  else if (!strcmp(blood_group, "A-"))
  {
    b = A_NEGATIVE;
  }
  else if (!strcmp(blood_group, "B-"))
  {
    b = B_NEGATIVE;
  }
  else if (!strcmp(blood_group, "AB-"))
  {
    b = AB_NEGATIVE;
  }
  else if (!strcmp(blood_group, "O-"))
  {
    b = O_NEGATIVE;
  }
  else
  {
    printf("\033[1;31mInvalid blood group input.\033[0m\n");
    goto repeat2;
  }

  printf("\033[1;36mHave you had any piercings or tattoos in the last 6 months? (y/n): \033[0m");
  scanf(" %c", &tattoos);
  if (tolower(tattoos) == 'y')
  {
    printf("\033[1;31mYou are not eligible due to recent tattoos or piercings.\033[0m\n");
    return;
  }

  loading_animation();

  int customer_id = generateCustomerId();

  printf("\033[1;32m");
  printf("╔════════════════════════════════════════════════════════════╗\n");
  printf("║         You have been registered successfully!             ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n");
  printf("\033[0m");

  printf("\033[1;33m");
  printf("Your Details: \n");
  print_customer_info(customer_id, name, age, get_gender(g), get_blood_group(b));
  printf("\033[0m");

  customers_record(customer_id, name, age, g, b);
}

void customers_record(int customer_id, char name[], int age, Gender g, Bloodgroup b)
{
  FILE *RECORDS;
  RECORDS = fopen("records.txt", "a");
  if (RECORDS == NULL)
  {
    printf("file didn't open:\n ");
    exit(1);
  }
  fprintf(RECORDS, "\n%d %s %d %s %s", customer_id, name, age, get_gender(g), get_blood_group(b));
  fclose(RECORDS);
}

const char *get_blood_group(Bloodgroup b)
{
  switch (b)
  {
  case A_POSITIVE:
    return "A+";
  case A_NEGATIVE:
    return "A-";
  case B_POSITIVE:
    return "B+";
  case B_NEGATIVE:
    return "B-";
  case AB_POSITIVE:
    return "AB+";
  case AB_NEGATIVE:
    return "AB-";
  case O_POSITIVE:
    return "O+";
  case O_NEGATIVE:
    return "O-";
  default:
    return "Unknown";
  }
}

const char *get_gender(Gender g)
{
  switch (g)
  {
  case MALE:
    return "Male";
  case FEMALE:
    return "Female";
  case NOT_SPECIFY:
    return "Unspecified";
  default:
    return "Unknown";
  }
}