#include "Camera.h"

mat4 Camera::GetViewRow( ) {
    // cout << m_position.x << ", " << m_position.y << ", " << m_position.z << "
    // " << m_yaw << " " << m_pitch << endl;

    // 렌더링에 사용할 View 행렬을 만들어주는 부분
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), -this->m_position) *
        glm::rotate(glm::mat4(1.0f), -this->m_yaw, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), this->m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));

    return viewMatrix;
}

vec3 Camera::GetEyePos( ) { return m_position; }
vec3 Camera::GetViewDir( ) { return m_viewDir; }
vec3 Camera::GetUpDir( ) { return m_upDir; }

void Camera::UpdateViewDir( ) {
    
    // 정면 방향 벡터(카메라가 바라보는 방향)를 Yaw에 따라 회전시킴
    vec3 front;
    front.x = cos(m_yaw) * cos(m_pitch);
    front.y = sin(m_pitch);
    front.z = sin(m_yaw) * cos(m_pitch);

    m_viewDir = glm::normalize(front);

    m_rightDir = -glm::normalize(glm::cross(m_upDir, m_viewDir));
}

void Camera::UpdateKeyboard(const float dt, bool const keyPressed[ 256 ]) {
    if (m_useFirstPersonView) {
        if (keyPressed[ 'W' ])
            MoveForward(dt);
        if (keyPressed[ 'S' ])
            MoveForward(-dt);
        if (keyPressed[ 'D' ])
            MoveRight(dt);
        if (keyPressed[ 'A' ])
            MoveRight(-dt);
        if (keyPressed[ 'E' ])
            MoveUp(dt);
        if (keyPressed[ 'Q' ])
            MoveUp(-dt);
    }
}

void Camera::UpdateMouse(float mouseNdcX, float mouseNdcY) {
    if (m_useFirstPersonView) {
        // 얼마나 회전할지 계산
        m_yaw = mouseNdcX * glm::two_pi<float>();     // 좌우 360도
        m_pitch = mouseNdcY * glm::half_pi<float>(); // 위 아래 90도

        UpdateViewDir( );
    }
}

void Camera::MoveForward(float dt) {
    // 이동후의_위치 = 현재_위치 + 이동방향 * 속도 * 시간차이;
    m_position += m_viewDir * m_speed * dt;
}

void Camera::MoveUp(float dt) {
    // 이동후의_위치 = 현재_위치 + 이동방향 * 속도 * 시간차이;
    m_position += m_upDir * m_speed * dt;
}

void Camera::MoveRight(float dt) { m_position += m_rightDir * m_speed * dt; }

void Camera::SetAspectRatio(float aspect) { m_aspect = aspect; }

mat4 Camera::GetProjRow( ) {
    if (m_usePerspectiveProjection)
    {
        glm::mat4 projection = glm::perspective(glm::radians(m_projFovAngleY), m_aspect, m_nearZ, m_farZ);
        projection[ 2 ][ 2 ] *= -1; // Z 축 반전
        projection[ 2 ][ 3 ] *= -1; // Far plane에 대한 Z축 반전

        return projection;
    }
    
    return glm::ortho(-m_aspect, m_aspect, -1.0f, 1.0f, m_nearZ, m_farZ);
}