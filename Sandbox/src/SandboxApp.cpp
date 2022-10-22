
//The client only needs to:
// 0 This means that the main only needs to exist in the engine instead of the game.
//   I am writing an application that uses the Hazel engine. How does that application start? 
// 1 include Hazel
// 2 create the sandbox class which inherits from application
// 3 write a function that returns the class

#include <hzpch.h>

#include <Hazel.h>

#include "imgui/imgui.h"


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
        m_VertexArray.reset(Hazel::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
        Hazel::BufferLayout layout = {
            { Hazel::ShaderDataType::Float3, "a_Position" },
            { Hazel::ShaderDataType::Float4, "a_Color" }
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
        indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        m_SquareVA.reset(Hazel::VertexArray::Create());

        float squareVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };

        std::shared_ptr<Hazel::VertexBuffer> squareVB;
        squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            { Hazel::ShaderDataType::Float3, "a_Position" }
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<Hazel::IndexBuffer> squareIB;
        squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			uniform mat4 u_ViewProjection;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

        std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

        m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));

        std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

        std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

        m_BlueShader.reset(new Hazel::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}


    /*
    You can also update your game state at a FIXED time step very much like a physics engine. 
    This will give you consistent updates as well as reproducible results if you are debugging 
    certain types of time related issues. With a simple time step, if your frame rate drops to 
    30 fps you will miss frame updates which could lead to problems and gives inconsistent game updates.
    Now if you update the game at a FIXED time step, the game updates independent of the frame rate. 
    So if you update at a fixed time step of 60 fps, and the frame rate drops to 30 fps, then for that 
    frame you will update the game state TWICE (fixed time step is (1/ 60)). This depending on 
    implementation and update rate will create choppy or stuttering as characters move through 
    the game. One solution (which I have seen in many games) is they update at a very high time 
    step (say 1/120) so that the time interval is very small and this reduces the stuttering. 
    I save the previous position and current position of an object and then use that to interpolate 
    what is displayed on the screen between fixed time step intervals. This eliminates any stuttering 
    and I don't have to update at such a high fixed time step but introduces some very slight lag with 
    what is displayed on screen.
    If anyone is interested in researching this further google:  Fix Your Time Step.
    */
	void OnUpdate(Hazel::Timestep ts) override
	{
        HZ_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
        //If movement is done on event, the movement is abrupt and less smooth
        if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
            m_CameraRotation += m_CameraRotationSpeed * ts;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
            m_CameraRotation -= m_CameraRotationSpeed * ts;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_R))
        {
            m_CameraPosition.y = 0.0;
            m_CameraPosition.x = 0.0;
            m_CameraRotation = 0.0;
        }

        Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Hazel::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Hazel::Renderer::BeginScene(m_Camera);

        Hazel::Renderer::Submit(m_BlueShader, m_SquareVA);
        Hazel::Renderer::Submit(m_Shader, m_VertexArray);

        Hazel::Renderer::EndScene();

        /*
        bool updateCamera = false;
        bool rotateCamera = false;
        bool translateCamera = false;
		//HZ_INFO("ExampleLayer::Update");
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("Tab key is pressed (poll)!");


        //Translate
        if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_1)) 
        {
            updateCamera = true;
            rotateCamera = true;
        }
        //Rotate in Z
        else if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_2)) 
        {
            updateCamera = true;
            rotateCamera = true;
        }

        if (updateCamera) 
        {
            static float OriginXPos = Hazel::Input::GetMouseX();
            static float OriginYPos = Hazel::Input::GetMouseY();
            float deltaXPos = 0.0f;
            float deltaYPos = 0.0f;

            if (OriginXPos != Hazel::Input::GetMouseX())
            {
                deltaXPos = Hazel::Input::GetMouseX() - OriginXPos;
            }
            if (OriginYPos != Hazel::Input::GetMouseY())
            {
                deltaYPos = Hazel::Input::GetMouseY() - OriginYPos;
            }
        }*/

	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(Hazel::Event& event) override
	{

	}

    private:
        std::shared_ptr<Hazel::Shader> m_Shader;
        std::shared_ptr<Hazel::VertexArray> m_VertexArray;

        std::shared_ptr<Hazel::Shader> m_BlueShader;
        std::shared_ptr<Hazel::VertexArray> m_SquareVA;

        Hazel::OrthographicCamera m_Camera;
        glm::vec3 m_CameraPosition;
        float m_CameraMoveSpeed = 5.0f;

        float m_CameraRotation = 0.0f;
        float m_CameraRotationSpeed = 180.0f;
};

class Sandbox : public Hazel::Application 
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		//PushOverlay(new Hazel::ImGuiLayer());
	
	}
	~Sandbox() 
	{
	
	}

};


Hazel::Application* Hazel::CreateApplication() 
{
	return new Sandbox(); //Returns an instance of Sandbox which is the application built
}


//1Pick a build system that you want to use CMAKE, Premake, etc
//2Make sure that each library that you use, that you write each in your build system
//3You can add them as a Git submodule to make sure you can update them as updates come up.
//Most of the times is a hybrid, a combination of things, there is no right way of doing it.