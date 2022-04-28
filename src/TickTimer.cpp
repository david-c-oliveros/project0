#include "TickTimer.h"


TickTimer::TickTimer(int iTickMax)
    : m_iTickMax(iTickMax), m_bRunning(false), m_iTicks(0)
{
}


TickTimer::~TickTimer()
{
}


void TickTimer::Start()
{
    m_bRunning = true;
}


void TickTimer::Reset()
{
    m_bRunning = false;
    m_iTicks = 0;
}


void TickTimer::ChangeMax(int iNewTickMax)
{
    m_iTickMax = iNewTickMax;
}


void TickTimer::Update()
{
    if (m_bRunning)
    {
        m_iTicks++;
    }
}


bool TickTimer::Check()
{
    if (m_iTicks >= m_iTickMax)
        return true;
    else
        return false;
}
