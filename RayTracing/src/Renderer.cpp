#include "Renderer.h"

#include "Walnut/Random.h"

namespace Utils
{
    uint32_t ConvertToRGBA(const glm::vec4 & color)
    {
        const uint8_t red = static_cast<uint8_t>(color.r * 255.0f);
        const uint8_t green = static_cast<uint8_t>(color.g * 255.0f);
        const uint8_t blue = static_cast<uint8_t>(color.b * 255.0f);
        const uint8_t alpha = static_cast<uint8_t>(color.a * 255.0f);

        return (alpha << 24) | (blue << 16) | (green << 8) | red;
    }
}

void Renderer::OnResize(uint32_t width, uint32_t height)
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

void Renderer::Render(const Camera & camera)
{
    Ray ray;
    ray.Origin = camera.GetPosition();
    
    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
    {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
        {
            ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
            auto color = TraceRay(ray);
            color = clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
        }
    }

    m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const Ray & ray) const
{
    constexpr auto radius = 0.5f;

    const float a = glm::dot(ray.Direction, ray.Direction);
    const float b = 2.0f * glm::dot(ray.Origin, ray.Direction);
    const float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

    // Quadratic formula discriminant:
    const float discriminant = b * b - 4.0f * a * c;

    if(discriminant < 0.0f)
    {
        return backgroundColor;
    }

    // Quadratic formula:
    // (-) will always be the smallest value because
    // - a is always positive (dot product of a vector with itself)
    // - (-b) will have the same value between t0 and t1
    // - sqrt(discriminant) will always be positive
    float t = (-b - sqrt(discriminant)) / (2.0f * a);
    
    const glm::vec3 hitPosition = ray.Origin + ray.Direction * t;
    const glm::vec3 normal = normalize(hitPosition);

    const glm::vec3 lightDirection = normalize(glm::vec3(-1, -1, -1));
    const float lightIntensity = glm::max(dot(normal, -lightDirection), 0.0f);
    
    glm::vec4 computedColor = sphereColor * lightIntensity;
    computedColor.a = 1.0f;
    
    return computedColor;
}