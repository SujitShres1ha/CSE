
#ifndef APPOINTMENTS_H
#define APPOINTMENTS_H

typedef struct
{
  char slots[10][10];
} appointments;

extern appointments Arlington;
extern appointments Irving;
extern appointments Denton;
extern appointments FortWorth;

char *showAppointments(char appointment_center[]);
void schedule_appointment(int customer_id);
char *checkExistingAppointment(int customer_id);
void closeAppointmentSlot(char appointment_slot[], char appointment[]);
void change_appointment(int customer_id);
void print_error_message(const char *message);
#endif