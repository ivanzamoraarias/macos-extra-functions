#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
   const char *notificationMessage = "Hola ! Pausa activa";
   const char *notificationTitle   = "Es hora de tu pausa activa";

   while (1)
   {
      char applescriptCommand[256];
      snprintf(applescriptCommand, sizeof(applescriptCommand),
               "osascript -e 'display notification \"%s\" with title \"%s\"'",
               notificationMessage, notificationTitle);

      int result = system(applescriptCommand);

      if (result == 0)
      {
         printf("Notification sent successfully!\n");
      }
      else
      {
         printf("Error sending notification\n");
      }

      sleep(3600);
   }

   return(0);
}
