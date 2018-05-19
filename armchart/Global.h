#pragma once
#pragma warning( disable: 4244; disable: 4996 )

#include <afxtempl.h>
#include "ADOConnection.h"
#include "Query.h"
#include "tags.h"
#include "Common.h"
#include "DebugWnd.h"
#include "AppString.h"
#include "ComboBoxGrid.h"
#include "Tables.h"
#include "resource_add.h"

#include "CommondbGlobal.h"

//#define DATE_STR_FORMAT     "%Y-%m-%d"
//#define DATETIME_STR_FORMAT "%Y-%m-%d %H:%M:%S"
//#define DATE_DISPLAY_FORMAT "%d-%m-%Y"


extern BOOL g_OperCardAnestHide;
extern BOOL g_OperCardDiagnosHide;
extern BOOL g_OperCardComplexShow;
extern BOOL g_OperCardMacroHide;
extern BOOL g_OperCardOperDoctorTimeHide;
extern BOOL g_OperCardNoteShow;
