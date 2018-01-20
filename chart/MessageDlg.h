#pragma once

#include <windows.h>
#include <string>


class MessageDlg
{
	std::wstring header;
	std::wstring text;
	UINT type;
public:
	MessageDlg(std::wstring Header, std::wstring Text, UINT Type=MB_OK)
		: header (Header), text(Text), type(Type)
	{
	}

	void Show()
	{
		MessageBox(0, text.c_str(), header.c_str(), type);
	}
};