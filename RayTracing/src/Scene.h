#pragma once
#include <vector>
#include <glm/vec3.hpp>

namespace Material
{
    constexpr float MIN_ROUGHNESS = 0.0f;
    constexpr float MAX_ROUGHNESS = 1.0f;
    constexpr float MIN_METALLIC = 0.0f;
    constexpr float MAX_METALLIC = 1.0f;
    
    struct Material
    {
        glm::vec3 Albedo{1.0f};
        float Roughness = 1.0f;
        float Metallic = 0.0f;
    };
}



struct Sphere
{
    glm::vec3 Position{0.0f};
    float Radius = 0.5f;
    
    size_t MaterialIndex = 0;
};

struct Scene
{
    std::vector<Sphere> Spheres;
    std::vector<Material::Material> Materials;
};