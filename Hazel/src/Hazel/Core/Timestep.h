#pragma once

namespace Hazel {

    class Timestep
    {
    public:
        Timestep(float time = 0.0f)
            : m_Time(time)
        {
        }

        //This is adding a cast operator
        // float time = ts.
        operator float() const { return m_Time; }
        //We can overload the + operator to have something more concise when working with time.
        float GetSeconds() const { return m_Time; }
        float GetMilliseconds() const { return m_Time * 1000.0f; }
    private:
        float m_Time;
    };

}
