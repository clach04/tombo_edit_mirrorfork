//
//  iTomboViewController.m
//  iTombo
//
//  Created by Michael Efimov on 2/25/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "iTomboViewController.h"
#import "ObjectiveTomboCrypt.h"

@implementation iTomboViewController

@synthesize txtLabel, txtPassword, txtData;


/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

-(IBAction) clearPressed:(id) sender
{
	[self.txtPassword setText:@""];
	[self.txtData setText:@""];
}

-(void)setEncryptedFileUrl:(NSURL*) encryptedFile
{
	NSString *msg = [NSString stringWithFormat:@"Password for %@:", [encryptedFile lastPathComponent]];
	
	//[self.txtLabel setText:msg];
	[self.txtLabel setTitle:msg];
	
	if( nil != encryptedFileUrl && NSOrderedSame != [[encryptedFileUrl path] compare:[encryptedFile path]] )
	{
		BOOL bDeleted = [[NSFileManager defaultManager] removeItemAtURL:encryptedFileUrl
																  error:nil];
		(bDeleted);
		[encryptedFileUrl release];
	}
	
	encryptedFileUrl = [encryptedFile copy];
}


-(IBAction) viewPressed:(id) sender
{
	//[self.helloLabel setText:@"zhopa"];
	if( [[self.txtPassword text] isEqualToString:@""] || nil == encryptedFileUrl )
	{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[NSString stringWithFormat:@"Please Enter Password"]
														message:nil
													   delegate:nil
											  cancelButtonTitle:@"OK"
											  otherButtonTitles:nil];
		[alert autorelease];
		[alert show];
		
		return;
	}

	NSString *strData = [ObjectiveTomboCrypt DecryptFile:encryptedFileUrl withPassword:[self.txtPassword text]];
	
	if( [strData length] == 0 )
	{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[NSString stringWithFormat:@"Invalid Password"]
														message:nil
													   delegate:nil
											  cancelButtonTitle:@"OK"
											  otherButtonTitles:nil];
		[alert autorelease];
		[alert show];
		
		return;
	}
	   
	// hide keyboard
	[self.txtPassword resignFirstResponder];
	[self.txtData setText:strData];
}	

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *theTouch = [touches anyObject];
	
    if ([theTouch tapCount] == 2  && [self becomeFirstResponder]) {
		
        // selection management code goes here...
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[NSString stringWithFormat:@"Double Tap!"]
														message:nil
													   delegate:nil
											  cancelButtonTitle:@"OK"
											  otherButtonTitles:nil];
		[alert autorelease];
		[alert show];
        // bring up edit menu.
        /*
		UIMenuController *theMenu = [UIMenuController sharedMenuController];
        CGRect selectionRect = CGRectMake (currentSelection.x, currentSelection.y, SIDE, SIDE);
        [theMenu setTargetRect:selectionRect inView:self];
        [theMenu setMenuVisible:YES animated:YES];
		*/
    }
}


// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

@end
