#pragma once
#pragma warning( disable: 4244; disable: 4996 )

#include <afxtempl.h>
#include "ADOConnection.h"
#include "Query.h"
#include "Common.h"
#include "DebugWnd.h"
#include "Tags.h"
#include "GlobalLib.h"
#include "CommondbGlobal.h"
#include "Param.h"
#include "HtmlDlg.h"


#define VERSION_SYS		L"0"
#define VERSION			L"0001"

//////////////////////////////////////////////////////
//  application globals
extern CString g_def_dep_out;
//////////////////////////////////////////////////////

#define WM_RELOAD_LIST		(WM_USER+1098)

#define INFO_UNDEFINED	-2

#define REGKEY_CURR_DATE	_T("CurrDate")
#define REGKEY_PO_DATE		_T("PODate")
#define REGKEY_Out_DATE		_T("OutDep")


#define LABEL_COLOR_BLUE			RGB(0x00, 0x14, 0xC8)
#define LABEL_COLOR_RED				RGB(0xD7, 0x00, 0x00)
//////////////////////////////////////////////////////
//  global definitions

#define DIALOG_BKCOLOR			RGB(0xEE, 0xEE, 0xEE)
#define REBAR_BKCOLOR			RGB(0xEE, 0xEE, 0xEE)
#define GRID_BKCOLOR			RGB(0xFE, 0xFE, 0xFE)
#define FRAME_SEPARATOR_COLOR	RGB(0x86, 0xA5, 0xEC)
#define POPUPMENU_BKCOLOR		RGB(0x00, 0x00, 0xFF)
#define WHITE_COLOR				RGB(0xFF, 0xFF, 0xFF)
#define SELECT_COLOR			RGB(0xFF, 0x00, 0x00)
#define GANTHDR_BKCOLOR			RGB(0xFF, 0xFF, 0xDE)
#define GANTNEW_COLOR			RGB(0xF0, 0xF0, 0xF0)
#define GANTLINE_COLOR			RGB(0x86, 0xA5, 0xEC)
#define DGANTRECT_COLOR			RGB(0x09, 0xB8, 0xF9)
#define OGANTRECT_COLOR			RGB(0x50, 0xFF, 0x0B)
#define DGANTLIGHT_BKCOLOR		RGB(0xE6, 0xFF, 0xEA)
#define OGANTLIGHT_BKCOLOR		RGB(0xF0, 0xF8, 0xFF)
#define DGANTDARK_BKCOLOR		RGB(0xCF, 0xFF, 0xD7)
#define OGANTDARK_BKCOLOR		RGB(0xE2, 0xF1, 0xFF)
#define OGANT_BKCOLOR			RGB(0xF0, 0xF8, 0xFF)
#define DGANT_BKCOLOR			RGB(0xF0, 0xF8, 0xFF)
#define NEW_NEW_COLOR				RGB(0x11, 0x2C, 0x1A)

#define IDC_AGRID				128
#define ERR_VALUE				0xFFFFFFFF
//#define TOOLBAR_STR				_T("Операционный день - таблица\0Операционный день - график\0Отчёты\0")


#define RIGHT_MOVING_SHOW_MANIPULATION  11028  // Показывать вкладку "Врач. сестринские манипуляции"
#define RIGHT_MOVING_SHOW_DIAGNOSTIC    11029  // Показывать вкладку "Диагностика"
#define RIGHT_MOVING_SHOW_LABORATORY	11030  // Показывать вкладку "Лаборатория"
#define RIGHT_MOVING_SHOW_OPERS			11031  // Показывать вкладку "Операции"
#define RIGHT_MOVING_SHOW_FISIO			11032  // Показывать вкладку "ФТО, ЛФК..."
#define RIGHT_MOVING_SHOW_ASSIGNMENT	11033  // Показывать вкладку "Назначения"
#define RIGHT_MOVING_SHOW_DIAGNOSES		11034  // Показывать вкладку Диагнозы
#define RIGHT_MOVING_SHOW_PROTOCOL_OUT	11035  // Показывать вкладку Протоколы (выбытия)
#define RIGHT_MOVING_SHOW_DIET			11036  // Показывать вкладку "Диет. питание"
#define RIGHT_MOVING_SHOW_PROTOCOL_ADD	11037  // Показывать вкладку "Доп. протоколы"
#define RIGHT_MOVING_SHOW_TEMP_LIST		11038  // Показывать вкладку "Температурный лист"
#define RIGHT_MOVING_SHOW_PERINATAL		11039  // Показывать вкладку "Роды", ведение родов в режиме "Перевод пациентов"
#define RIGHT_MOVING_SHOW_MEDICINE		11040  // Показывать вкладку "Медикаменты и расходники"
#define RIGHT_MOVING_SHOW_PERS_MEDICINE	11044  // Показывать вкладку "Личные медикаменты"


//////////////////////////////////////////////////////
//  global variables

extern CADOConnection*	g_lpConn;
extern CString			CONN_STRING;

/*
struct DepInfo {
long KeyID;
char Text[255];
};

extern DepInfo g_DepInfo;
*/

//////////////////////////////////////////////////////
// String routine
void CheckStrLength(CString& str);
CString DateToStr(DATE dt);
CString OleDateTimeToStr(COleDateTime& dt);
CString GetDayChangeTime();
CString GetStacOutTime();
//////////////////////////////////////////////////////
//  global structures
typedef struct _OPERRES
{
	int     KeyID;
	CString Code;
	CString Text;
} OperRes, *lpOperRes;

typedef struct _OPERRECT
{
	int	KeyID;
	int BgnTime;
	int EndTime;
	int NumRow;
	int Level;
	CRect Rect;
} OperRect, *lpOperRect;

typedef enum _BORDERTRG { BRDTRG_LEFT, BRDTRG_RIGHT } BorderTrg;
typedef enum _STATUSOPERCARD { OCST_NONE, OCST_INSERT, OCST_DELETE } StatusOperCard;

typedef struct _IDNAME
{
	int ID;
	int Code;
	CString Name;
	StatusOperCard Status;
} IDName, *lpIDName;

typedef struct _OPERCARD
{
	int    KeyID;
	IDName OperDep;
	IDName Patient;
	CArray<IDName, IDName&>	arrOpers;
	CArray<IDName, IDName&>	arrMainDocs;
	CArray<IDName, IDName&>	arrAnestDocs;
	COleDateTime BgnTime;
	COleDateTime EndTime;
	BOOL UpdateFlag;
	_OPERCARD& operator = (_OPERCARD &other)
	{
		KeyID = other.KeyID;
		OperDep = other.OperDep;
		Patient = other.Patient;
		arrOpers.RemoveAll();
		arrOpers.Copy(other.arrOpers);
		arrMainDocs.RemoveAll();
		arrMainDocs.Copy(other.arrMainDocs);
		arrAnestDocs.RemoveAll();
		arrAnestDocs.Copy(other.arrAnestDocs);
		BgnTime = other.BgnTime;
		EndTime = other.EndTime;
		UpdateFlag = other.UpdateFlag;
		return *this;
	}
} OperCard, *lpOperCard;


extern CString gDayChangeTime;
extern CString gStacOutTime;