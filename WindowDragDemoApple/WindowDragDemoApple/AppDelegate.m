//
//  AppDelegate.m
//  WindowDragDemoApple
//
//  Created by Kaihua Li on 25/10/23.
//

#import "AppDelegate.h"
#import "DraggableView.h"

@interface AppDelegate ()

@property (strong, nonatomic) NSWindow *mainWindow;
@property (strong, nonatomic) NSView *draggableView;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Calculate 80% of the screen size
        NSSize screenSize = [NSScreen mainScreen].frame.size;
        CGFloat width = screenSize.width * 0.8;
        CGFloat height = screenSize.height * 0.8;
        
        NSRect windowRect = NSMakeRect((screenSize.width - width) / 2,
                                       (screenSize.height - height) / 2,
                                       width, height);
        
        self.mainWindow = [[NSWindow alloc] initWithContentRect:windowRect
                                                      styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable)
                                                        backing:NSBackingStoreBuffered
                                                          defer:NO];
        
        self.mainWindow.title = @"Draggable View Window";
        
        [self.mainWindow makeKeyAndOrderFront:nil];
        
        // Add draggable view
        [self setupDraggableView];
}

- (void)setupDraggableView {
    NSSize screenSize = [NSScreen mainScreen].frame.size;
    CGFloat width = screenSize.width * 0.3;
    CGFloat height = screenSize.height * 0.3;
    self.draggableView = [[DraggableView alloc] initWithFrame:NSMakeRect(50, 50, width, height)];
    self.draggableView.wantsLayer = YES;
    self.draggableView.layer.backgroundColor = [NSColor redColor].CGColor;
    
    [self.mainWindow.contentView addSubview:self.draggableView];
    
    // Register for mouse events
    [self.draggableView addTrackingArea:[[NSTrackingArea alloc] initWithRect:self.draggableView.bounds
                                                                     options:NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow | NSTrackingMouseMoved
                                                                       owner:self
                                                                    userInfo:nil]];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}


@end
