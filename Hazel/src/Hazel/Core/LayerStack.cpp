#include "hzpch.h"
#include "Hazel/Core/LayerStack.h"

namespace Hazel {

	//If a game changes levels, it could be necessary to destroy the layer stack and create a new one.
	LayerStack::~LayerStack()
	{
        for (Layer* layer : m_Layers)
        {
            layer->OnDetach();
            delete layer;
        }
	}


	//Get pushed into the first half of the list
	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	//Get pushed into the second half of the list
	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}


	//The poplayer does not destroy the layer. Only destruction of the layer stack will destroy the layers.
	//The layer stack is owned by the application.
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.begin() + m_LayerInsertIndex) //Fixed a bug in LayerStack which would occur if passing an overlay or  non existent layer in to PopLayer.
		{
            layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
	}

}