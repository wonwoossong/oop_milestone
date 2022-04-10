
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <algorithm>
#include <string_view>
#include <iostream>
#include <iomanip>  
#include "Station.h"
#include "Utilities.h"
using namespace std;
namespace sdds {

	size_t Station::m_widthField = 0u;
	size_t Station::id_generator = 0u;

	Station::Station(const std::string& str)
	{
		m_stationid = id_generator;
		Utilities util;
		size_t next_pos = 0;
		bool more = true;
		std::string temp{ "" };

		try
		{
			m_itemname = util.extractToken(str, next_pos, more);
			m_serialnum = stoi(util.extractToken(str, next_pos, more));
			m_stockitem = stoi(util.extractToken(str, next_pos, more));
			m_widthField = max(util.getFieldWidth(), m_widthField);
			temp = util.extractToken(str, next_pos, more);
			temp.erase(0, temp.find_first_not_of(" "));
			m_stationdesc = temp;

		}
		catch (string& msg)
		{
			cout << msg;
		}
		id_generator++;
	}
	const std::string& Station::getItemName() const
	{
		return m_itemname;
	}
	size_t Station::getNextSerialNumber()
	{
		return m_serialnum++;
	}
	size_t Station::getQuantity() const
	{
		return m_stockitem;
	}
	void Station::updateQuantity()
	{
		if (m_stockitem > 0)
		{
			m_stockitem -= 1;
		}
	}
	void Station::display(std::ostream& os, bool full) const
	{
		os << std::right << std::setw(3) << std::setfill('0') << m_stationid + 1 << " | ";
		os << std::left << std::setw(Station::m_widthField + 1) << std::setfill(' ') << m_itemname;
		os << " | " << std::right << std::setw(6) << std::setfill('0') << m_serialnum << " | ";

		if (full) {
			os << std::right << std::setw(4) << std::setfill(' ') << m_stockitem << " | ";
			os << m_stationdesc << std::endl;
		}
		else
			os << std::endl;
	}
}