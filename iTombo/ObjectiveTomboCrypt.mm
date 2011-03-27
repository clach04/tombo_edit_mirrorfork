//
//  ObjectiveTomboCrypt.mm
//  iTombo
//
//  Created by Michael Efimov on 2/25/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "ObjectiveTomboCrypt.h"

typedef unsigned char       byte;
typedef unsigned char       BYTE;
typedef BYTE             *LPBYTE;
typedef const char		 *LPCTSTR;
typedef unsigned int	 *LPDWORD;	

#import "../jni/CryptManager.h"


@implementation ObjectiveTomboCrypt

+(NSString*) Decrypt:(NSData*) encryptedData withPassword:(NSString*) password
{
	
	NSMutableData* decryptedData = [NSMutableData dataWithData:encryptedData];
	int dataLen = decryptedData.length;
	
	unsigned char * pBytes = (unsigned char*) [decryptedData mutableBytes];
	
	int result = CryptManager_Decrypt(pBytes, dataLen, [password UTF8String]);
		
	if( 0 != result )
	{
		return [[NSString alloc] initWithData:decryptedData encoding:NSWindowsCP1251StringEncoding];
		// 
	}
	
	return @"";
}

+(NSString*) DecryptFile:(NSURL*) encryptedFile withPassword:(NSString*) password
{
	NSData* encryptedData = [NSData dataWithContentsOfURL:encryptedFile];

	return [self Decrypt:encryptedData withPassword:password];
}

@end
