//
//  FileBrowserCell.h
//  jwviewer
//
//  Created by seniorcoder on 11/13/18.
//  Copyright © 2018 pic. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface FileBrowserCell : UITableViewCell

@property (strong, nonatomic) IBOutlet UIImageView *m_IconView;
@property (strong, nonatomic) IBOutlet UILabel *m_TitleLabel;
@property (strong, nonatomic) IBOutlet UILabel *m_DescLabel;

@end
