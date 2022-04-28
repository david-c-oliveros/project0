#ifndef TICKTIMER_H
#define TICKTIMER_H


class TickTimer
{
    private:
        int m_iTicks;
        int m_iTickMax;
        bool m_bRunning;

    public:
        TickTimer(int iTickMax);
        ~TickTimer();

        void Start();
        void Reset();
        void ChangeMax(int iNewTickMax);
        void Update();
        bool Check();
};

#endif
