//
//  GLView.m
//  jwviewer
//
//  Created by seniorcoder on 11/9/18.
//  Copyright © 2018 pic. All rights reserved.
//

#import "GLView.h"
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

@interface GLView()
@property (nonatomic, retain) EAGLContext *m_Context;
@property (nonatomic, retain) UITapGestureRecognizer *tapGR;
@property (nonatomic, retain) UIPanGestureRecognizer *panGR;
@property (nonatomic, retain) UIPinchGestureRecognizer *pinchGR;
@property (nonatomic, retain) UILongPressGestureRecognizer *longPressGR;
@end

@implementation GLView

@synthesize m_Context;

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/
- (id) initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if ([self respondsToSelector:@selector(setContentScaleFactor:)])
    {
        CGFloat scale = [[UIScreen mainScreen] scale];
        self.contentScaleFactor = scale;
        self.layer.contentsScale = scale;
    }
    return self;
}

- (void) renderPlan
{
    [EAGLContext setCurrentContext:m_Context];
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
    //glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    
    if (m_engine != nil && m_bIsOpen)
    {
        switch (m_nDrawState) {
            case ALL_VIEW:
                [m_engine OnAllView];
                break;
            case EXTENT_VIEW:
                [m_engine OnExtentView];
                break;
            case PREVSATATE_VIEW:
                [m_engine OnPrevStateView];
                break;
            case INITSATATE_VIEW:
                [m_engine OnInitStateView];
                break;
            case PAN_MOVE:
                [m_engine OnPanMove:m_prePoint : m_curPoint];
                break;
            case PAN_MOVE_ZOOM:
                [m_engine OnRealtimeZoom:m_zoomScale];
                break;
            case PAN_CLICK_ZOOM:
                [m_engine OnZoom:m_zoomPoint : m_nZoomState == ZOOM_IN];
                break;
            case LONG_PRESS:
                [m_engine OnLongPress:m_longPressPoint];
                break;
            default:
                break;
        }
    }
    
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderBuffer);
    [m_Context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void) awakeFromNib
{
    [super awakeFromNib];
    m_Context = nil; //[self prepareOpenGL];
    [self installGestureRecognizers];
    [self installPinchRecognizers];
    [self installPanRecognizers];
    [self installLongPressRecognizers];
}

- (void)installGestureRecognizers
{
    self.tapGR = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTap:)];
    self.tapGR.numberOfTapsRequired = 2;
    [self addGestureRecognizer:self.tapGR];
}

- (CGPoint) GetPoint:(CGPoint)inPt
{
    CGPoint pt;
    pt.x = inPt.x * self.contentScaleFactor;
    pt.y = inPt.y * self.contentScaleFactor;
    return pt;
}
- (void)handleTap:(UITapGestureRecognizer *)sender
{
    if (sender.state == UIGestureRecognizerStateEnded)
    {
        if(m_nZoomState == ZOOM_NONE)
            return;
        m_zoomPoint = [self GetPoint:[sender locationInView:self]];
        NSLog(@"Double Tapped! m_zoomPoint = %@", NSStringFromCGPoint(m_zoomPoint));
        m_nDrawState = PAN_CLICK_ZOOM;
        [self renderPlan];
    }
}

- (void)installPinchRecognizers
{
    self.pinchGR = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(handlePinch:)];
    [self.pinchGR setDelaysTouchesBegan:YES];
    [self addGestureRecognizer:self.pinchGR];
}

- (void)handlePinch:(UIPinchGestureRecognizer *)sender
{
    if (sender.state == UIGestureRecognizerStateBegan)
    {
        m_zoomInitScale = [m_engine GetZoomScale];
        [m_engine SetPanMoveFlag:true];
    }
    if(sender.state == UIGestureRecognizerStateEnded)
    {
        [m_engine SetPanMoveFlag:false];
    }
    
    if(sender.state == UIGestureRecognizerStateChanged || sender.state == UIGestureRecognizerStateEnded)
    {
        m_zoomScale = [m_engine GetZoomScale];
        NSLog(@"Pinched! m_zoomScale = %f", sender.scale);
        m_zoomScale = m_zoomInitScale * sender.scale / m_zoomScale;
        m_nDrawState = PAN_MOVE_ZOOM;
        [self renderPlan];
     }
}

- (void)installPanRecognizers
{
    self.panGR = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(handlePan:)];
    [self.panGR setDelaysTouchesBegan:YES];
    [self addGestureRecognizer:self.panGR];
}

- (void)handlePan:(UIPanGestureRecognizer *)sender
{
    if(sender.state == UIGestureRecognizerStateBegan)
    {
        m_prePoint = [self GetPoint:[sender locationInView:self]];
        NSLog(@"Paned! m_prePoint = %@", NSStringFromCGPoint(m_prePoint));
        [m_engine SetPanMoveFlag:true];
    }
    if(sender.state == UIGestureRecognizerStateEnded)
    {
        [m_engine SetPanMoveFlag:false];
    }

    if(sender.state == UIGestureRecognizerStateChanged || sender.state == UIGestureRecognizerStateEnded)
    {
        m_curPoint = [self GetPoint:[sender locationInView:self]];
        NSLog(@"Paned! m_curPoint = %@", NSStringFromCGPoint(m_curPoint));
        m_nDrawState = PAN_MOVE;
        [self renderPlan];
        m_prePoint = m_curPoint;
    }
}

- (void)installLongPressRecognizers
{
    self.longPressGR = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPress:)];
    [self.longPressGR setNumberOfTapsRequired : 0];
    [self.longPressGR setMinimumPressDuration : 1.5];
    //[self.longPressGR setDelegate : self];
    [self addGestureRecognizer : self.longPressGR];
}

- (void)handleLongPress:(UILongPressGestureRecognizer *)sender
{
    if (sender.state == UIGestureRecognizerStateBegan)
    {
        m_longPressPoint = [sender locationInView:self];
        NSLog(@"Paned! m_curPoint = %@", NSStringFromCGPoint(m_longPressPoint));
        m_nDrawState = LONG_PRESS;
        [self renderPlan];
    }
}

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (void) prepareOpenGL
{
    if (m_Context != nil)
        return;
    m_Context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    [EAGLContext setCurrentContext:m_Context];
    
    glGenFramebuffersOES(1, &framebuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
    glGenRenderbuffersOES(1, &colorRenderBuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderBuffer);
    [m_Context renderbufferStorage:GL_RENDERBUFFER_OES
                    fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderBuffer);
    GLint height, width;
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &width);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &height);
    glGenRenderbuffersOES(1, &depthBuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthBuffer);
    glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, width, height);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthBuffer);
    GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
    if (status != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"fail");
    }
}

- (void) initPlan
{
    m_bIsOpen = true;
    m_zoomScale = 1.0f;
    m_nZoomState = ZOOM_NONE;
    m_prePoint.x = 0; m_prePoint.y = 0;
    m_curPoint.x = 0; m_curPoint.y = 0;
    m_zoomPoint.x = 0; m_zoomPoint.y = 0;
    m_longPressPoint.x = 0; m_longPressPoint.y = 0;
    m_nDrawState = ALL_VIEW;
}

@end
