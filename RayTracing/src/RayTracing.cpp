#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Frame Time: %.3fms", m_LastFrameTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		m_ViewportWidth = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
		m_ViewportHeight = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

		if(m_Image)
		{
			ImGui::Image(m_Image->GetDescriptorSet(), { static_cast<float>(m_Image->GetWidth()), static_cast<float>(m_Image->GetHeight()) });
		}
		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Walnut::Timer timer;
		
		if(!m_Image || m_Image->GetWidth() != m_ViewportWidth || m_Image->GetHeight() != m_ViewportHeight)
		{
			m_Image = std::make_shared<Walnut::Image>(m_ViewportWidth, m_ViewportHeight, Walnut::ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[static_cast<unsigned long long>(m_ViewportWidth * m_ViewportHeight)];
		}

		for(uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++)
		{
			m_ImageData[i] = Walnut::Random::UInt();
			m_ImageData[i] |= 0xFF << 24;
		}

		m_Image->SetData(m_ImageData);

		m_LastFrameTime = timer.ElapsedMillis();
	}

private:
	std::shared_ptr<Walnut::Image> m_Image;
	uint32_t * m_ImageData = nullptr;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastFrameTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
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