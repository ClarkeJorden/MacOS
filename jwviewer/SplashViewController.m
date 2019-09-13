//
//  SplashViewController.m
//  jwviewer
//
//  Created by Lucas Ibrahim on 25.11.18.
//  Copyright © 2018 pic. All rights reserved.
//

#import "SplashViewController.h"

@interface SplashViewController ()

@end

@implementation SplashViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [NSTimer scheduledTimerWithTimeInterval:2.0f
                                     target:self selector:@selector(showMain) userInfo:nil repeats:NO];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (void)showMain {
    [self performSegueWithIdentifier:@"showMain" sender:self];
}

@end
