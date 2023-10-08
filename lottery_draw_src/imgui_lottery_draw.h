// imgui_lottery_draw. RCSZ.
// C++17 [x64]

#ifndef _IMGUI_LOTTER_DRAW_H
#define _IMGUI_LOTTER_DRAW_H
#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define SRAND_UNIQUE_MIN  0.025f 
#define SRAND_UNIQUE_MAX  0.001f

#define IMGUI_ITEM_SPACING ImGui::GetStyle().ItemSpacing.x
#define IMGUI_ITEM_TITLE   ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().FramePadding.y * 2.0f - ImGui::GetStyle().ItemSpacing.y

struct PrizeItem {

	uint32_t    ItemTexture;     // OGL纹理句柄.
	size_t      ItemUnique;      // 唯一标识.
	std::string ItemPrizename;   // 奖品名称.
	std::string ItemDescription; // 奖品简介.

	float Position;

	PrizeItem(uint32_t Tex, size_t Idx, std::string Nam, std::string Des = {}, float Pos = 0.0f) :
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

namespace ImLDmaths {

	class MathsTick {
	protected:
		std::vector<PrizeItem> RunnerItems = {};
		std::vector<PrizeItem> PrizeItems  = {};
		
		float Acceleration = 0.0f, MoveVector = 0.0f;

		// VecSpeed:  [+-n] right => left.
		// RectImgSz: square image length.
		void CreateRunner(float VecSpeed, float RectImgSz, size_t Repeat = 1);

		// Speed:  scale: VecSpeed * Speed.
		// MinPos: x轴最左(item轮回).
		void CalcRunnerStatus(float Speed, float MinPos, float RectImgSz);

		// RectImgPos: input x-move.
		// PosLimit:   x:x-axis min, y:x-axis max.
		void CalcImgCropping(
			ImVec2& OutSize, ImVec2& OutUv0, ImVec2& OutUv1, float& RectImgPos, 
			float RectImgSz, ImVec2 PosLimit
		);
	};
}

namespace ImLD {

	class ImLotteryDraw :public ImLDmaths::MathsTick {
	protected:
		ImGuiIO& GetImguiIO = ImGui::GetIO();

		std::string MainWindowTitle = {};
		std::string PrizePoolViewButtonTitle = {};
		std::string PrizePoolViewWinPoolTitle = {};

		// render param.
		float  CenterLineWidth        = 2.25f;
		ImVec4 CenterLineColor        = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
		float  BackgroundTransparency = 0.32f;
		ImVec2 BackgroundRectSize     = ImVec2(480.0f, 160.0f);

		bool FlagLotteryDraw  = false;
		bool FlagViewPrize    = false;
		bool FlagViewWinPrize = false;

		float       TmpSquareImageLen = BackgroundRectSize.y - IMGUI_ITEM_SPACING * 3.0f;
		std::string TmpHeadlining = {};

		// 跟随鼠标悬浮窗口.
		void comp_mouse_windowa(ImVec2 window_size, bool focus = false, ImVec2 mouse_len = ImVec2(16.0f, 16.0f));
		void comp_mouse_windowb();

		// 子(副)窗口. view: True: ImageButton, False: Image.
		void secondary_window(std::vector<PrizeItem>& indata, const char* name, bool& open, bool view = true);

	public:
		ImLotteryDraw(const char* WindowTitle, const char* ViewButton, const char* ViewWin);

		// 赢取的奖品池.
		std::vector<PrizeItem> WinPrizePool = {};

		bool ImLD_PushItem(std::string PrizeName, size_t Unique, uint32_t GLTexture, std::string Description = {});
		void ImLD_Init();

		bool ImLD_StartLottery();

		void ImLD_RunTick(std::function<void()> ExtFunc = {});
	};
}

#endif