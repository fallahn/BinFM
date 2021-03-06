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

//file validation class

#ifndef FILEDATA_HPP_
#define FILEDATA_HPP_

#include <string>
#include <vector>

using byte = unsigned char;

class FileData final
{
public:
    FileData();
    ~FileData() = default;

    FileData(FileData&&);
    FileData& operator = (FileData&&);


    bool open(const std::string& path);
    const std::vector<byte>& getData() const;

private:

    std::vector<byte> m_data;
};

#endif //FILEDATA_HPP_