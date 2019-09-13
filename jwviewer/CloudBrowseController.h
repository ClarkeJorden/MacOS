//
//  CloudBrowseController.h
//  jwviewer
//
//  Created by Lucas Ibrahim on 24.11.18.
//  Copyright © 2018 pic. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface CloudBrowseController : UIViewController<UITableViewDelegate, UITableViewDataSource>

@property (weak, nonatomic) IBOutlet UITableView *tableView;


- (void)showErrorAlert:(NSError*)error;

@end

NS_ASSUME_NONNULL_END
