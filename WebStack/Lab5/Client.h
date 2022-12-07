#pragma once

#pragma once

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <iostream>
#include "Renderer.h"


class CClient
{

public:

    void Render(IRenderer* renderer)
    {
        std::string output = renderer->Render();

        std::cout << output << std::endl;

    }

};


#endif // __CLIENT_H__
