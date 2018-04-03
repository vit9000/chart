#pragma once
#include <mysql.h>
#include "DrugInfo.h"
#include "utils.h"

class SQL
{
protected:
	MYSQL *connection, mysql;
	MYSQL_RES *result = nullptr;
	bool connected = false;
public:
	

	bool Connect()
	{
		mysql_init(&mysql);
		connection = mysql_real_connect(&mysql, "mysql.vit9000.myjino.ru",
			"vit9000", "spider", "vit9000_drugstore", 3306, 0, 0);

		if (connection == NULL)
		{
			return false;
		}
		
		//mysql_query(connection, "SET NAMES utf8 COLLATE utf8_unicode_ci");
		mysql_query(connection, "SET NAMES 'cp1251'");
		connected = true;
		return true;
	}

	bool SendRequest(const wstring& request)
	{
		if (!connected)
		{
			if (!Connect())
			{
				MessageBox(0, L"Подключение к базе данных отсутствует. Проверьте соединение с сервером.", L"Ошибка соединения с БД", MB_ICONERROR | MB_OK);
				return false;
			}
		}
		/*if(!Connect())
		{
		mylog.print("MySQL Failed\n");
		return -1;
		}*/
		
		if (result != nullptr)
			mysql_free_result(result);
		int query_state = mysql_query(connection, static_cast<std::string>(StringConverter(request)).c_str());
		if (query_state != 0)
		{
			//MessageBox("Ошибка запроса в базу данных");
			return false;
		}
		result = mysql_store_result(connection);

		return true;
	}
	//--------------------------------------------------------------------------
	inline auto CountStrings()
	{
		
		return mysql_num_rows(result);
	}
	//--------------------------------------------------------------------------
	inline auto CountFields()
	{

		return mysql_num_fields(result);
	}
	//--------------------------------------------------------------------------
	vector<wstring> RecieveNextData()
	{
		vector<wstring> rows;
		if (!result) return rows;

		char** r = mysql_fetch_row(result);
		rows.clear();
		for(decltype(CountFields()) i=0; i<CountFields(); ++i)
		{
			rows.push_back(static_cast<wstring>(StringConverter(r[i])));
		}
		return rows;
	}
	auto getInsertID()
	{
		return mysql_insert_id(connection);
	}
	//--------------------------------------------------------------------------
	void Disconnect()
	{
		mysql_free_result(result);
		mysql_close(connection);
	}

};