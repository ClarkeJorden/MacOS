#ifndef _CAD_VIEWMAIN_H_
#define _CAD_VIEWMAIN_H_

void OnMainInit();
void OnMainRemove();
BOOL OnOpenPlan(char *path);
void OnSetRect(int left, int top, int right, int bottom);
BOOL OnUpdateView(long wParam, long lParam);
float GetZoomScale();
void OnZoom(float x, float y, bool bZoomIn);
void OnRealtimeZoom(float fScale);
void OnPanMove(float x1, float y1, float x2, float y2);
void OnLongPress(float x, float y);
void SetSnapFlag(int bSnapFlag);
bool IsDrawing();
void SetDrawing(bool flag);
void OnAllView();
void OnExtentView();
void OnInitStateView();
void OnPrevStateView();

int  GetRenderMode();
void SetRenderMode(int nMode);

void OnEscape();
void SetPanMoveFlag(bool flag);
bool IsPanMoving();
void SetTempBuffer(unsigned char * buffer);
unsigned char* GetTempBuffer();
void SetTempBufferFlag(bool flag);
bool IsGetTempBuffer();
void SetCurrentSheet(int nIndex);
void DestroyFont();
void InstallFont(char *fontName);
void SetDrawTempBuffer(bool blTemp);
bool IsDrawTempBuffer();
void SetMagnifierViewRect(int x, int y, int width, int height);
void SetDrawMagnifierFlag(bool flag);
bool GetDrawMagnifierFlag();
void GetPaperDocInfo(char* szDocInfo);
void SetViewport(bool bClear = true);
void ConvertStringLocalToUni(char* ioStr, int nLen);

#endif
