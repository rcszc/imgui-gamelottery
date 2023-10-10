// imgui_gamelottery_calc.
#include <algorithm>
#include <cfloat>

#include "imgui_gamelottery.hpp"

using namespace std;

namespace ImGameLotteryCalc {

	void CalculateTick::CreateRunner(float VecSpeed, float ImageSize, size_t Repeat) {
		SpeedVector = VecSpeed;

		size_t PrizeItemsCount = NULL;
		size_t RunnerItemsNum  = PrizeItems.size() * Repeat;

		// PrizeItems => RunnerItems(转轮)
		for (size_t i = 0; i < RunnerItemsNum; ++i) {

			RunnerItems.push_back(PrizeItem(
				PrizeItems[PrizeItemsCount].ItemTexture,
				PrizeItems[PrizeItemsCount].ItemUnique,
				PrizeItems[PrizeItemsCount].ItemPrizename,
				PrizeItems[PrizeItemsCount].ItemDescription,
				ImageSize * i
			));

			++PrizeItemsCount;
			if (PrizeItemsCount >= PrizeItems.size())
				PrizeItemsCount = NULL;
		}
	}

	void CalculateTick::CalcRunnerStatus(float Speed, float MinPos, float ImageSize) {
		float temp_itemloss = RunnerItems[0].Position - RunnerItems[RunnerItems.size() - 1].Position;
		// 修正 item 之间距离.
		if (temp_itemloss > ImageSize && temp_itemloss < (ImageSize + 100.0f))
			RunnerItems[0].Position = RunnerItems[RunnerItems.size() - 1].Position + ImageSize;

		for (size_t i = 0; i < RunnerItems.size(); ++i) {

			RunnerItems[i].Position -= SpeedVector * Speed;

			if (RunnerItems[i].Position < MinPos && i > 0)
				RunnerItems[i].Position = RunnerItems[i - 1].Position + ImageSize;
			else if (RunnerItems[i].Position < MinPos && i == 0)
				RunnerItems[0].Position = RunnerItems[RunnerItems.size() - 1].Position + ImageSize;
		}

		SpeedVector += Acceleration;
		SpeedVector = (SpeedVector < 0.0f) ? 0.0f : SpeedVector;
	}

	void CalculateTick::CalcImgCropping(
		ImVec2& OutSize, ImVec2& OutUv0, ImVec2& OutUv1, float& ImagePos,
		float ImageSize, ImVec2 LimitPos
	) {
		// position min(left).
		if ((ImagePos + ImageSize) > LimitPos.y) {
			float CroppingTemp = LimitPos.y - ImagePos;

			CroppingTemp = (CroppingTemp < 0.0f) ? 0.0f : CroppingTemp;

			OutSize = ImVec2(CroppingTemp, ImageSize);
			OutUv0  = ImVec2(0.0f, 0.0f);
			OutUv1  = ImVec2(CroppingTemp / ImageSize, 1.0f);
		}
		// position max(left).
		else if (ImagePos < LimitPos.x) {
			float CroppingTemp = ImageSize - abs(ImagePos - LimitPos.x);

			CroppingTemp = (CroppingTemp < 0.0f) ? 0.0f : CroppingTemp;

			OutSize = ImVec2(CroppingTemp, ImageSize);
			OutUv0  = ImVec2(1.0f - (CroppingTemp / ImageSize), 0.0f);
			OutUv1  = ImVec2(1.0f, 1.0f);
		}
		else {
			OutSize = ImVec2(ImageSize, ImageSize);
			OutUv0  = ImVec2(0.0f, 0.0f);
			OutUv1  = ImVec2(1.0f, 1.0f);
		}
		ImagePos = (ImagePos < LimitPos.x) ? LimitPos.x : (ImagePos > LimitPos.y) ? LimitPos.y : ImagePos;
	}
}