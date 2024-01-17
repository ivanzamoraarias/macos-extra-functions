#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
   const char *notificationMessage = "Hola ! Pausa activa";
   const char *notificationTitle   = "Es hora de tu pausa activa";
   const char *iconPath = "/Users/ivan/Repos/c-test/assets/lion.icns";

   while (1)
   {
      //char iconApplescriptCommand[256];
      // snprintf(iconApplescriptCommand, sizeof(iconApplescriptCommand),
      //            "osascript -e 'tell application \"System Events\"' -e 'activate' -e 'end tell' -e "
      //            "'tell application \"Finder\" to set desktop picture to POSIX file \"%s\"'", iconPath);

        // Execute the AppleScript command to change the desktop picture (icon)
      // system(iconApplescriptCommand);

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
