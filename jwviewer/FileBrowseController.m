//
//  FileBrowseController.m
//  jwviewer
//
//  Created by Lucas Ibrahim on 24.11.18.
//  Copyright © 2018 pic. All rights reserved.
//

#import "FileBrowseController.h"
#import "Constant.h"
#import "FileBrowserCell.h"
#import "ViewController.h"
#import "engine.h"

@interface FileBrowseController () {
    engine* m_engine;
    NSMutableArray *m_LocalData;
    NSString *strFilePath;
    NSString *strFileName;
    NSString *strIconName;
}
@end

@implementation FileBrowseController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    m_engine = [[engine alloc] init];
    [m_engine OnMainInit];
    
    NSString *strFontPath = [[NSBundle mainBundle] pathForResource:@"Arial Unicode" ofType:@"ttf"];
    [m_engine InstallFont:strFontPath];
    
    NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
    NSLog(@"%@", documentsDirectory);
    NSError *error;
    NSArray* files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:documentsDirectory error:&error];
    NSRange jwwRange;
    NSRange dxfRange;
    
    m_LocalData = [[NSMutableArray alloc] init];
    for (NSString* file in files) {
        jwwRange = [file rangeOfString:@".jww"
                               options:NSCaseInsensitiveSearch];
        dxfRange = [file rangeOfString:@".dxf"
                               options:NSCaseInsensitiveSearch];
        if (jwwRange.location != NSNotFound || dxfRange.location!= NSNotFound) {
            [m_LocalData addObject:file];
        }
    }
}

- (void) viewDidUnload {
    [m_engine DestroyFont];
    [m_engine OnMainRemove];
    
    [super viewDidUnload];
}

- (void) viewWillAppear:(BOOL)animated {
    [self configureColorVC];
}

- (void)configureColorVC {
    [[UINavigationBar appearance] setBarTintColor:[UIColor primaryColor]];
    self.view.backgroundColor = [UIColor primaryColor];
}

#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
    if ([segue.identifier isEqualToString:@"showView"]) {
        ViewController *vc = [segue destinationViewController];
        vc.m_engine = m_engine;
        vc.filePath = strFilePath;
        vc.fileName = strFileName;
        vc.iconName = strIconName;
    }
}

#pragma TableView

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    FileBrowserCell *cell = (FileBrowserCell *) [tableView dequeueReusableCellWithIdentifier:@"browserTableCell"];
    
    NSString *fileName = [m_LocalData objectAtIndex:indexPath.row];
    cell.m_TitleLabel.text = fileName;
    if([fileName hasSuffix:@".jww"])
    {
        cell.m_IconView.image = [UIImage imageNamed:@"Doc_JWW.png"];
        cell.m_DescLabel.text = @"jww document";
    }
    else
    {
        cell.m_IconView.image = [UIImage imageNamed:@"Doc_DXF.png"];
        cell.m_DescLabel.text = @"dxf document";
    }

    return cell;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return [m_LocalData count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [m_LocalData count];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
    strFileName = [m_LocalData objectAtIndex:indexPath.row];
    strFilePath = [documentsDirectory stringByAppendingString:@"/"];
    strFilePath = [strFilePath stringByAppendingString:strFileName];
    
    if([[strFileName lowercaseString] hasSuffix:@".jww"]) {
        strIconName = @"Doc_JWW.png";
    }
    else if([[strFileName lowercaseString] hasSuffix:@".dxf"]) {
        strIconName = @"Doc_DXF.png";
    }

    [self performSegueWithIdentifier:@"showView" sender:self];
}

@end
