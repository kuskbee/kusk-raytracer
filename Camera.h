#pragma once

#include "graphics.h"

using namespace glm;

class Camera
{
public:
	Camera( ) { UpdateViewDir( );  }
	mat4 GetViewRow( );
	mat4 GetProjRow( );
	vec3 GetEyePos( );
    vec3 GetViewDir( );
    vec3 GetUpDir( );

    void UpdateViewDir( );
    void UpdateKeyboard(const float dt, bool const keyPressed[ 256 ]);
    void UpdateMouse(float mouseNdcX, float mouseNdcY);
    void MoveForward(float dt);
    void MoveRight(float dt);
    void MoveUp(float dt);
    void SetAspectRatio(float aspect);

public:
    bool m_useFirstPersonView = false;

private:
    vec3 m_position = vec3(0.0f, 0.0f, -2.5f);
    vec3 m_viewDir = vec3(0.107699148f, 0.107699148f, 0.107699148f);
    vec3 m_upDir = vec3(0.0f, 1.0f, 0.0f);
    vec3 m_rightDir = vec3(1.0f, 0.0f, 0.0f);

    // roll, pitch, yaw
    // https://en.wikipedia.org/wiki/Aircraft_principal_axes
    float m_yaw = 1.46280432f, m_pitch = -0.0392698720f;

    float m_speed = 3.0f; // 움직이는 속도

    // 프로젝션 옵션도 카메라 클래스로 이동
    float m_projFovAngleY = 90.0f;
    float m_nearZ = 0.01f;
    float m_farZ = 100.0f;
    float m_aspect = 16.0f / 9.0f;
    bool m_usePerspectiveProjection = true;
};

