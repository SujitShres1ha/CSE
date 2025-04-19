#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep function
#include "customer_utils.h"

typedef struct
{
  char slots[100][100];
} appointments;
char *showAppointments(char appointment_center[]);
void schedule_appointment(int customer_id);
char *checkExistingAppointment(int customer_id);
// void closeAppointmentSlot(char appointment_slot[], char appointment[]);
void change_appointment(int customer_id);

// Decorative pattern functions
void print_appointment_header()
{
  printf("\033[1;36m"); // Cyan text, bold
  printf("\n");
  printf("╔════════════════════════════════════════════════════════════╗\n");
  printf("║                 APPOINTMENT MANAGEMENT                     ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n");
  printf("\033[0m"); // Reset color
}

void print_center_header(const char *center)
{
  printf("\033[1;33m"); // Yellow text, bold
  printf("\n");
  printf("┌────────────────────────────────────────────────────────────┐\n");
  printf("│            Available Slots at %-28s │\n", center);
  printf("└────────────────────────────────────────────────────────────┘\n");
  printf("\033[0m"); // Reset color
}

void print_success_message(const char *message)
{
  printf("\033[1;32m"); // Green text, bold
  printf("\n");
  printf("╔════════════════════════════════════════════════════════════╗\n");
  printf("║  ✓  %-56s ║\n", message);
  printf("╚════════════════════════════════════════════════════════════╝\n");
  printf("\033[0m"); // Reset color
}

void print_error_message(const char *message)
{
  printf("\033[1;31m"); // Red text, bold
  printf("\n");
  printf("╔════════════════════════════════════════════════════════════╗\n");
  printf("║  ✗  %-56s ║\n", message);
  printf("╚════════════════════════════════════════════════════════════╝\n");
  printf("\033[0m"); // Reset color
}

void print_divider()
{
  printf("\033[1;36m"); // Cyan text, bold
  printf("──────────────────────────────────────────────────────────────\n");
  printf("\033[0m"); // Reset color
}

void show_progress()
{
  printf("\033[1;35mProcessing"); // Magenta text, bold
  for (int i = 0; i < 5; i++)
  {
    printf(".");
    fflush(stdout);
    usleep(200000); // 200ms delay
  }
  printf("\033[0m\n"); // Reset color
}

appointments Arlington = {
    .slots = {
        "2025-04-20 10:00 open",
        "2025-04-20 11:00 open",
        "2025-04-21 09:00 open"}};
appointments Irving = {
    .slots = {
        "2025-04-20 10:00 open",
        "2025-04-20 11:00 open",
        "2025-04-21 09:00 open"}};
appointments Denton = {
    .slots = {
        "2025-04-20 10:00 open",
        "2025-04-20 11:00 open",
        "2025-04-21 09:00 open"}};
appointments FortWorth = {
    .slots = {
        "2025-04-20 10:00 open",
        "2025-04-20 11:00 open",
        "2025-04-21 09:00 open"}};

void change_appointment(int customer_id)
{
  print_appointment_header();

  char *oldAppointment = checkExistingAppointment(customer_id);

  // Check if the customer has an existing appointment
  if (strlen(oldAppointment) > 0)
  {
    printf("\033[1;33mYou have an existing appointment on: %s\033[0m", oldAppointment);

    // Extract location from old appointment string
    // Format from checkExistingAppointment: "date at time at location"
    char location[50] = {0};
    char date[20] = {0};
    char time[20] = {0};

    sscanf(oldAppointment, "%s at %s at %s", date, time, location);

    print_divider();

    // Show available appointments and get new selection
    char *newAppointmentSlot = showAppointments(location);
    if (newAppointmentSlot == NULL)
    {
      print_error_message("Failed to select a new appointment");
      free(oldAppointment); // Free memory allocated by checkExistingAppointment
      return;
    }

    // Extract date and time from new appointment
    char newDate[20] = {0};
    char newTime[20] = {0};
    char status[20] = {0};
    sscanf(newAppointmentSlot, "%s %s %s", newDate, newTime, status);

    show_progress();

    // Create temporary file to update appointments
    FILE *APPOINTMENTS = fopen("appointments.txt", "r");
    FILE *TEMP = fopen("temp_appointments.txt", "w");

    if (APPOINTMENTS == NULL || TEMP == NULL)
    {
      print_error_message("Failed to open files");
      if (APPOINTMENTS)
        fclose(APPOINTMENTS);
      if (TEMP)
        fclose(TEMP);
      free(oldAppointment);
      free(newAppointmentSlot);
      return;
    }

    char line[200];
    int found = 0;

    // Copy all lines to temp file, replacing the appointment
    while (fgets(line, sizeof(line), APPOINTMENTS))
    {
      int line_customer_id;
      if (sscanf(line, "%d", &line_customer_id) == 1 && line_customer_id == customer_id)
      {
        // Replace with new appointment
        fprintf(TEMP, "%d %s %s %s\n", customer_id, location, newDate, newTime);
        found = 1;
      }
      else
      {
        // Copy line as is
        fputs(line, TEMP);
      }
    }

    fclose(APPOINTMENTS);
    fclose(TEMP);

    // Replace original file with updated temp file
    if (remove("appointments.txt") != 0)
    {
      print_error_message("Error deleting original file");
      free(oldAppointment);
      free(newAppointmentSlot);
      return;
    }

    if (rename("temp_appointments.txt", "appointments.txt") != 0)
    {
      print_error_message("Error renaming temporary file");
      free(oldAppointment);
      free(newAppointmentSlot);
      return;
    }

    if (found)
    {
      char success_msg[100];
      snprintf(success_msg, sizeof(success_msg), "Appointment updated to %s at %s", newDate, newTime);
      print_success_message(success_msg);
    }
    else
    {
      print_error_message("Could not find your existing appointment record");
    }

    // Free allocated memory
    free(oldAppointment);
    free(newAppointmentSlot);
  }
  else
  {
    print_error_message("You don't have an existing appointment");
    printf("\033[1;33mReturning to main menu...\033[0m\n");
  }
}

char *checkExistingAppointment(int customer_id)
{
  char line[200];
  char *slot = malloc(100);
  char date[10], time[10], location[10];
  int id;
  if (!slot)
  {
    printf("\033[1;31mMemory allocation failed\033[0m\n");
    return NULL;
  }

  FILE *APPOINTMENT = fopen("appointments.txt", "r");
  if (!APPOINTMENT)
  {
    printf("\033[1;31mFile didn't open\033[0m\n");
    exit(1);
  }

  while (fgets(line, sizeof(line), APPOINTMENT))
  {
    if (sscanf(line, "%d", &id) == 1 && id == customer_id) // Match with customer id
    {
      sscanf(line, "%*d %s %s %s", location, date, time); 
      snprintf(slot, 100, "%s at %s at %s\n", date, time, location); // Concat the data for convenience
      fclose(APPOINTMENT);
      return slot;
    }
  }

  fclose(APPOINTMENT);
  free(slot);
  return "";
}

char *showAppointments(char appointment_center[])
{
  appointments *center = NULL;
  char *appointment = NULL;
  int slot = 0, actual_slot = -1;
  int displayed_slots[10], display_count = 0;
  // Assign location to the center variable, by comparing with the actual locations
  if (!strcmp(appointment_center, "Arlington"))
  {
    center = &Arlington;
  }
  else if (!strcmp(appointment_center, "Irving"))
  {
    center = &Irving;
  }
  else if (!strcmp(appointment_center, "Denton"))
  {
    center = &Denton;
  }
  else if (!strcmp(appointment_center, "FortWorth"))
  {
    center = &FortWorth;
  }
  else
  {
    print_error_message("Invalid center");
    return NULL;
  }

  print_center_header(appointment_center);

  for (int i = 0; i < 10; i++)
  {
    if (strlen(center->slots[i]) == 0)
      break;
    if (strstr(center->slots[i], "open"))
    {
      printf("\033[1;36m  %d. %s\033[0m\n", display_count + 1, center->slots[i]);
      displayed_slots[display_count++] = i;
    }
  }

  if (display_count == 0)
  {
    print_error_message("No open slots available");
    return NULL;
  }

  print_divider();
  printf("\033[1;32mSelect a slot: \033[0m");
  scanf("%d", &slot);

  if (slot >= 1 && slot <= display_count)
  {
    actual_slot = displayed_slots[slot - 1];
    appointment = malloc(strlen(center->slots[actual_slot]) + 1);
    if (appointment == NULL)
    {
      print_error_message("Memory allocation failed");
      return NULL;
    }
    strcpy(appointment, center->slots[actual_slot]);
    return appointment;
  }

  print_error_message("Invalid selection");
  return NULL;
}

void schedule_appointment(int customer_id)
{
  print_appointment_header();

  FILE *RECORD = fopen("records.txt", "r");
  if (RECORD == NULL)
  {
    print_error_message("Failed to open records.txt");
    return;
  }

  char line[100];
  int id, age;
  char name[100], gender[10], blood_group[10], location[10];
  while (fgets(line, sizeof(line), RECORD))
  {
    if (sscanf(line, "%d", &id) == 1)
    {
      if (id == customer_id)
      {
        sscanf(line, "%d %s %d %s %s", &id, name, &age, gender, blood_group);
        break;
      }
    }
  }

  if (id != customer_id)
  {
    print_error_message("Your customer ID doesn't match");
    printf("\033[1;33mReturning to main menu.....\033[0m\n");
    return;
  }

  printf("\033[1;33m┌────────────────────── USER DETAILS ───────────────────────┐\033[0m\n");
  print_customer_info(id, name, age, gender, blood_group);
  printf("\033[1;33m└────────────────────────────────────────────────────────────┘\033[0m\n");

  FILE *APPOINTMENTS;
  APPOINTMENTS = fopen("appointments.txt", "a");
  if (APPOINTMENTS == NULL)
  {
    print_error_message("File didn't open");
    exit(1);
  }

  char *existingAppointment = checkExistingAppointment(customer_id);
  if (strlen(existingAppointment) > 0)
  {
    printf("\033[1;33mYou have an existing appointment on %s\033[0m", existingAppointment);
    printf("\033[1;33mReturning to main menu....\033[0m\n");
    free(existingAppointment);
    return;
  }

  print_divider();
  printf("\033[1;36m");
  printf("Choose an appointment center:\n");
  printf("  1. Arlington\n  2. FortWorth\n  3. Denton\n  4. Irving\n");
  printf("\033[0m");

  int center_no;
  char appointment_center[100];
  printf("\033[1;32mEnter your choice: \033[0m");
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
    print_error_message("Invalid choice");
    return;
  }

  printf("\033[1;36mYou have chosen the appointment center: \033[1;33m%s\033[0m\n", appointment_center);
  char *appointment = showAppointments(appointment_center);

  if (appointment == NULL)
  {
    return;
  }

  show_progress();

  fprintf(APPOINTMENTS, "\n%d %s %s", customer_id, appointment_center, appointment);
  fclose(APPOINTMENTS);

  char success_msg[100];
  snprintf(success_msg, sizeof(success_msg), "Appointment scheduled successfully at %s", appointment_center);
  print_success_message(success_msg);

  free(appointment);
}