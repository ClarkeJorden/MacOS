//
//  SnapModeController.m
//  jwviewer
//
//  Created by Lucas Ibrahim on 26.11.18.
//  Copyright © 2018 pic. All rights reserved.
//

#import "SnapModeController.h"
#import "Constant.h"

@interface SnapModeController ()

@end

@implementation SnapModeController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    m_bIsEnd = false;
    m_bIsMiddle = false;
    m_bIsIntersect = false;
    m_bIsPerpen = false;
}

- (void) viewWillAppear:(BOOL)animated {
    [self configureColorVC];
}

- (void)configureColorVC {
    [[UINavigationBar appearance] setBarTintColor:[UIColor primaryColor]];
    self.view.backgroundColor = [UIColor primaryColor];
}
- (IBAction)onBack:(id)sender {
    int bOSnapFlag = SNAP_NONE;
    if (m_bIsEnd)
        bOSnapFlag |= SNAP_END;
    if (m_bIsMiddle)
        bOSnapFlag |= SNAP_MIDDLE;
    if (m_bIsIntersect)
        bOSnapFlag |= SNAP_INTERSECT;
    if (m_bIsPerpen)
        bOSnapFlag |= SNAP_PERPEN;
    [m_engine SetSnapFlag:bOSnapFlag];
    [self dismissViewControllerAnimated:true completion:nil];
}

- (IBAction)onEndSnap:(id)sender {
    m_bIsEnd = !m_bIsEnd;
}

- (IBAction)onMiddleSnap:(id)sender {
    m_bIsMiddle = !m_bIsMiddle;
}

- (IBAction)onIntersectSnap:(id)sender {
    m_bIsIntersect = !m_bIsIntersect;
}

- (IBAction)onPerpenSnap:(id)sender {
    m_bIsPerpen = !m_bIsPerpen;
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
