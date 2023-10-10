// imgui_lottery_draw. RCSZ.
// C++17 [RESx64]

#ifndef _IMGUI_LOTTER_DRAW_H
#define _IMGUI_LOTTER_DRAW_H
#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define SRAND_UNIQUE_MIN  0.032f 
#define SRAND_UNIQUE_MAX  0.00085f

#define IMGUI_ITEM_SPACING ImGui::GetStyle().ItemSpacing.x
#define IMGUI_ITEM_TITLE   ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().FramePadding.y * 2.0f - ImGui::GetStyle().ItemSpacing.y

struct PrizeItem {

	uint32_t    ItemTexture;     // opengl handle(ui32).
	size_t      ItemUnique;      // prize uniqueNumber.
	std::string ItemPrizename;   // prize name.
	std::string ItemDescription; // prize description.

	float Position;

	PrizeItem(uint32_t Tex, size_t Idx, std::string Nam, std::string Des = "", float Pos = 0.0f) :
		ItemTexture     (Tex),
		ItemUnique      (Idx),
		ItemPrizename   (Nam),
		ItemDescription (Des),
		Position        (Pos)
	{}
	PrizeItem() :
		ItemTexture     (NULL),
		ItemUnique      (NULL),
		ItemPrizename   (""),
		ItemDescription (""),
		Position        (0.0f)
	{}
};

namespace ImGameLotteryCalc {

	class CalculateTick {
	protected:
		std::vector<PrizeItem> RunnerItems = {};
		std::vector<PrizeItem> PrizeItems  = {};

		float Acceleration = 0.0f, SpeedVector = 0.0f;

		// @param float 初始速度, float image大小(square), size_t 重复 = 1
		void CreateRunner(float VecSpeed, float ImageSize, size_t Repeat = 1);

		// @param float 速度缩放, float 最低位置(x-asix), float image大小(square)
		void CalcRunnerStatus(float Speed, float MinPos, float ImageSize);

		// RectImgPos: input x-move.
		// PosLimit:   x:x-axis min, y:x-axis max.
		void CalcImgCropping(
			ImVec2& OutSize, ImVec2& OutUv0, ImVec2& OutUv1, float& ImagePos,
			float ImageSize, ImVec2 LimitPos
		);
	};
}

namespace ImGLO {

	class ImGameLottery :public ImGameLotteryCalc::CalculateTick {
	protected:
		ImGuiIO& GetImguiIO = ImGui::GetIO();

		std::string MainWindowTitle          = {};
		std::string PrizePoolViewButtonTitle = {};
		std::string PrizePoolViewWinTitle    = {};

		// render param.
		float  CenterLineWidth        = 2.25f;
		ImVec4 CenterLineColor        = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
		float  BackgroundTransparency = 0.72f;
		ImVec2 BackgroundRectSize     = ImVec2(480.0f, 160.0f);

		bool FlagGameLottery  = false;
		bool FlagViewPrize    = false;
		bool FlagViewPrizeWin = false;

		float       TmpSquareImageLen = BackgroundRectSize.y - IMGUI_ITEM_SPACING * 3.0f;
		std::string TmpHeadlining     = {};

		// 跟随鼠标悬浮窗口(context).
		void comp_mouse_windowa(ImVec2 WindowSize, bool Focus = false, ImVec2 MouseLen = ImVec2(16.0f, 16.0f));
		void comp_mouse_windowb();

		// 子(副)窗口.
		void comp_window_secondary(std::vector<PrizeItem>& DataIn, const char* Name, bool& OpenFlag, bool View = true);

	public:
		ImGameLottery(const char* WindowTitle, const char* ViewButton, const char* ViewWin);

		std::vector<PrizeItem> WinPrizePool = {};
		uint32_t WindowBackgroundImg = NULL;

		bool PushItem(std::string PrizeName, size_t Unique, uint32_t GLTexture, std::string Description = {});
		void InitPrizePool();

		bool StartLottery();
		void RunTick(std::function<void()> ExtFunc = {}, bool WinOpen = true, ImGuiWindowFlags WinFlags = NULL);
	};
}

#endif