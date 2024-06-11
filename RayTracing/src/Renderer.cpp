#include "Renderer.h"

#include "Walnut/Random.h"

void Renderer::Resize(uint32_t width, uint32_t height)
{
    if(m_FinalImage && m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
    {
        return;
    }

    if(m_FinalImage)
    {
        m_FinalImage->Resize(width, height);
    }
    else
    {
        m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
    }

     delete[] m_ImageData;
     m_ImageData = new uint32_t[static_cast<unsigned long long>(width * height)];
}

void Renderer::Render()
{
    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
    {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
        {
            auto coordinate = glm::vec2({
                static_cast<float>(x) / static_cast<float>(m_FinalImage->GetWidth()),
                static_cast<float>(y) / static_cast<float>(m_FinalImage->GetHeight())
            });
            // coordinate = coordinate * 2.0f - 1.0f; // -1 -> 1
            m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coordinate);
        }
    }

    m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coordinate)
{
    const uint8_t r = static_cast<uint8_t>(255.99f * coordinate.x);
    const uint8_t g = static_cast<uint8_t>(255.99f * coordinate.y);


    
    return 0xFF000000 | r | (g << 8);
}