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
#include <algorithm>

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

    const u16 duration = 48;

    const std::size_t maxSegmentSize = 1200;
    const std::size_t maxSegmentCount = 32;

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

bool Modulator::modulate(const std::vector<byte>& data, const std::string& fileName)
{
    //check if wave tables built and create if not
    if (lowTable.empty() || highTable.empty())
    {
        buildWaveTable(lowTable, lowFreq);
        buildWaveTable(highTable, highFreq);
    }
    
    //create ostream / check valid
    std::fstream file(fileName, std::ios::binary | std::ios::out);
    if (!file.good() || !file.is_open())
    {
        std::cout << "Failed to open file for writing" << std::endl;
        file.close();
        return false;
    }
    std::cout << "Processing..." << std::endl;

    //test data size and limit if necessary
    auto dataSize = data.size() * highTable.size();
    auto segmentCount = std::min(dataSize / maxSegmentSize, maxSegmentCount);
    dataSize = segmentCount * maxSegmentSize * 8 * highTable.size();

    //create header
    WavHeader header;

    //add to header / update header data count - riffChuknSize and dataChunkSize
    header.riffChunkSize += dataSize;
    header.dataChunkSize = dataSize;

    file.write((const char*)&header, sizeof(header));

    //output segments
    for (auto i = 0; i < segmentCount; ++i)
    {
        std::vector<short> outData(maxSegmentSize * 8 * highTable.size());
        auto ptr = outData.data();

        for (auto j = 0u; j < maxSegmentSize; ++j)
        {
            for (auto k = 0; k < 8; ++k)
            {
                if (data[j] & (1 << k))
                {
                    std::memcpy((void*)ptr, lowTable.data(), lowTable.size() * sizeof(short));
                    ptr += lowTable.size();
                }
                else
                {
                    std::memcpy((void*)ptr, highTable.data(), highTable.size() * sizeof(short));
                    ptr += highTable.size();
                }
            }
        }
        file.write((const char*)outData.data(), outData.size() * sizeof(short));
    }

    file.close();

    std::cout << "wrote " << dataSize << "bytes" << std::endl;

    return true;
}