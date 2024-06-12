﻿#include "Renderer.h"

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
    if (m_FinalImage && m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
    {
        return;
    }

    if (m_FinalImage)
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

void Renderer::Render(const Camera & camera, const Scene & scene)
{
    m_ActiveScene = &scene;
    m_ActiveCamera = &camera;

    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
    {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
        {
            auto color = RayGen(x, y);
            color = clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
        }
    }

    m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::RayGen(const uint32_t x, const uint32_t y)
{
    Ray ray;
    ray.Origin = m_ActiveCamera->GetPosition();
    ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

    const auto payload = TraceRay(ray);
    if(!payload.Hit)
    {
        return backgroundColor;
    }

    const glm::vec3 lightDirection = normalize(glm::vec3(-1, -1, -1));
    const float lightIntensity = glm::max(dot(payload.WorldNormal, -lightDirection), 0.0f);

    const Sphere & sphere = m_ActiveScene->Spheres[payload.ObjectIndex];

    glm::vec3 computedColor = sphere.Albedo * lightIntensity;

    return {computedColor, 1.0f};
}

Renderer::HitPayload Renderer::TraceRay(const Ray & ray) const
{
    size_t closestSphere = std::numeric_limits<size_t>::max();
    float hitDistance = std::numeric_limits<float>::max();

    const float a = dot(ray.Direction, ray.Direction);

    for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
    {
        const Sphere & sphere = m_ActiveScene->Spheres[i];
        glm::vec3 origin = ray.Origin - sphere.Position;

        const float b = 2.0f * glm::dot(origin, ray.Direction);
        const float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

        // Quadratic formula discriminant:
        const float discriminant = b * b - 4.0f * a * c;

        if (discriminant < 0.0f)
        {
            continue;
        }

        // Quadratic formula:
        // (-) will always be the smallest value because
        // - a is always positive (dot product of a vector with itself)
        // - (-b) will have the same value between t0 and t1
        // - sqrt(discriminant) will always be positive
        if (const float sphereDistance = (-b - sqrt(discriminant)) / (2.0f * a); sphereDistance < hitDistance && sphereDistance > 0.0f)
        {
            closestSphere = i;
            hitDistance = sphereDistance;
        }
    }

    if (closestSphere == std::numeric_limits<size_t>::max())
    {
        return Miss(ray);
    }

    return ClosestHit(ray, hitDistance, closestSphere);

}

Renderer::HitPayload Renderer::ClosestHit(const Ray & ray, float hitDistance, size_t objectIndex) const
{
    HitPayload payload;
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;
    payload.Hit = true;

    const Sphere & closestSphere = m_ActiveScene->Spheres[objectIndex];

    const glm::vec3 origin = ray.Origin - closestSphere.Position;
    payload.WorldPosition = origin + ray.Direction * hitDistance;
    payload.WorldNormal = normalize(payload.WorldPosition);

    payload.WorldPosition += closestSphere.Position;

    return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray & ray) const
{
    HitPayload payload;
    payload.Hit = false;
    payload.HitDistance = std::numeric_limits<float>::max();
    payload.ObjectIndex = std::numeric_limits<size_t>::max();
    return payload;
}