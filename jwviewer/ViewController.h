//
//  ViewController.h
//  jwviewer
//
//  Created by seniorcoder on 11/6/18.
//  Copyright © 2018 pic. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "engine.h"

@interface ViewController : UIViewController {
    
}

@property (nonatomic, strong) engine *m_engine;
@property (nonatomic, strong) NSString *filePath;
@property (nonatomic, strong) NSString *fileName;
@property (nonatomic, strong) NSString *iconName;

@end

