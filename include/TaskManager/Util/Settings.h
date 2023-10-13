#pragma once

class Settings
{
public:
    /// @brief Time a frame will take (minimally)
    static const int FrameTime_Ms = 10;

    /// @brief After how many frames a poll is done
    static const int PollFrequencyFrameCount = 100;

    /// @brief How many data points are shown in the CPU graphs
    static const int CpuBufferGraphSize = 10;
};