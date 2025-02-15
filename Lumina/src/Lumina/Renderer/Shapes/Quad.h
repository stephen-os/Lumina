#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../VertexArray.h"
#include "../../Base.h"

namespace Lumina 
{


class Quad 
{
    public:
        Quad();
        ~Quad() = default;

        const Lumina::Ref<Lumina::VertexArray>& GetVertexArray() { return m_VertexArray; }
    private:
        Lumina::Ref<Lumina::VertexArray> m_VertexArray;
    };
}