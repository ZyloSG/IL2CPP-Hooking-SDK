//https://www.github.com/ZyloSG

#include "includes.h"
#include "iostream"
#include "functions.h"
#include "hooks.h"
#include "game.h"
#include "offsets.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void SetStyles()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.17f, 0.19f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.21f, 0.22f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.08f, 0.51f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.28f, 0.56f, 1.00f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.40f, 0.67f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.35f, 0.58f, 1.00f);
}

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
	SetStyles();
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool show = false;
bool init = false;

enum Tabs {
	TAB_MAIN,
	TAB_MISC
};

int currentTab = TAB_MAIN;

void DrawRainbowWatermark() {
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiCond_Always);
	ImGui::Begin("Watermark", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetCursorScreenPos();
	float width = 300;
	float thickness = 4.0f;

	ImU32 rainbow_start = IM_COL32(0, 255, 255, 255);
	ImU32 rainbow_end = IM_COL32(255, 255, 0, 255);

	draw_list->AddRectFilledMultiColor(
		ImVec2(p.x, p.y),
		ImVec2(p.x + width, p.y + thickness),
		rainbow_start,
		rainbow_end,
		rainbow_end,
		rainbow_start
	);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + thickness + 4);
	ImGui::Text("stumbleploit v1.0");

	ImGui::End();
}

void RenderUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	DrawRainbowWatermark();

	ImGui::Begin("Stumbleploit - @26zylo", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::BeginChild("LeftSidebar", ImVec2(150, 300), true);
	if (ImGui::Selectable("Main", currentTab == TAB_MAIN)) currentTab = TAB_MAIN;
	if (ImGui::Selectable("Misc", currentTab == TAB_MISC)) currentTab = TAB_MISC;
	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("RightContent", ImVec2(300, 300), true);
	switch (currentTab)
	{
	case TAB_MAIN:
		ImGui::Text("Main");
		ImGui::Spacing();
		ImGui::Text("Cosmetics");
		if (ImGui::Button("Unlock All")) {
			unlockAll();
		}
		break;
	case TAB_MISC:
		ImGui::Text("Misc");
		if (ImGui::Button("Self-Destruct")) {
			MH_Uninitialize();
			//MH_DisableHook(MH_ALL_HOOKS);
			kiero::shutdown();
		}
		break;
	default:
		break;
	}
	ImGui::EndChild();

	ImGui::End();

	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui(); // initialize imgui
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}
	if (GetAsyncKeyState(VK_INSERT)&1)
	{
		show = !show;
	}

	if (show)
	{
		RenderUI();
	}
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
			AllocConsole();
			FILE * f;
			freopen_s(&f, "CONOUT$", "w", stdout);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
			std::cout << "\n*******************************************************************************" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			std::cout << R"(  
  ██████   ▄████  ██▓███   ██▓     ▒█████   ██▓▄▄▄█████▓
▒██    ▒  ██▒ ▀█▒▓██░  ██▒▓██▒    ▒██▒  ██▒▓██▒▓  ██▒ ▓▒
░ ▓██▄   ▒██░▄▄▄░▓██░ ██▓▒▒██░    ▒██░  ██▒▒██▒▒ ▓██░ ▒░
  ▒   ██▒░▓█  ██▓▒██▄█▓▒ ▒▒██░    ▒██   ██░░██░░ ▓██▓ ░ 
▒██████▒▒░▒▓███▀▒▒██▒ ░  ░░██████▒░ ████▓▒░░██░  ▒██▒ ░ 
▒ ▒▓▒ ▒ ░ ░▒   ▒ ▒▓▒░ ░  ░░ ▒░▓  ░░ ▒░▒░▒░ ░▓    ▒ ░░   
░ ░▒  ░ ░  ░   ░ ░▒ ░     ░ ░ ▒  ░  ░ ▒ ▒░  ▒ ░    ░    
░  ░  ░  ░ ░   ░ ░░         ░ ░   ░ ░ ░ ▒   ▒ ░  ░      
      ░        ░              ░  ░    ░ ░   ░           
	)" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
			std::cout << "\n*******************************************************************************\n" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
			std::cout << "[stumbleploit] base address: " << module << std::endl;
			std::cout << "[stumbleploit] gameassembly base address: " << game << std::endl;
			std::cout << "[stumbleploit] unityplayer base address: " << unityplayer << std::endl;

			/*SetWindowText(
				FindWindow("Stumble Guys", NULL),
				"Stumble Guys - SGPloit 1.0"
			); - doesnt work for now open an issue on the repo if you know a fix*/

			std::cout << "press insert to view the menu" << std::endl;


			std::cout << "" << std::endl;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}