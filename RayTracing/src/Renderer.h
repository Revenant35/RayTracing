#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Camera.h"
#include "Ray.h"
#include "Walnut/Image.h"

class Renderer
{
public:

    Renderer() = default;
    void OnResize(uint32_t width, uint32_t height);
    void Render(const Camera & camera);
    [[nodiscard]] std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

    [[nodiscard]] glm::vec4 GetBackgroundColor() const { return backgroundColor; }
    void SetBackgroundColor(const glm::vec4 & color) { backgroundColor = color; }
    [[nodiscard]] glm::vec4 GetSphereColor() const { return sphereColor; }
    void SetSphereColor(const glm::vec4 & color) { sphereColor = color; }

private:

    [[nodiscard]] glm::vec4 TraceRay(const Ray & ray) const;
    std::shared_ptr<Walnut::Image> m_FinalImage;
    uint32_t * m_ImageData = nullptr;
    glm::vec4 backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 sphereColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

};