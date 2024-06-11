#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include "Walnut/Image.h"

class Renderer
{
public:

    Renderer() = default;
    void Resize(uint32_t width, uint32_t height);
    void Render();
    [[nodiscard]] std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private:

    static uint32_t PerPixel(glm::vec2 coordinate);

    std::shared_ptr<Walnut::Image> m_FinalImage;
    uint32_t * m_ImageData = nullptr;

};