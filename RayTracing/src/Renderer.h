﻿#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Camera.h"
#include "Ray.h"
#include "Walnut/Image.h"
#include "Scene.h"

class Renderer
{
public:

    Renderer() = default;
    void OnResize(uint32_t width, uint32_t height);
    void Render(const Camera & camera, const Scene & scene);
    
    std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private:

    struct HitPayload
    {
        float HitDistance;
        glm::vec3 WorldPosition;
        glm::vec3 WorldNormal;

        size_t ObjectIndex;
        bool Hit;
    };
    
    glm::vec4 RayGen(uint32_t x, uint32_t y); // RayGen

    HitPayload TraceRay(const Ray & ray) const;
    HitPayload ClosestHit(const Ray & ray, float hitDistance, size_t objectIndex) const;
    HitPayload Miss(const Ray & ray) const;
    
    const Scene * m_ActiveScene = nullptr;
    const Camera * m_ActiveCamera = nullptr;
    
    std::shared_ptr<Walnut::Image> m_FinalImage;
    uint32_t * m_ImageData = nullptr;
    glm::vec4 backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

};