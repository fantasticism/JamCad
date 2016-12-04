#pragma once
//------------------------------------------

//------------------------------------------

typedef union {
	class CCadArc *pCadArc;
	class CCadArcCent *pCadArcCent;
	class CCadArcPcb *pCadArcPcb;
	class CCadArrow *pCadArrow;
	class CCadBitmap *pCadBitmap;
	class CCadBus *pCadBus;
	class CCadBusEntry *pCadBusEntry;
	class CCadDimension *pCadDimension;
	class CCadElispe *pCadElispe;
	class CCadFillArea *pCadFillArea;
	class CCadHoleRect *pCadHoleRect;
	class CCadHoleRnd1Flat *pCadHoleRnd1Flat;
	class CCadHoleRnd2Flat *pCadHoleRnd2Flat;
	class CCadHoleRound *pCadHoleRound;
	class CCadJunction *pCadJunction;
	class CCadLibComp *pCadLibComp;
	class CCadLine *pCadLine;
	class CCadObject *pCadObject;
	class CCadOrigin *pCadOrigin;
	class CCadPad *pCadPad;
	class CCadPin *pCadPin;
	class CCadPcbComp *pCadPcbComp;
	class CCadPolyFilled *pCadPolyFilled;
	class CCadPolygon *pCadPolygon;
	class CCadPower *pCadPower;
	class CCadRect *pCadRect;
	class CCadRndRect *pCadRndRect;
	class CCadSchPart *pCadSchPart;
	class CCadSheet *pCadSheet;
	class CCadSheetPin *pCadSheetPin;
	class CCadSheetPort *pCadSheetPort;
	class CCadText *pCadText;
	class CCadTrack *pCadTrack;
	class CCadVia *pCadVia;
	class CCadWire *pCadWire;
}CADObjectTypes;

//-----------------------------------------------------

#define MAX_ZOOM		7	//maximum number of zoom levels

#define RED(x)		((x & 0xff0000) >> 16)
#define GREEN(x)	((x & 0xff00) >> 8)
#define BLUE(x)		(x & 0x0ff)

//-----------------------------------
// Defines for drawing check selected
//-----------------------------------

enum enumDwgChkSel {
	CADDRAWUBG_CHECKSEL_FLAG_ALL,
	CADDRAWUBG_CHECKSEL_FLAG_UNSEL,
	CADDRAWUBG_CHECKSEL_FLAG_SEL
};

//----------------------------------
// Defines for Properties
//----------------------------------

enum enumROTATION {
	ROTATION_0,		//0 degrees
	ROTATION_90,	//90 degrees
	ROTATION_180,	//180 degrees
	ROTATION_270	//270 degrees
};

//----------------------------------
// Defines for parsers
//----------------------------------

enum enumTOKEN{
	TOKEN_NUM	=256,
	TOKEN_STRING,
	TOKEN_LINE,
	TOKEN_RECT,
	TOKEN_ELISPE,
	TOKEN_ROUNDRECT,
	TOKEN_POLYGON,
	TOKEN_POLYGONFILLED,
	TOKEN_LIBPART,
	TOKEN_HOLEROUND,
	TOKEN_HOLERECT,
	TOKEN_HOLERND1FLAT,
	TOKEN_HOLERND2FLAT,
	TOKEN_TEXT,
	TOKEN_ARC,
	TOKEN_ARCCENTERED,
	TOKEN_BITMAP,
	TOKEN_ARROW,
	TOKEN_ORG,
	TOKEN_DIMENSION,
	TOKEN_WIRE,
	TOKEN_BUS,
	TOKEN_SCH_PART,
	TOKEN_POWER,
	TOKEN_SHEET,
	TOKEN_SHEETPORT,
	TOKEN_SHEETPIN,
	TOKEN_BUSENTRY,
	TOKEN_JUNCTION,
	TOKEN_PIN,
	TOKEN_COLOR,
	TOKEN_BKCOLOR,
	TOKEN_FILLCOLOR,
	TOKEN_WIDTH,
	TOKEN_POINT,
	TOKEN_SIZE,
	TOKEN_FIELD,
	TOKEN_PROPERTY,
	TOKEN_SCHEMATIC,
	TOKEN_DRAWING,
	TOKEN_RED,
	TOKEN_GREEN,
	TOKEN_BLUE,
	TOKEN_RADIUS,
	TOKEN_ROTATION,		//Property Selector Type
	TOKEN_PINTYPE,		//Property Selector Type
	TOKEN_PINSTYLE,		//property selector type
	TOKEN_PINROTATION,	//property selector type
	TOKEN_PINLENGTH,	//Property Int Type
	TOKEN_PINNUMBER,	//Property String Type
	TOKEN_PINNAME,		//Property String Type
	TOKEN_PARTNAME,		//Property String Type
	TOKEN_REFDESIGNATOR,//Property Strying Type
	TOKEN_NUMUNITS,		//Property Int Type
	TOKEN_REFNUMBER,	//property Int Type
	TOKEN_SCHLIBNAME,	//proterty String Type
	TOKEN_SCHLIBRARY,
	TOKEN_INTFIELD,
	TOKEN_SCHPART,
	TOKEN_STRINGFIELD,
	TOKEN_LENGTH,
	TOKEN_FILE,
	TOKEN_INTSELFIELD,
	TOKEN_VERTEX,
	TOKEN_FONT,
	TOKEN_WEIGHT,
	TOKEN_HEIGHT,
	TOKEN_ANGLE,
	TOKEN_TRANSPARENT,
	TOKEN_STRINGSELFIELD,
	TOKEN_SELECTION,
	TOKEN_VALUE,
	TOKEN_SELTITLES,
	TOKEN_DISPLAY,
};

typedef struct {
	int m_Token;
	char *m_pName;
}KEYWORD;

//----------------------------------
// CCadObject derived types
//----------------------------------

enum enumOBJECTTYPE{
	OBJECT_TYPE_BASE		=0,
	OBJECT_TYPE_LINE,			//drawing objects
	OBJECT_TYPE_RECT,
	OBJECT_TYPE_ELISPE,
	OBJECT_TYPE_ROUNDEDRECT,
	OBJECT_TYPE_POLYGON,
	OBJECT_TYPE_POLYGONFILLED,
	OBJECT_TYPE_LIBCOMP	,	
	OBJECT_TYPE_HOLEROUND	,	
	OBJECT_TYPE_HOLERECT	,	
	OBJECT_TYPE_HOLERND1FLAT,	
	OBJECT_TYPE_HOLERND2FLAT,	
	OBJECT_TYPE_TEXT		,	
	OBJECT_TYPE_ARC	,			
	OBJECT_TYPE_ARCCENTERED		,
	OBJECT_TYPE_BITMAP	,		
	OBJECT_TYPE_ARROW	,		
	OBJECT_TYPE_ORIGIN		,	
	OBJECT_TYPE_DIMENSION	,	
//----------------------------------------------------------
	OBJECT_TYPE_WIRE		=64,	//electronic "sch" objects
	OBJECT_TYPE_BUS,
	OBJECT_TYPE_SCH_PART,
	OBJECT_TYPE_POWER,
	OBJECT_TYPE_SHEET,
	OBJECT_TYPE_SHEETPORT,
	OBJECT_TYPE_SHEETPIN,
	OBJECT_TYPE_BUSENTRY,
	OBJECT_TYPE_JUNCTION,
	OBJECT_TYPE_PIN,
//----------------------------------------------------------
	OBJECT_TYPE_TRACK		=128,	//pc board objects
	OBJECT_TYPE_PCBARC,
	OBJECT_TYPE_PAD,
	OBJECT_TYPE_FILLAREA,
	OBJECT_TYPE_VIA,
	OBJECT_TYPE_PCBCOMP,
//----------------------------------------------------------
	OBJECT_TYPE_PROPERTY = 192,		//property objects
//---------------------------------------------------------
	OBJECT_TYPE_SCHLIB = 224,		//Document Types
	OBJECT_TYPE_SCH,
	OBJECT_TYPE_PCBLIB,
	OBJECT_TYPE_PCB,
	OBJECT_TYPE_FRONTCADLIB,
	OBJECT_TYPE_FRONTCAD
};

//--------------------------------------------
// DRAW mode defines
//--------------------------------------------
enum enumOBJECTMODE{
	OBJECT_MODE_FINAL=0,
	OBJECT_MODE_SELECTED,
	OBJECT_MODE_SKETCH,
	OBJECT_MODE_ARCSTART,
	OBJECT_MODE_ARCEND
};

enum enumDRAWMODE{
	DRAWMODE_SELECT = 0,
	DRAWMODE_LINE,
	DRAWMODE_RECT,
	DRAWMODE_ELIPSE,
	DRAWMODE_RNDRECT,
	DRAWMODE_ARC,
	DRAWMODE_POLYGON,
	DRAWMODE_POLYFILL,
	DRAWMODE_ARC_CENTER,
	DRAWMODE_HOLE_ROUND,
	DRAWMODE_HOLE_RECT,
	DRAWMODE_HOLE_RND1F,
	DRAWMODE_HOLE_RND2F,
	DRAWMODE_TEXT,
	DRAWMODE_LIBPART,
	DRAWMODE_GETREF,
	DRAWMODE_MOVE,
	DRAWMODE_COPY,
	DRAWMODE_CUT,
	DRAWMODE_PASTE,
	DRAWMODE_SELECTREGION,
	DRAWMODE_BITMAP,
	DRAWMODE_ARROW,
	DRAWMODE_ORIGIN,
	DRAWMODE_DIMENSION,
	DRAWMODE_ALIGN_DIM_TO_HOLE,
	DRAWMODE_PRINTRECT,
	DRAWMODE_PIN
};

enum enumDRAWSTATE{
	DRAWSTATE_SET_ATTRIBUTES,
	DRAWSTATE_WAITFORMOUSE_DOWN,
	DRAWSTATE_MOVE,
	DRAWSTATE_PLACE,
	DRAWSTATE_PLACE_AUTO,
	DRAWSTATE_ARCSTART,
	DRAWSTATE_ARCEND,
	DRAWSTATE_GETREFEREMCE,
	DRAWSTATE_DRAG,
	DRAWSTATE_SELECT_NOTHING,	//nothing is selected
	DRAWSTATE_SELECT_MOVE,
	DRAWSTATE_SELECT_COPY,
};

//---------------------------------------------------------
// ruler types
//----------------------------------------------------------
#define RT_VERTICAL	  0
#define RT_HORIZONTAL 1

// hint information
#define RW_VSCROLL		0x01
#define RW_HSCROLL		0x02
#define RW_POSITION		0x04
#define RW_ZOOM			0x08
#define RW_SETLABELS	0x10

#define RULER_SIZE    16

struct stRULER_INFO {
	UINT   uMessage;
	CPoint ScrollPos;
	CPoint Pos;
	CSize  DocSize;
	char **ppLabels;
	int nLabels;
	double  fZoomFactor;
};

//--------------------------------------------
// Windows User Messages
//--------------------------------------------

//-------------------------
// Arrow Attributes Dialog
//-------------------------

#define WM_ARROWATTRB	WM_USER+1

#define	WM_ARROWATTRB_VALUE_L1	0
#define WM_ARROWATTRB_VALUE_L2	1
#define WM_ARROWATTRB_VALUE_W	2

//------------------------
// Pin Properites Dialog
//------------------------

#define WM_PINPROP		WM_USER+2

#define WM_PINPROP_PINLEN	0

//-------------------------
// CAutoScrollView Messages
//-------------------------

#define WM_STOPAUTOSCROLL	WM_USER+3

//--------------------------------------------
// Misc Functions
//--------------------------------------------
extern char *TokeToString(int Token);
extern double ArcTan(double X, double Y);
extern int CheckAngle(double Start, double End, double Angle);
