#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import "include/window.h"

NSWindow *window;
NSAutoreleasePool *pool;

// Creates a new macOS window
void macos_createWindow(int width, int height, const char *title)
{
  pool = [[NSAutoreleasePool alloc] init];
  [NSApplication sharedApplication];

  NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | 
    NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable;

  // Create the window
  NSRect rect = NSMakeRect(200, 200, width, height);
  window = [[NSWindow alloc] initWithContentRect:rect
    styleMask:style
    backing:NSBackingStoreBuffered
    defer:NO];
  
  [window setTitle:[NSString stringWithUTF8String:title]];
  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
}

// Runs the macOS window
void macos_run()
{
  [window orderFrontRegardless];
  [NSApp run];
  [pool drain];
}
