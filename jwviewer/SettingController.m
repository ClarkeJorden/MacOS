//
//  SettingController.m
//  jwviewer
//
//  Created by Lucas Ibrahim on 24.11.18.
//  Copyright © 2018 pic. All rights reserved.
//

#import "SettingController.h"
#import "Constant.h"

@interface SettingController ()

@end

@implementation SettingController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    //The setup code (in viewDidLoad in your view controller)
    UITapGestureRecognizer *snapViewTap =
    [[UITapGestureRecognizer alloc] initWithTarget:self
                                            action:@selector(onSnapViewClicked)];
    [self.snapView addGestureRecognizer:snapViewTap];
}

- (void) viewWillAppear:(BOOL)animated {
    [self configureColorVC];
}

- (void)configureColorVC {
    [[UINavigationBar appearance] setBarTintColor:[UIColor primaryColor]];
    self.view.backgroundColor = [UIColor primaryColor];
}
- (IBAction)onBack:(id)sender {
    [self dismissViewControllerAnimated:true completion:nil];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (void)onSnapViewClicked {
    [self performSegueWithIdentifier:@"showSnap" sender:self];
}

@end
