#include "hzpch.h"
#include "Hazel/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

    //uses -1 for far and 1 for near as default values
    //Create a default identity matrix for m_ViewMatrix
    //Calculate a valid value for m_ViewProjectionMatrix
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        HZ_PROFILE_FUNCTION();
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        HZ_PROFILE_FUNCTION();
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    //This can be optimized
    void OrthographicCamera::RecalculateViewMatrix()
    {
        HZ_PROFILE_FUNCTION();
        //First we use the identity matrix and translate it by the position
        //Then it gets multiplied by the rotation below
        //Then we create the rotation, Use the identity matrix, add the angle, and create the axis at Z (0,0,1)
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        //This has to be inversed because the scene moves oposite to the movement of the camera
        m_ViewMatrix = glm::inverse(transform);

        //GLM is Column major. For this reason the multiplication has to be in this order
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}
