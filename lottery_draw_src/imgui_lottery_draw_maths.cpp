// imgui_lottery_draw_maths.
#include <algorithm>
#include <cfloat>

#include "imgui_lottery_draw.h"

using namespace std;

namespace ImLDmaths {

	void MathsTick::CreateRunner(float VecSpeed, float RectImgSz, size_t Repeat) {
		size_t PrizeItemsCount = NULL;

		size_t RunnerItemsNum = PrizeItems.size() * Repeat;
		MoveVector = VecSpeed;

		// PrizeItems => RunnerItems(×ªÂÖ)
		for (size_t i = 0; i < RunnerItemsNum; ++i) {

			RunnerItems.push_back(PrizeItem(
				PrizeItems[PrizeItemsCount].ItemTexture,
				PrizeItems[PrizeItemsCount].ItemUnique,
				PrizeItems[PrizeItemsCount].ItemPrizename,
				PrizeItems[PrizeItemsCount].ItemDescription,
				RectImgSz * i
			));

			++PrizeItemsCount;
			if (PrizeItemsCount >= PrizeItems.size())
				PrizeItemsCount = NULL;
		}
	}

	void MathsTick::CalcRunnerStatus(float Speed, float MinPos, float RectImgSz) {
		// ÁÙÊ±ÐÞÕý.
		float tmploss = RunnerItems[0].Position - RunnerItems[RunnerItems.size() - 1].Position;
		if (tmploss > RectImgSz && tmploss < 200.0f)
			RunnerItems[0].Position = RunnerItems[RunnerItems.size() - 1].Position + RectImgSz;

		for (size_t i = 0; i < RunnerItems.size(); ++i) {

			RunnerItems[i].Position -= MoveVector * Speed;
		
			if (RunnerItems[i].Position < MinPos && i > 0)
				RunnerItems[i].Position = RunnerItems[i - 1].Position + RectImgSz;
			else if (RunnerItems[i].Position < MinPos && i == 0)
				RunnerItems[0].Position = RunnerItems[RunnerItems.size() - 1].Position + RectImgSz;
		}

		MoveVector += Acceleration;
		MoveVector = (MoveVector < 0.0f) ? 0.0f : MoveVector;
	}
	
	void MathsTick::CalcImgCropping(
		ImVec2& OutSize, ImVec2& OutUv0, ImVec2& OutUv1, float& RectImgPos, 
		float RectImgSz, ImVec2 PosLimit
	) {
		if ((RectImgPos + RectImgSz) > PosLimit.y) {
			float CroppingTemp = PosLimit.y - RectImgPos;

			CroppingTemp = (CroppingTemp < 0.0f) ? 0.0f : CroppingTemp;
			
			OutSize = ImVec2(CroppingTemp, RectImgSz);
			OutUv0  = ImVec2(0.0f, 0.0f);
			OutUv1  = ImVec2(CroppingTemp / RectImgSz, 1.0f);
		}
		else if (RectImgPos < PosLimit.x) {
			float CroppingTemp = RectImgSz - abs(RectImgPos - PosLimit.x);

			CroppingTemp = (CroppingTemp < 0.0f) ? 0.0f : CroppingTemp;

			OutSize = ImVec2(CroppingTemp, RectImgSz);
			OutUv0  = ImVec2(1.0f - (CroppingTemp / RectImgSz), 0.0f);
			OutUv1  = ImVec2(1.0f, 1.0f);
		}
		else {
			OutSize = ImVec2(RectImgSz, RectImgSz);
			OutUv0  = ImVec2(0.0f, 0.0f);
			OutUv1  = ImVec2(1.0f, 1.0f);
		}
		RectImgPos = (RectImgPos < PosLimit.x) ? PosLimit.x : (RectImgPos > PosLimit.y) ? PosLimit.y : RectImgPos;
	}
}