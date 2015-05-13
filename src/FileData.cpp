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

#include <FileData.hpp>

#include <fstream>
#include <iostream>

namespace
{
    const int maxFileSize = 1024 * 1024; //never read more than 1MB
}


FileData::FileData(){}

FileData::FileData(FileData&& other)
{
    m_data = std::move(other.m_data);
}

FileData& FileData::operator=(FileData&& other)
{
    m_data = std::move(other.m_data);
    return *this;
}

//public
bool FileData::open(const std::string& path)
{
    std::fstream file;
    file.open(path, std::ios::in | std::ios::binary);

    if (file.fail())
    {
        std::cout << "Failed opening file " << path << std::endl;
        file.close();
        return false;
    }

    file.seekg(0, std::ios::end);
    int fileSize = static_cast<int>(file.tellg());
    file.seekg(0, std::ios::beg);

    if (fileSize == 0)
    {
        std::cout << "File " << path << " returned 0 file size" << std::endl;;
        file.close();
        return false;
    }

    fileSize = (fileSize > maxFileSize) ? maxFileSize : fileSize;

    m_data.resize(fileSize);
    file.read((char*)(m_data.data()), fileSize);
    file.close();

    std::cout << "Loaded " << fileSize << " bytes." << std::endl;

    return true;
}

const std::vector<byte>& FileData::getData() const
{
    return m_data;
}

//private