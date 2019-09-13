//
//  GLView.h
//  jwviewer
//
//  Created by seniorcoder on 11/9/18.
//  Copyright © 2018 pic. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "engine.h"

enum ZoomState {
    ZOOM_NONE,
    ZOOM_IN,
    ZOOM_OUT
};

enum DrawState {
    ALL_VIEW,
    EXTENT_VIEW,
    PREVSATATE_VIEW,
    INITSATATE_VIEW,
    PAN_MOVE,
    PAN_MOVE_ZOOM,
    PAN_CLICK_ZOOM,
    LONG_PRESS
};

@interface GLView : UIView {
    GLuint framebuffer;
    GLuint colorRenderBuffer;
    GLuint depthBuffer;

@public
    engine* m_engine;
    Boolean m_bIsOpen;
    enum DrawState m_nDrawState;
    CGPoint m_prePoint;
    CGPoint m_curPoint;
    CGPoint m_zoomPoint;
    CGFloat m_zoomInitScale;
    CGFloat m_zoomScale;
    enum ZoomState m_nZoomState;
    CGPoint m_longPressPoint;
}
- (void) prepareOpenGL;
- (void) initPlan;
- (void) renderPlan;
@end
