#include "Camera.h"
#include "Renderer.h"
#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Walnut::Layer
{
public:
    ExampleLayer()
        :m_Camera(45.0f, 0.1f, 100.0f )
    {
        Materials::Material & pinkSphere = m_Scene.Materials.emplace_back();
        pinkSphere.Albedo = {1.0f, 0.0f, 1.0f};
        pinkSphere.Roughness = 0.0f;
        pinkSphere.Metallic = 0.0f;

        Materials::Material & blueSphere = m_Scene.Materials.emplace_back();
        blueSphere.Albedo = {0.2f, 0.3f, 1.0f};
        blueSphere.Roughness = 0.1f;
        blueSphere.Metallic = 0.0f;
        
        Sphere sphere1;
        sphere1.Position = {0.0f, 0.0f, 0.0f};
        sphere1.Radius = 1.0f;
        sphere1.MaterialIndex = 0;

        Sphere sphere2;
        sphere2.Position = {0.0f, -101.0f, 0.0f};
        sphere2.Radius = 100.0f;
        sphere2.MaterialIndex = 1;

        m_Scene.Spheres.push_back(sphere1);
        m_Scene.Spheres.push_back(sphere2);
    }

    virtual void OnUpdate(float ts) override
    {
        m_Camera.OnUpdate(ts);
    }

    virtual void OnUIRender() override
    {
        ImGui::Begin("Settings");
        ImGui::Text("Last Frame Time: %.3fms", m_LastFrameTime);
        ImGui::End();
        
        ImGui::Begin("Scene");
        for(size_t i = 0; i < m_Scene.Spheres.size(); i++)
        {
            ImGui::PushID(i);
            
            Sphere & sphere = m_Scene.Spheres[i];
            ImGui::Text("Sphere %d", i + 1);
            ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.025f);
            ImGui::DragFloat("Radius", &sphere.Radius, 0.01f, 0.0f);
            ImGui::DragInt("Material", reinterpret_cast<int *>(&sphere.MaterialIndex), 1, 0, m_Scene.Materials.size() - 1);
            ImGui::Separator();

            ImGui::PopID();
        }

        for(size_t i = 0; i < m_Scene.Materials.size(); i++)
        {
            ImGui::PushID(i);

            Materials::Material & material = m_Scene.Materials[i];
            ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
            ImGui::DragFloat("Roughness", &material.Roughness, 0.01f, Materials::MIN_ROUGHNESS, Materials::MAX_ROUGHNESS);
            ImGui::DragFloat("Metallic", &material.Metallic, 0.01f, Materials::MIN_METALLIC, Materials::MAX_METALLIC);
            ImGui::Separator();

            ImGui::PopID();

        }

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");

        m_ViewportWidth = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
        m_ViewportHeight = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

        if (const auto image = m_Renderer.GetFinalImage())
        {
            ImGui::Image(image->GetDescriptorSet(),
                { static_cast<float>(image->GetWidth()), static_cast<float>(image->GetHeight()) },
                ImVec2(0, 1), ImVec2(1, 0)
            );
        }
        ImGui::End();
        ImGui::PopStyleVar();

        Render();
    }

    void Render()
    {
        Walnut::Timer timer;

        m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
        m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
        m_Renderer.Render(m_Camera, m_Scene);

        m_LastFrameTime = timer.ElapsedMillis();
    }

private:

    Camera m_Camera;
    Scene m_Scene;
    Renderer m_Renderer;
    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
    float m_LastFrameTime = 0.0f;
    
};

Walnut::Application * Walnut::CreateApplication(int argc, char ** argv)
{
    Walnut::ApplicationSpecification spec;
    spec.Name = "Ray Tracing";

    Walnut::Application * app = new Walnut::Application(spec);
    app->PushLayer<ExampleLayer>();
    app->SetMenubarCallback([app](){
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                app->Close();
            }
            ImGui::EndMenu();
        }
    });
    return app;
}