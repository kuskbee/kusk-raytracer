#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <windows.h>
#include <memory>
#include <iostream>

#include "Example.h"

Example* example = nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main()
{
	const int width = 1280, height = 720;
	
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"KuskRayTracer",
		NULL
	};

	RegisterClassEx(&wc);

	// 해상도 설정
	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		L"Kusk Ray Tracer",
		WS_OVERLAPPEDWINDOW,
		100, // 윈도우 좌측 상단 x 좌표
		100, // 윈도우 좌측 상단 y 좌표
		wr.right - wr.left, // 윈도우 가로 방향 해상도
		wr.bottom - wr.top, // 윈도우 세로 방향 해상도
		NULL,
		NULL,
		wc.hInstance,
		NULL
	);

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	// Create Example Instance
	example = new Example(hwnd, width, height);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(width, height);
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplDX11_Init(example->device, example->deviceContext);
	ImGui_ImplWin32_Init(hwnd);


	// Main Message loop
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Start ImGui frame :IMGUI:
			/*ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Scene Control");
			ImGui::SliderFloat3("Light Position", &example->raytracer.light.pos.x, -2.0f, 2.0f);
			ImGui::SliderFloat3("Sphere Position", &example->raytracer.sphere->center.x, -1.0f, 1.0f);
			ImGui::End();
			ImGui::Render();*/

			// Example cycle
			example->Update(ImGui::GetIO( ).DeltaTime);
			example->Render();

			//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // :IMGUI:

			// switch the back buffer and the front buffer
			example->swapChain->Present(1, 0);
		}
	}

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	example->Clean();
	DestroyWindow(hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	delete example;
	example = nullptr;

	return 0;
}

// Windows procedure
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (example)
		return example->MsgProc(hWnd, msg, wParam, lParam);
	else
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
}