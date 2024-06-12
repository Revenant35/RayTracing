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
        m_Scene.Spheres.push_back(Sphere{{0.5f, -2.0f, 1.0f}, 0.25f, {1.0f, 1.0f, 0.0f}});
        m_Scene.Spheres.push_back(Sphere{{1.0f, 1.0f, 0.0f}, 0.15f, {1.0f, 0.0f, 1.0f}});
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
            ImGui::DragFloat("Radius", &sphere.Radius, 0.01f);
            ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.Albedo));
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