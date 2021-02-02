#include <limits>
#include <numeric>
#include <boost/circular_buffer.hpp>
#include <cmath>

namespace ndn {
namespace nfd {

template <typename T>
class BufferedCounter
{
private:
    boost::circular_buffer<T> m_buffer;

public:
    explicit BufferedCounter(int unsigned size) : m_buffer(size){}

    void
    push(const T &value)
    {
        m_buffer.push_front(value);
    }

    T
    min() const
    {
        auto it = std::min_element(m_buffer.cbegin(), m_buffer.end());
        if (it == m_buffer.end())
        {
            return std::numeric_limits<T>::max();
        }
        else
        {
            return *it;
        }
    }

    T
    max() const
    {
        auto it = std::max_element(m_buffer.cbegin(), m_buffer.end());
        if (it == m_buffer.end())
        {
            return std::numeric_limits<T>::max();
        }
        else
        {
            return *it;
        }
    }

    float
    average() const
    {
        float counter = std::accumulate(m_buffer.cbegin(), m_buffer.end(), 0.0);
        return counter / m_buffer.size();
    }

    float
    std_dev() const
    {
        float avg = average();
        auto op = [avg](float counter, float operand) {
            return std::move(counter) + std::pow(operand - avg, 2);
        };
        float sum_of_squares = std::accumulate(m_buffer.cbegin(), m_buffer.end(), 0.0, op);
        return sqrt(sum_of_squares / (m_buffer.size() - 1));
    }
};

} // namespace nfd
} // namespace ndn
