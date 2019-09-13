//
//  CloudBrowseController.m
//  jwviewer
//
//  Created by Lucas Ibrahim on 24.11.18.
//  Copyright © 2018 pic. All rights reserved.
//

#import "CloudBrowseController.h"
#import "Constant.h"
#import "CloudBrowseCell.h"
#import "DriveManager.h"

@interface CloudBrowseController ()

@end

@implementation CloudBrowseController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    if (DriveManager.sharedInstance.oneDriveClient == nil) {        
        DriveManager.sharedInstance.oneDriveClient = [ODClient loadCurrentClient];
    }
}

- (void) viewWillAppear:(BOOL)animated {
    [self configureColorVC];
}

- (void)configureColorVC {
    [[UINavigationBar appearance] setBarTintColor:[UIColor primaryColor]];
    self.view.backgroundColor = [UIColor primaryColor];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    CloudBrowseCell *cell = (CloudBrowseCell *) [tableView dequeueReusableCellWithIdentifier:@"browserTableCell"];

    if (indexPath.row == 0) {
        UIImage *icon = [UIImage imageNamed:@"DropBox"];
        [cell.m_IconView setImage:icon];
        [cell.m_TitleLabel setText:@"DropBox"];
        [cell.m_LoginBtn setTitle:@"Login" forState:UIControlStateNormal];
        if (!DriveManager.sharedInstance.bDropBoxSign) {
            [cell.m_LoginBtn setTitle:@"Login" forState:UIControlStateNormal];
            [cell.m_LoginBtn addTarget:self action:@selector(signInDropBoxAction) forControlEvents:UIControlEventTouchUpInside];
        } else {
            [cell.m_LoginBtn setTitle:@"Log out" forState:UIControlStateNormal];
            [cell.m_LoginBtn addTarget:self action:@selector(signOutDropBoxAction) forControlEvents:UIControlEventTouchUpInside];
        }
    }
    if (indexPath.row == 1) {
        UIImage *icon = [UIImage imageNamed:@"OneDrive"];
        [cell.m_IconView setImage:icon];
        [cell.m_TitleLabel setText:@"OneDrive"];
        
        if (!DriveManager.sharedInstance.bOneDriveSign) {
            [cell.m_LoginBtn setTitle:@"Login" forState:UIControlStateNormal];
            [cell.m_LoginBtn addTarget:self action:@selector(signInOneDriveAction) forControlEvents:UIControlEventTouchUpInside];
        } else {
            [cell.m_LoginBtn setTitle:@"Log out" forState:UIControlStateNormal];
            [cell.m_LoginBtn addTarget:self action:@selector(signOutOneDriveAction) forControlEvents:UIControlEventTouchUpInside];
        }
    }
    if (indexPath.row == 2) {
        UIImage *icon = [UIImage imageNamed:@"GoogleDrive"];
        [cell.m_IconView setImage:icon];
        [cell.m_TitleLabel setText:@"Google Drive"];
        [cell.m_LoginBtn setTitle:@"Login" forState:UIControlStateNormal];
        if (!DriveManager.sharedInstance.bGoogleSign) {
            [cell.m_LoginBtn setTitle:@"Login" forState:UIControlStateNormal];
            [cell.m_LoginBtn addTarget:self action:@selector(signInGoogleAction) forControlEvents:UIControlEventTouchUpInside];
        } else {
            [cell.m_LoginBtn setTitle:@"Log out" forState:UIControlStateNormal];
            [cell.m_LoginBtn addTarget:self action:@selector(signOutGoogleAction) forControlEvents:UIControlEventTouchUpInside];
        }
    }
    
    return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 3;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0) {
    }
    if (indexPath.row == 1) {
    }
    if (indexPath.row == 2) {
        if (DriveManager.sharedInstance.bOneDriveSign) {
            
        } else {
            [self signInOneDriveAction];
        }
    }
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 60.0f;
}

#pragma mark - Drop Box
- (void)signInDropBoxAction
{
}
- (void)signOutDropBoxAction
{
}

#pragma mark - One Drvie
- (void)signInOneDriveAction
{
    [ODClient authenticatedClientWithCompletion:^(ODClient *client, NSError *error){
        if (!error){
            DriveManager.sharedInstance.oneDriveClient = client;
            DriveManager.sharedInstance.bOneDriveSign = true;
            dispatch_async(dispatch_get_main_queue(), ^(){
                [self.tableView reloadData];
            });
        }
        else{
            [self showErrorAlert:error];
        }
    }];
}

- (void) signOutOneDriveAction {
    [[DriveManager sharedInstance].oneDriveClient signOutWithCompletion:^(NSError *signOutError){
        DriveManager.sharedInstance.bOneDriveSign = false;
        dispatch_async(dispatch_get_main_queue(), ^(){
            [self.tableView reloadData];
        });
    }];
}

#pragma mark - Google
- (void)signInGoogleAction
{
}
- (void)signOutGoogleAction
{
}


#pragma mark
- (void)showErrorAlert:(NSError*)error
{
    NSString *errorMsg;
    if ([error isAuthCanceledError]) {
        errorMsg = @"Sign-in was canceled!";
    }
    else if ([error isAuthenticationError]) {
        errorMsg = @"There was an error in the sign-in flow!";
    }
    else if ([error isClientError]) {
        errorMsg = @"Oops, we sent a bad request!";
    }
    else {
        errorMsg = @"Uh oh, an error occurred!";
    }
    UIAlertController *errorAlert = [UIAlertController alertControllerWithTitle:errorMsg
                                                                        message:[NSString stringWithFormat:@"%@", error]
                                                                 preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *ok = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action){}];
    [errorAlert addAction:ok];
    dispatch_async(dispatch_get_main_queue(), ^{
        [self presentViewController:errorAlert animated:YES completion:nil];
    });
    
}

@end
