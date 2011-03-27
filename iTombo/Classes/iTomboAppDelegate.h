//
//  iTomboAppDelegate.h
//  iTombo
//
//  Created by Michael Efimov on 2/25/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class iTomboViewController;

@interface iTomboAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    iTomboViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet iTomboViewController *viewController;

@end

