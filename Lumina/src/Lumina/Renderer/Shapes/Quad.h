#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../VertexArray.h"
#include "../../Core/Aliases.h"

namespace Lumina 
{


class Quad 
{
    public:
        Quad();
        ~Quad() = default;

        const Lumina::Shared<Lumina::VertexArray>& GetVertexArray() { return m_VertexArray; }
    private:
        Lumina::Shared<Lumina::VertexArray> m_VertexArray;
    };
}