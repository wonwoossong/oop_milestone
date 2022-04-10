#include <fstream>
#include <algorithm>
#include "Utilities.h"
#include "LineManager.h"

namespace sdds {

    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations)
    {
        std::ifstream file_(file);
        if (!file_) throw "Failed to open the file";
        else {
            Utilities utils;
            bool more;
            size_t npos = 0;

            std::string fileLine, currWSname, nxtWSname;

            Workstation* nxtWS = nullptr;
            Workstation* currWS = nullptr;
            Workstation* firstWS = nullptr;
            while (std::getline(file_, fileLine))
            {
                currWSname = utils.extractToken(fileLine, npos, more);
                currWS = *std::find_if(stations.begin(), stations.end(), [&](Workstation* ws) {
                    return ws->getItemName() == currWSname;
                    });
                m_activeLine.push_back(currWS);

                if (more) {
                    nxtWSname = utils.extractToken(fileLine, npos, more);
                    nxtWS = *std::find_if(stations.begin(), stations.end(), [&](Workstation* ws) {
                        return ws->getItemName() == nxtWSname;
                        });
                    currWS->setNextStation(nxtWS);
                }
            }

            for_each(stations.begin(), stations.end(), [&](Workstation* tmp) {
                firstWS = *find_if(stations.begin(), stations.end(), [&](Workstation* station) {
                    return station->getNextStation() == firstWS;
                    });
                });

            m_firstStation = firstWS;
        }
        file_.close();
        m_cntCustomerOrder = g_pending.size();
    }

    void LineManager::reorderStations()
    {
        const Workstation* ws = m_firstStation;
        size_t i{ 0 };
        do
        {
            m_activeLine[i++] = const_cast<Workstation*>(ws);
            ws = ws->getNextStation();
        } while (ws != nullptr);
    }

    bool LineManager::run(std::ostream& os) {
        static size_t iterations = 0;
        os << "Line Manager Iteration: " << ++iterations << std::endl;

        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        std::for_each(m_activeLine.begin(), m_activeLine.end(), [&os](Workstation* ws) {
            ws->fill(os);
            });
        std::for_each(m_activeLine.begin(), m_activeLine.end(), [](Workstation* ws) {
            ws->attemptToMoveOrder();
            });

        return m_cntCustomerOrder == (g_completed.size() + g_incomplete.size());
    }

    void LineManager::display(std::ostream& os) const
    {
        if (!m_activeLine.empty())
            for (auto& i : m_activeLine)
                i->display(os);
    }
}