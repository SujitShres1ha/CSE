#include <string.h>
#include <stdio.h>


typedef struct{
  char slots[100][100];
} appointments ;
void showAppointments(char appointment_center[]);

appointments Arlington = {
  .slots = {
    "open 2025-04-20 10:00",
    "open 2025-04-20 11:00",
    "open 2025-04-21 09:00"
  }
};
appointments Irving = {
  .slots = {
    "open 2025-04-20 10:00",
    "open 2025-04-20 11:00",
    "open 2025-04-21 09:00"
  }
};
appointments Denton = {
  .slots = {
    "open 2025-04-20 10:00",
    "open 2025-04-20 11:00",
    "open 2025-04-21 09:00"
  }
};
appointments FortWorth = {
  .slots = {
    "open 2025-04-20 10:00",
    "open 2025-04-20 11:00",
    "open 2025-04-21 09:00"
  }
};

void showAppointments(char appointment_center[]){
  appointments *center = NULL;

    if (!strcmp(appointment_center, "Arlington")) {
        center = &Arlington;
    } else if (!strcmp(appointment_center, "Irving")) {
        center = &Irving;
    } else if (!strcmp(appointment_center, "Denton")) {
        center = &Denton;
    } else if (!strcmp(appointment_center, "FortWorth")) {
        center = &FortWorth;
    } else {
        printf("Invalid center.\n");
        return;
    }

    printf("Available slots at %s:\n", appointment_center);
    for (int i = 0; i < 10; i++) {
        if (strlen(center->slots[i]) == 0) break; 
        if (strncmp(center->slots[i], "open", 4) == 0) {
            printf("%d. %s\n", i + 1, center->slots[i]);
        }
    }
}


