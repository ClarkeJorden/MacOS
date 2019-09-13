//
//  DriveManager.h
//  jwviewer
//
//  Created by Lucas Ibrahim on 25.11.18.
//  Copyright © 2018 pic. All rights reserved.
//

#import <OneDriveSDK/OneDriveSDK.h>

@interface DriveManager : NSObject {
    
    
}

@property (nonatomic, strong)  ODClient *oneDriveClient;
@property (nonatomic, assign) Boolean bOneDriveSign;
@property (nonatomic, assign) Boolean bGoogleSign;
@property (nonatomic, assign) Boolean bDropBoxSign;

+(DriveManager*) sharedInstance;

@end

