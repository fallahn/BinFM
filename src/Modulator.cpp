/*********************************************************************
Matt Marchant 2015
http://trederia.blogspot.com

Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include <Modulator.hpp>

#include <fstream>
#include <iostream>

/*
48khz mono
square wave
16bit signed data
*/

namespace
{
    const u16 sampleRate = 48000u;

    const u16 lowFreq = 1200u;
    const u16 highFreq = 2400u;
    const int maxVol = 30000;

    const u16 duration = 4800;

    //wave tables
    std::vector<short> lowTable;
    std::vector<short> highTable;

    void buildWaveTable(std::vector<short>& dst, u16 freq)
    {
        int sampleCount = sampleRate / freq;
        int cycleCount = duration / sampleCount;

        for (auto i = 0; i < cycleCount; ++i)
        {
            for (auto j = 0; j < sampleCount; ++j)
            {
                dst.push_back((j < sampleCount / 2) ? maxVol : -maxVol);
            }
        }
    }
}

bool Modulator::modulate(const std::vector<byte>& data)
{
    //check if wave tables built and create if not
    if (lowTable.empty() || highTable.empty())
    {
        buildWaveTable(lowTable, lowFreq);
        buildWaveTable(highTable, highFreq);
    }
    
    //create ostream / check valid
    std::fstream file("test.wav", std::ios::binary | std::ios::out);
    if (!file.good() || !file.is_open())
    {
        std::cout << "Failed to open file for writing" << std::endl;
        file.close();
        return false;
    }

    //create header
    WavHeader header;

    //process data
    std::vector<short> ndata(200 * lowTable.size());
    auto ptr = ndata.data();
    for (int i = 0; i < 200; ++i)
    {
        std::memcpy((void*)ptr, lowTable.data(), lowTable.size());
        ptr += lowTable.size();
    }

    //add to header / update header data count - riffChuknSize and dataChunkSize
    header.riffChunkSize += sizeof(lowTable);
    header.dataChunkSize = sizeof(lowTable);

    //output via stream
    file.write((const char*)&header, sizeof(header));
    file.write((const char*)ndata.data(), sizeof(data));

    file.close();

    return true;
}