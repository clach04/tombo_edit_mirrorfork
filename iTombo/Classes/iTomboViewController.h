//
//  iTomboViewController.h
//  iTombo
//
//  Created by Michael Efimov on 2/25/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface iTomboViewController : UIViewController {

	IBOutlet id txtLabel;
	IBOutlet id txtPassword;
	IBOutlet id txtData;
	
	NSURL*		encryptedFileUrl;

}

@property (nonatomic, retain) IBOutlet id txtLabel;
@property (nonatomic, retain) IBOutlet id txtPassword;
@property (nonatomic, retain) IBOutlet id txtData;

-(IBAction) viewPressed:(id) sender;
-(IBAction) clearPressed:(id) sender;

-(void)setEncryptedFileUrl:(NSURL*) encryptedFile;

@end

