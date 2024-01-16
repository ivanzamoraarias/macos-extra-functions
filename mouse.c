#include <stdio.h>
#include <ApplicationServices/ApplicationServices.h>

void changeDesktopToLeft()
{
   system("osascript -e 'tell application \"System Events\" to key code 123 using control down'");
}

void changeDesktopToRight()
{
   system("osascript -e 'tell application \"System Events\" to key code 124 using control down'");
}

// Function to handle mouse events
CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
   int buttonNumber =
      (int)CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber);

   printf("Mouse event detected:(Button %d)\n", buttonNumber);

   if (buttonNumber == 4)
   {
      printf("Changing desktop to the one on the left...\n");
      changeDesktopToLeft();
   }
   if (buttonNumber == 3)
   {
      printf("Changing desktop to the one on the left...\n");
      changeDesktopToRight();
   }

   // Return the event
   return(event);
}

int main()
{
   // Create an event tap to listen for mouse events
   CGEventMask   eventMask = (1 << kCGEventLeftMouseDown) | (1 << kCGEventRightMouseDown) | (1 << kCGEventOtherMouseDown);
   CFMachPortRef eventTap  = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, myCGEventCallback, NULL);

   if (!eventTap)
   {
      fprintf(stderr, "Failed to create event tap\n");
      return(1);
   }

   // Add the event tap to the run loop
   CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
   CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

   // Start the event tap
   CGEventTapEnable(eventTap, true);

   // Run the run loop to start listening for events
   CFRunLoopRun();

   // Clean up and release resources
   CFRelease(runLoopSource);
   CFRelease(eventTap);

   return(0);
}
