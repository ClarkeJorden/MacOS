//
//  engine.h
//  engine
//
//  Created by seniorcoder on 11/6/18.
//  Copyright © 2018 pic. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@interface engine : NSObject

-(void) OnMainInit;
-(void) OnMainRemove;
-(BOOL) OnOpenPlan:(NSString*)strPath;
-(void) OnSetRect:(int)left : (int)top : (int)right : (int)bottom;
-(void) OnInitStateView;
-(void) OnPrevStateView;
-(void) OnExtentView;
-(void) OnAllView;
-(void) DestroyFont;
-(void) InstallFont:(NSString*)fontName;
-(void) OnZoom:(CGPoint)pt : (bool)bZoomIn;
-(void) OnRealtimeZoom:(CGFloat)fScale;
-(void) OnPanMove:(CGPoint)pt1 : (CGPoint)pt2;
-(void) OnLongPress:(CGPoint)pt;
-(void) SetSnapFlag:(int)bSnapFlag;
-(double) GetZoomScale;
-(void) SetPanMoveFlag:(bool)flag;

@end
