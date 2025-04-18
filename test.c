#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char **getExistingCustomerIds();

char **getExistingCustomerIds()
{
  char *ids[10];
  FILE *RECORD = NULL;
  RECORD = fopen("records.txt", "r");
  
  
  int counter = 0;
  char line[100];
  while (fgets(line, sizeof(line), RECORD))
  {
    char str[10] = "";

    for (int i = 0; i < strlen(line); i++)
    {
      if (isspace(line[i]))
      {
        break;
      }
      char temp[2] = {line[i], '\0'};
      strcat(str, temp);
    }

    ids[counter] = (char *)malloc(strlen(str) + 1);
    strcpy(ids[counter], str);
    counter++;
  }
  return ids;
}