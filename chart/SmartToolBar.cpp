#include "stdafx.h"
#include "SmartToolBar.h"
#include "ugc.h"



BEGIN_MESSAGE_MAP(CSmartToolBar, CToolBarBase)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


//-------------------------------------------------------------------------

void CSmartToolBar::addButton(CWnd* parent, const wstring& text, const function<void()>& Func, int group)
{
	UGC ugc(GetDC(), Width, Height);
	DPIX dpix;
	int border = dpix(2);
	int x = border;
	int b_w = (params && ST_FILL_ALL_WIDTH) ? Width : ((text.length()!=0) ? ugc.GetTextWidth(text, 11)+border*4 : dpix(BUTTON_MIN_WIDTH));
	b_w -= border * 2;

	if (buttons.size() != 0)
	{
		int count = static_cast<int>(buttons.size());
		if (params && ST_FILL_ALL_WIDTH)
		{
			b_w = (Width - border * 2) / (1 + count);
			for (int i = 0; i < count; i++)
			{
				::SetWindowPos(GetDlgItem(2000 + i)->m_hWnd, HWND_TOP,
					i*b_w + border,
					border,
					b_w,
					Height - border * 2, NULL);

			}
			x = count * b_w + border;
		}
		else
		{
			
			for (const auto& b : buttons)
				x += b->getWidth();
			x += border;
		}
		
	}

	CRect rect;
	rect.top = border;
	rect.bottom = Height-border;
	rect.left = x;
	rect.right = x + b_w;
	buttons.push_back(CSmartButton_Ptr(new CSmartButton(this, nextButtonID, text, rect, Func, group)));

	nextButtonID++;
}

void CSmartToolBar::OnPaint()
{
	CWnd::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 255, 135);

	ugc.Clear();
}


void CSmartToolBar::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(&rect);
	Width = rect.Width();
	Height = rect.Height();
	CWnd::OnSize(nType, cx, cy);
}


void CSmartToolBar::setPressed(int button_index)
{
	int count = static_cast<int>(buttons.size());
	if (button_index >= count)
		return;
	CSmartButton_Ptr& button = buttons[button_index];
	button->setPressed(true);
	int group = button->getGroup();
	for (int i = 0; i < count; i++)
	{
		if (i == button_index) continue;
		if (group == buttons[i]->getGroup())
			buttons[i]->setPressed(false);
	}
}


void CSmartToolBar::setEnabled(bool status)
{
	for (auto& b : buttons)
	{
		b->setEnabled(status);
	}
	RedrawWindow();
}