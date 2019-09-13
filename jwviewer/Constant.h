//
//  Constant.h
//  jwviewer
//
//  Created by Lucas Ibrahim on 24.11.18.
//  Copyright © 2018 pic. All rights reserved.
//


#ifndef Constant_h
#define Constant_h

@interface UIColor (MyProject)

+(UIColor *) primaryColor;

@end

@implementation UIColor (MyProject)

+(UIColor *) primaryColor { return [UIColor colorWithRed:18/255.0f green:174/255.0f blue:225/255.0f alpha:1.0]; }

@end

#endif /* Constant_h */
