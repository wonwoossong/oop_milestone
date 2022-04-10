


#include <iostream>
#include <string>
#include <iomanip>
#include "CustomerOrder.h"
#include "Utilities.h"
#include "Station.h"

size_t CustomerOrder::m_widthField = 0;

CustomerOrder::CustomerOrder()
{
	m_name = "default";
	m_product = "default";
	m_cntItem = 0;
	m_listItem = nullptr;
}

CustomerOrder::CustomerOrder(const std::string& str)
{
	sdds::Utilities util;
	auto more = true;
	size_t next_pos = 0;

	m_name = util.extractToken(str, next_pos, more);
	m_product = util.extractToken(str, next_pos, more);
	m_cntItem = 0;

	auto start_pos = next_pos;

	while (more)
	{
		util.extractToken(str, next_pos, more);
		m_cntItem++;
	}

	m_listItem = new ItemInfo * [m_cntItem];

	more = true;

	for (size_t i = 0; i < m_cntItem; i++)
	{
		m_listItem[i] = new ItemInfo(util.extractToken(str, start_pos, more));
	}

	if (util.getFieldWidth() > m_widthField)
		m_widthField = util.getFieldWidth();
}


CustomerOrder::CustomerOrder(CustomerOrder&)
{
	std::string err_mess = "A CustomerOrder object should not allow copy operations";
	throw err_mess;
}


CustomerOrder::CustomerOrder(CustomerOrder&& co) noexcept
{
	*this = std::move(co);
}


CustomerOrder& CustomerOrder::operator=(CustomerOrder&& src) noexcept
{
	if (&src != this)
	{
		if (m_listItem)
			for (unsigned int i = 0; i < m_cntItem; i++)
				delete this->m_listItem[i];

		delete[] this->m_listItem;
		this->m_name = src.m_name;
		this->m_product = src.m_product;
		this->m_cntItem = src.m_cntItem;
		this->m_listItem = src.m_listItem;
		src.m_listItem = nullptr;
	}

	return *this;
}

CustomerOrder::~CustomerOrder()
{
	if (m_listItem)
		for (unsigned int i = 0; i < m_cntItem; i++)
			delete m_listItem[i];

	delete[] m_listItem;
	m_listItem = nullptr;
}

bool CustomerOrder::isItemFilled(const std::string& str) const
{
	for (unsigned int i = 0; i < m_cntItem; i++)
		if (m_listItem[i]->m_itemName == str)
			return m_listItem[i]->m_fillState;

	return true;
}

bool CustomerOrder::isOrderFilled() const
{
	for (unsigned int i = 0; i < m_cntItem; i++)
		if (!m_listItem[i]->m_fillState)
			return false;

	return true;
}

void CustomerOrder::fillItem(sdds::Station& station, std::ostream& os) const
{
	for (unsigned int i = 0; i < m_cntItem; i++)
	{
		if (m_listItem[i]->m_itemName == station.getItemName())
		{
			if (station.getQuantity() <= 0)
				os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_listItem[i]->m_itemName << "]" << std
				::endl;
			else
			{
				m_listItem[i]->m_serialNumber = station.getNextSerialNumber();
				station.updateQuantity();
				m_listItem[i]->m_fillState = true;
				os << "    Filled " << m_name << ", " << m_product << " [" << m_listItem[i]->m_itemName << "]" << std::endl;
			}

			return;
		}
	}
}

void CustomerOrder::display(std::ostream& os) const
{
	os << m_name << " - " << m_product << std::endl;
	for (unsigned int i = 0; i < m_cntItem; i++)
	{

		os << "[" << std::setw(6) << std::setfill('0') << m_listItem[i]->m_serialNumber
			<< std::setfill(' ') << "] " << std::left << std::setw(m_widthField + 2) << m_listItem[i]->m_itemName
			<< " - " << (m_listItem[i]->m_fillState ? "FILLED" : "TO BE FILLED") << std::endl;
	}
}