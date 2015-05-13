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

//modulates a byte array into an fm output and writes it to disc

#include <vector>

#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

using byte = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;

class Modulator final
{
public:

    static bool modulate(const std::vector<byte>& data);

private:

    struct WavHeader
    {
        //riff chunk
        char riffChunkId[4];
        u32 riffChunkSize = 36; //TODO add total data size to this
        char riffFormat[4];
        //fmt chunk
        char fmtChunkId[4];
        u32 fmtChunkSize = 16u; //default PCM
        u16 fmtFormat = 1; //default PCM
        u16 fmtChannelCount = 1;
        u32 fmtSampleRate = 48000;
        u32 fmtByteRate = 48000 * 16 / 8;
        u16 fmtBlockAlignment = 2u;
        u16 fmtBitsPerSample = 16u;
        //data chunk
        char dataChunkId[4];
        u32 dataChunkSize = 0u; //samples * channels * bitspersample / 8
        //byte* data = nullptr;

        WavHeader()
        {
            //header strings are big endian
            riffChunkId[0] = 'R';
            riffChunkId[1] = 'I';
            riffChunkId[2] = 'F';
            riffChunkId[3] = 'F';

            riffFormat[0] = 'W';
            riffFormat[1] = 'A';
            riffFormat[2] = 'V';
            riffFormat[3] = 'E';

            fmtChunkId[0] = 'f';
            fmtChunkId[1] = 'm';
            fmtChunkId[2] = 't';
            fmtChunkId[3] = ' ';

            dataChunkId[0] = 'd';
            dataChunkId[1] = 'a';
            dataChunkId[2] = 't';
            dataChunkId[3] = 'a';
        }
    };

};

#endif //MODULATOR_HPP_