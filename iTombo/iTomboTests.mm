//
//  iTomboTests.mm
//  iTombo
//
//  Created by Michael Efimov on 2/25/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "iTomboTests.h"


@implementation iTomboTests

#if USE_APPLICATION_UNIT_TEST     // all code under test is in the iPhone Application

- (void) testFail {
	//STFail(@"Must fail to succeed.");
}

- (void) testAppDelegate {
    
    id yourApplicationDelegate = [[UIApplication sharedApplication] delegate];
    //STAssertNotNil(yourApplicationDelegate, @"UIApplication failed to find the AppDelegate");
    
}

#else                           // all code under test must be linked into the Unit Test bundle

- (void) testMath {
    
    STAssertTrue((1+1)==2, @"Compiler isn't feeling well today :-(" );
    
}


#endif


@end
