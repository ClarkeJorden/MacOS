// CAD_Layer.h: interface for the CCAD_Layer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_LAYER_H)
#define AFX_CAD_LAYER_H


#include	"CAD_Style.h"
#include	"CAD_Pen.h"
#include	"CAD_Brush.h"

 

#define		LAYER_ON			0x0001
#define		LAYER_EDITABLE		0x0002
#define		LAYER_WORKABLE		0x0004

#define		LAYER_ENABLE		(LAYER_ON | LAYER_EDITABLE | LAYER_WORKABLE)


class   CCAD_Layer : public CCAD_Style {
public:
	WORD		m_wState;
	CCAD_Pen	m_cPen;
	CCAD_Brush	m_cBrush;

public:
	CCAD_Layer();
	CCAD_Layer(const CCAD_Layer *pStyle);
	CCAD_Layer & operator =(const CCAD_Layer &);

	BOOL	IsVisible() const;
	WORD	GetState(WORD wState) const;
	void	SetState(WORD wState, BOOL blSet = TRUE);
	
	virtual void	Init();
	virtual void	CopyFrom(const CCAD_Style *pStyle);
	virtual BOOL	Read(CCAD_File &cFile);
};

 

#endif
