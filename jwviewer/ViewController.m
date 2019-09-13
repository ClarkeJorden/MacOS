//
//  ViewController.m
//  jwviewer
//
//  Created by seniorcoder on 11/6/18.
//  Copyright © 2018 pic. All rights reserved.
//

#import "ViewController.h"
#import "GLView.h"
#import "Constant.h"
#import "FileBrowserCell.h"

@interface ViewController ()
{
}

@property (strong, nonatomic) IBOutlet UIImageView *m_IconView;
@property (strong, nonatomic) IBOutlet UILabel *m_TitleLabel;
@property (strong, nonatomic) IBOutlet GLView *m_GLView;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.m_GLView->m_engine = self.m_engine;

}

- (void) viewWillAppear:(BOOL)animated {
    [self configureColorVC];
}

- (void) viewDidAppear:(BOOL)animated {
    if (self.filePath != nil)
        [self openDocument:self.filePath : self.fileName : self.iconName];
}

- (void) viewDidUnload {
   
    [super viewDidUnload];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)configureColorVC {
    [[UINavigationBar appearance] setBarTintColor:[UIColor blackColor]];
    self.view.backgroundColor = [UIColor blackColor];
}

- (IBAction)onBack:(id)sender {
    [self dismissViewControllerAnimated:true completion:nil];
}

- (IBAction)onZoomIn:(id)sender {
    self.m_GLView->m_nZoomState = ZOOM_IN;
}

- (IBAction)onZoomOut:(id)sender {
    self.m_GLView->m_nZoomState = ZOOM_OUT;
}

- (IBAction)onPanMove:(id)sender {
    self.m_GLView->m_nZoomState = ZOOM_NONE;
}

- (void) openDocument:(NSString*)strFilePath : (NSString*)strFileName : (NSString*)strIconName
{
    [self.m_GLView prepareOpenGL];
    [self.m_engine OnSetRect: 0 :0 :(int)(self.m_GLView.bounds.size.width*self.m_GLView.layer.contentsScale) :(int)(self.m_GLView.bounds.size.height*self.m_GLView.layer.contentsScale)];
    if ([self.m_engine OnOpenPlan:strFilePath] == YES)
    {
        NSLog(@"true");
        self.m_IconView.image = [UIImage imageNamed:strIconName];
        self.m_TitleLabel.text = strFileName;
        [self.m_GLView initPlan];
        [self.m_GLView renderPlan];
    }
    else
    {
        NSLog(@"false");
    }
}

@end
