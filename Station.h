
#include <string>
#ifndef SDDS_STATION_H
#define SDDS_STATION_H

namespace sdds {

	class Station {
		size_t m_stationid{ 0 };
		std::string m_itemname{ "" };
		std::string m_stationdesc{ "" };

		size_t m_serialnum{ 0u };
		size_t m_stockitem{ 0u };

		static size_t m_widthField;
		static size_t id_generator;
	public:
		Station(const std::string& line);
		const std::string& getItemName() const;
		size_t getNextSerialNumber();
		size_t getQuantity() const;
		void updateQuantity();
		void display(std::ostream& os, bool full) const;
	};
}

#endif // !SDDS_STATION_H