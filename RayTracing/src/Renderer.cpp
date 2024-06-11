﻿#include "Renderer.h"

#include "Walnut/Random.h"

void Renderer::Resize(uint32_t width, uint32_t height)
{
    if (!m_FinalImage)
    {
        m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
        delete[] m_ImageData;
        m_ImageData = new uint32_t[static_cast<unsigned long long>(width * height)];
    }

    if (m_FinalImage->GetWidth() != width && m_FinalImage->GetHeight() != height)
    {
        m_FinalImage->Resize(width, height);
        delete[] m_ImageData;
        m_ImageData = new uint32_t[static_cast<unsigned long long>(width * height)];
    }
}

void Renderer::Render()
{
    for(uint32_t i = 0; i < m_FinalImage->GetWidth() * m_FinalImage->GetHeight(); i++)
    {
        m_ImageData[i] = Walnut::Random::UInt();
        m_ImageData[i] |= 0xFF << 24;
    }

    m_FinalImage->SetData(m_ImageData);
}