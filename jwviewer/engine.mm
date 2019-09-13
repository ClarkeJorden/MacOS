//
//  engine.m
//  engine
//
//  Created by seniorcoder on 11/6/18.
//  Copyright © 2018 pic. All rights reserved.
//

#import "engine.h"
#include "engine/CAD_ViewMain.h"

@implementation engine

-(void) OnMainInit
{
    OnMainInit();
}

-(void) OnMainRemove
{
    OnMainRemove();
}

-(void) DestroyFont
{
    DestroyFont();
}

-(void) InstallFont:(NSString*)fontName
{
    InstallFont((char*)[fontName UTF8String]);
}

-(BOOL) OnOpenPlan:(NSString*)strFilePath
{
    return OnOpenPlan((char*)[strFilePath UTF8String]);
}

-(void) OnSetRect:(int)left : (int)top : (int)right : (int)bottom
{
    OnSetRect(left, top, right, bottom);
}

-(void) OnInitStateView
{
    OnInitStateView();
}

-(void) OnPrevStateView
{
    OnPrevStateView();
}

-(void) OnExtentView
{
    OnExtentView();
}

-(void) OnAllView
{
    OnAllView();
}

-(void) OnZoom:(CGPoint)pt : (bool)bZoomIn
{
    OnZoom(pt.x, pt.y, bZoomIn);
}

-(void) OnRealtimeZoom:(CGFloat)fScale
{
    OnRealtimeZoom(fScale);
}

-(void) OnPanMove:(CGPoint)pt1 : (CGPoint)pt2
{
    OnPanMove(pt1.x, pt1.y, pt2.x, pt2.y);
}

-(void) OnLongPress:(CGPoint)pt
{
    OnLongPress(pt.x, pt.y);
}

-(void) SetSnapFlag:(int)bSnapFlag
{
    SetSnapFlag(bSnapFlag);
}

-(double) GetZoomScale
{
    return GetZoomScale();
}
-(void) SetPanMoveFlag:(bool)flag
{
    SetPanMoveFlag(flag);
}

@end
