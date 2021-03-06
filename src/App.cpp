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

#include <App.hpp>
#include <FileData.hpp>
#include <Modulator.hpp>

#include <iostream>

int App::run(int argsc, char** argsv)
{
    if (argsc <= 1)
    {
        std::cout << "Not enough arguments, please supply file path" << std::endl;
        return 1;
    }
    
    FileData fd;
    if (!fd.open(std::string(argsv[1])))
    {
        return 2;
    }

    std::cout << "Enter file name:" << std::endl;
    std::string fileName;
    std::cin >> fileName;
    if (fileName.empty()) fileName = "default.wav";
    if (fileName.find_last_of(".wav") != fileName.size() - 1) fileName += ".wav";

    if (!Modulator::modulate(fd.getData(), fileName))
    {
        return 3;
    }

    return 0;
}