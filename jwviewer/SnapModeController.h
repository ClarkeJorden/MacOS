//
//  SnapModeController.h
//  jwviewer
//
//  Created by Lucas Ibrahim on 26.11.18.
//  Copyright © 2018 pic. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "engine.h"

#define		SNAP_NONE			0x0000
#define		SNAP_END			0x0001
#define		SNAP_MIDDLE			0x0002
#define		SNAP_INTERSECT		0x0040
#define		SNAP_PERPEN			0x0100

NS_ASSUME_NONNULL_BEGIN

@interface SnapModeController : UIViewController {
    
@public
    engine* m_engine;
    Boolean m_bIsEnd;
    Boolean m_bIsMiddle;
    Boolean m_bIsIntersect;
    Boolean m_bIsPerpen;
}

@end

NS_ASSUME_NONNULL_END
