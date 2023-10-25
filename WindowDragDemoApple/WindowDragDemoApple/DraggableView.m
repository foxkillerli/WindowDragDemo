//
//  DraggableView.m
//  WindowDragDemoApple
//
//  Created by Kaihua Li on 25/10/23.
//

#import <Foundation/Foundation.h>
#import "DraggableView.h"

@implementation DraggableView

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)mouseDown:(NSEvent *)theEvent {
    // Capture the initial mouse down event if needed.
}

- (void)mouseDragged:(NSEvent *)theEvent {
    NSPoint locationInSuperView = [self.superview convertPoint:theEvent.locationInWindow fromView:nil];
    CGFloat halfWidth = self.frame.size.width / 2;
    CGFloat halfHeight = self.frame.size.height / 2;

    // Set the new origin
    NSPoint newOrigin = NSMakePoint(locationInSuperView.x - halfWidth,
                                    locationInSuperView.y - halfHeight);
    
    // Constrain the draggable view within the superview bounds
    if (newOrigin.x < 0) {
        newOrigin.x = 0;
    }
    if (newOrigin.y < 0) {
        newOrigin.y = 0;
    }
    if (newOrigin.x + self.frame.size.width > self.superview.frame.size.width) {
        newOrigin.x = self.superview.frame.size.width - self.frame.size.width;
    }
    if (newOrigin.y + self.frame.size.height > self.superview.frame.size.height) {
        newOrigin.y = self.superview.frame.size.height - self.frame.size.height;
    }
    
    self.frame = NSMakeRect(newOrigin.x, newOrigin.y,
                            self.frame.size.width,
                            self.frame.size.height);
}

@end
