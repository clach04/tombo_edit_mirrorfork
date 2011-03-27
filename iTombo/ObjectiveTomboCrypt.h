//
//  ObjectiveTomboCrypt.h
//  iTombo
//
//  Created by Michael Efimov on 2/25/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface ObjectiveTomboCrypt : NSObject {

}
+(NSString*) DecryptFile:(NSURL*) encryptedFile withPassword:(NSString*) password;

@end
