#include "Example.h"

#include <imgui.h>
#include <imgui_impl_win32.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Example::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        return 0;
        break;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_MOUSEMOVE:
        OnMouseMove(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_LBUTTONDOWN:
        if (!m_leftButton) {
            m_dragStartFlag = true; // 드래그를 새로 시작하는지 확인
        }
        m_leftButton = true;
        break;
    case WM_LBUTTONUP:
        m_leftButton = false;
        break;
    case WM_RBUTTONDOWN:
        if (!m_rightButton) {
            m_dragStartFlag = true; // 드래그를 새로 시작하는지 확인
        }
        m_rightButton = true;
        break;
    case WM_RBUTTONUP:
        m_rightButton = false;
        break;
    case WM_KEYDOWN:
        m_keyPressed[ wParam ] = true;
        if (wParam == VK_ESCAPE) { // ESC키 종료
            DestroyWindow(hwnd);
        }
        if (wParam == VK_SPACE) {
            raytracer.m_lightRotate = !raytracer.m_lightRotate;
        }
        break;
    case WM_KEYUP:
        if (wParam == 'F') { // f키 일인칭 시점
            raytracer.m_camera.m_useFirstPersonView = !raytracer.m_camera.m_useFirstPersonView;
        }

        m_keyPressed[ wParam ] = false;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

void Example::OnMouseMove(int mouseX, int mouseY) {

    // 마우스 커서의 위치를 NDC로 변환
    // 마우스 커서는 좌측 상단 (0, 0), 우측 하단(width-1, height-1)
    // NDC는 좌측 하단이 (-1, -1), 우측 상단(1, 1)
    m_cursorNdcX = mouseX * 2.0f / width - 1.0f;
    m_cursorNdcY = -mouseY * 2.0f / height + 1.0f;

    // 커서가 화면 밖으로 나갔을 경우 범위 조절
    m_cursorNdcX = std::clamp(m_cursorNdcX, -1.0f, 1.0f);
    m_cursorNdcY = std::clamp(m_cursorNdcY, -1.0f, 1.0f);

    // 카메라 시점 회전
    raytracer.m_camera.UpdateMouse(m_cursorNdcX, m_cursorNdcY);
}