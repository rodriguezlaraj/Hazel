#pragma once

#include <glm/glm.hpp>

namespace Hazel {

    class OrthographicCamera
    {
    public:
        //We could also receive the near and far bounds (-1 for near, +1 for far)
        //This is like a rectangle or a cube
        OrthographicCamera(float left, float right, float bottom, float top);

        void SetProjection(float left, float right, float bottom, float top);


        //after updating rotation or position, we need to recalculate the ViewProjectionMatrix
        //if we are responding to the mouse, we can update the ViewProjectionMatrix on the event.
        const glm::vec3& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

        float GetRotation() const { return m_Rotation; }
        void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    private:
        void RecalculateViewMatrix();
    private:
        //Once per frame, we want to recalculate the view matrix.
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;

        //holdthe multiplication to avoid making the multiplication every time
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

        //Orthographic cameras are not really rotated. They can be rotated on the z axis but in this case the rotation can be a single float.
        float m_Rotation = 0.0f;
    };

}
