
#include <complex.h>
#include "Utilities.h"
#include <iostream>

namespace sdds
{

    char Utilities::m_delimiter = ' ';


    void Utilities::setFieldWidth(size_t newWidth)
    {
        m_widthField = newWidth;
    }
    size_t Utilities::getFieldWidth() const
    {
        return m_widthField;
    }
    std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more)
    {
        std::string result = "";
        size_t first_pos = next_pos;

        next_pos = str.find(m_delimiter, first_pos);

        if (first_pos == next_pos)
        {
            more = false;
            throw "   ERROR: No token.\n";
        }
        else if (next_pos == std::string::npos)
        {
            result = str.substr(first_pos);
            m_widthField = std::max(result.length(), m_widthField);
            more = false;
        }
        else
        {
            result = str.substr(first_pos, next_pos - first_pos);
            result.erase(0, result.find_first_not_of(" "));
            result.erase(result.find_last_not_of(" ") + 1);
            m_widthField = std::max(result.length(), m_widthField);
            more = true;
        }

        next_pos++;

        return result;
    }
    void Utilities::setDelimiter(char newDelimiter)
    {
        m_delimiter = newDelimiter;
    }
    char Utilities::getDelimiter()
    {
        return m_delimiter;
    }
}