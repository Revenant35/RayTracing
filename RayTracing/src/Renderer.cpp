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
            coordinate = coordinate * 2.0f - 1.0f; // [0,1] -> [-1, 1]
            m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coordinate);
        }
    }

    m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(const glm::vec2 & coordinate)
{
    constexpr glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
    const glm::vec3 rayDirection(coordinate.x, coordinate.y, -1.0f);
    constexpr auto radius = 0.5f;
    
    // rayDirection = normalize(rayDirection);

    const float a = dot(rayDirection, rayDirection);
    const float b = 2.0f * dot(rayOrigin, rayDirection);
    constexpr float c = dot(rayOrigin, rayOrigin) - radius * radius;

    // Quadratic formula discriminant:
    const float discriminant = b * b - 4.0f * a * c;

    if(discriminant < 0.0f)
    {
        return 0xFF000000;
    }

    const float divisor = 1.0f / (2.0f * a);

    // Select the closest intersection point in front of the camera:
    float t = std::max(
        (-b - sqrt(discriminant)) * divisor,
        (-b + sqrt(discriminant)) * divisor
    );

    // Normalize t to [0, 1]:
    t = (t + 1.0f) / 2.0f;

    const glm::vec3 sphereCenter(0.0f, 0.0f, -1.0f);
    const glm::vec3 intersectionPoint = rayOrigin + t * rayDirection;

    const glm::vec3 normal = normalize(intersectionPoint - sphereCenter);
    const glm::vec3 color = 0.5f * (normal + 1.0f);

    return 0xFF000000 | // Alpha
        (static_cast<uint32_t>(color.r * 255.0f) << 16) | // Red
        (static_cast<uint32_t>(color.g * 255.0f) << 8) | // Green
        static_cast<uint32_t>(color.b * 255.0f); // Blue
}