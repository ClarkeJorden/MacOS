#if !defined(AFX_CAD_LAYOUTVIEW_H)
#define AFX_CAD_LAYOUTVIEW_H


// CAD_LayoutView.h : header file


/*#include "define.h"*/
#include "CAD_ViewportView.h"

 

/////////////////////////////////////////////////////////////////////////////
// CCAD_LayoutView window

class CCAD_PaperDoc;

class   CCAD_LayoutView : public CCAD_ViewportView
{
// Construction
public:
	CCAD_LayoutView(short nID = 0);

// Attributes
public:
	CCAD_PaperDoc*	m_pPaperDoc;
// Operations
public:
	virtual void			DrawBackground(BOOL blExportbmp = FALSE);
	virtual	void			DrawEntire(BOOL blRedraw = FALSE);
	virtual void			DrawEntity(CCAD_DC *pCadDC);
	virtual CCAD_SpaceDoc*	GetSpaceDoc() { return (CCAD_SpaceDoc *) m_pPaperDoc; }
	virtual CAD_FLOAT		GetLimitScale(CCAD_DC *, CCAD_Camera *, CAD_FLOAT &, CAD_FLOAT &);
	virtual CAD_FLOAT		GetMaxLimit();
	void						DrawTempBuffer();

	CCAD_PaperDoc*	GetPaperDoc() {return m_pPaperDoc;}
	void			SetPaperDoc(CCAD_PaperDoc*	pDoc);
	
	void			DrawFrame();
	virtual BOOL	UpdateRenderGL(CCAD_DC *pCadDC);
	void			InitUpdateView();
    
	virtual void		AdjustCameraOrg(CCAD_DC * pDC, CCAD_Camera &, bool blCalcExt); // for Scroll.

	CCAD_Size				GetTotalSize();

	virtual float		GetScale();
	
// Implementation
public:
	virtual ~CCAD_LayoutView() {}

};


#endif
