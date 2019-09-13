//
//  CloudBrowseCell.h
//  jwviewer
//
//  Created by Lucas Ibrahim on 24.11.18.
//  Copyright © 2018 pic. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface CloudBrowseCell : UITableViewCell

@property (strong, nonatomic) IBOutlet UIImageView *m_IconView;
@property (strong, nonatomic) IBOutlet UILabel *m_TitleLabel;
@property (weak, nonatomic) IBOutlet UIButton *m_LoginBtn;

@end

NS_ASSUME_NONNULL_END
