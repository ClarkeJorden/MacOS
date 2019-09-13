//
//  DriveManager.m
//  jwviewer
//
//  Created by Lucas Ibrahim on 25.11.18.
//  Copyright © 2018 pic. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DriveManager.h"

@implementation DriveManager


-(id) init {
    if (self = [super init]) {
        self.bOneDriveSign = false;
        self.bGoogleSign = false;
        self.bDropBoxSign = false;
    }
    
    return self;
}
+(DriveManager*) sharedInstance {
    static DriveManager * sharedInstance = nil;
    
    @synchronized(self)
    {
        if (sharedInstance == nil) {
            sharedInstance = [[DriveManager alloc] init];
        }
        
        return sharedInstance;
    }
}

@end
