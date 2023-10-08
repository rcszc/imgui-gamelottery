// imgui_lottery_draw.
#include <chrono>
#include <random>

#include "imgui_lottery_draw.h"

using namespace std;

namespace ImLD {

    void ImLotteryDraw::comp_mouse_windowa(ImVec2 window_size, bool focus, ImVec2 mouse_len) {
        
        ImGui::SetNextWindowPos(ImVec2(GetImguiIO.MousePos.x + mouse_len.x, GetImguiIO.MousePos.y + mouse_len.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.16f, 0.16f, 0.16f, 0.95f)); // Push1.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 7.2f);                      // Push2.

        ImGui::Begin("", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

        if (focus)
            ImGui::SetWindowFocus();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.72f, 0.92f));      // Push3.
    }

    void ImLotteryDraw::comp_mouse_windowb() {
        ImGui::PopStyleColor(); // Pop3'

        ImGui::End();
        ImGui::PopStyleVar();   // Pop2'
        ImGui::PopStyleColor(); // Pop1'
    }

    void ImLotteryDraw::secondary_window(std::vector<PrizeItem>& indata, const char* name, bool& open, bool view) {

        ImGui::Begin(name, &open);
        {
            uint32_t LineCount = NULL, UniqueCount = NULL;
            for (const auto& Handle : indata) {

                ImGui::PushID(UniqueCount);
                ImGui::BeginGroup();
                {
                    if (view) {
                        ImGui::ImageButton(to_string(UniqueCount).c_str(), (void*)(intptr_t)Handle.ItemTexture, ImVec2(TmpSquareImageLen, TmpSquareImageLen));
                        if (ImGui::IsItemHovered() && GetImguiIO.MouseDown[0] && view) {

                            comp_mouse_windowa(ImVec2(192.0f, 192.0f));
                            ImGui::TextWrapped(Handle.ItemDescription.c_str());
                            comp_mouse_windowb();
                        }
                    }
                    else
                        ImGui::Image((void*)(intptr_t)Handle.ItemTexture, ImVec2(TmpSquareImageLen, TmpSquareImageLen));
                    ImGui::Indent((TmpSquareImageLen - ImGui::CalcTextSize(Handle.ItemPrizename.c_str()).x + IMGUI_ITEM_SPACING) / 2.0f);
                    ImGui::Text(Handle.ItemPrizename.c_str());
                }
                ImGui::EndGroup();
                ImGui::PopID();

                LineCount++;
                if (LineCount >= uint32_t(ImGui::GetWindowSize().x / TmpSquareImageLen) - 1)
                    LineCount = NULL;
                else
                    ImGui::SameLine();
                UniqueCount++;
            }
        }
        ImGui::End();
    }

    ImLotteryDraw::ImLotteryDraw(const char* WindowTitle, const char* ViewButton, const char* ViewWin) {
        MainWindowTitle           = WindowTitle;
        PrizePoolViewButtonTitle  = ViewButton;
        PrizePoolViewWinPoolTitle = ViewWin;
    }

    bool ImLotteryDraw::ImLD_PushItem(std::string PrizeName, size_t Unique, uint32_t GLTexture, std::string Description) {
        bool regflag = true;

        for (const auto& PrizeItem : PrizeItems) {
            if (Unique == PrizeItem.ItemUnique)
                regflag = false;
        }

        if (regflag)
            PrizeItems.push_back(PrizeItem(GLTexture, Unique, PrizeName, Description));
        return regflag;
    }

    bool ImLotteryDraw::ImLD_StartLottery() {
        if (!FlagLotteryDraw) {

            // 真随机性源 =种子=> MT19937 => 随机数(负加速度).
            random_device create_random;
            mt19937 _gen(create_random());
            uniform_real_distribution<float> _dis(SRAND_UNIQUE_MIN, SRAND_UNIQUE_MAX);

            FlagLotteryDraw = true;
            MoveVector      = 15.0f;
            Acceleration    = -_dis(_gen);
            
            return true;
        }
        else
            return false;
    }

    void ImLotteryDraw::ImLD_RunTick(std::function<void()> ExtFunc) {
        ImVec2    RenderBgSize = BackgroundRectSize;
        PrizeItem PrizeTemp = {};

        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));

        ImGui::Begin(MainWindowTitle.c_str());
        {
            // Item min width 384.0f.
            if (ImGui::GetWindowSize().x >= 384.0f)
                RenderBgSize.x = ImGui::GetWindowSize().x - IMGUI_ITEM_SPACING * 2.0f;
            else
                RenderBgSize.x = 384.0f;

            ImVec2 PosTemp = ImGui::GetCursorScreenPos();

            // Draw background rect.

            ImGui::GetWindowDrawList()->AddRectFilled(
                ImGui::GetCursorScreenPos(),
                ImVec2(
                    ImGui::GetCursorScreenPos().x + RenderBgSize.x,
                    ImGui::GetCursorScreenPos().y + RenderBgSize.y
                ),
                IM_COL32(
                    ImU32(ImGui::GetStyle().Colors[ImGuiCol_Button].x * 255.0f),
                    ImU32(ImGui::GetStyle().Colors[ImGuiCol_Button].y * 255.0f),
                    ImU32(ImGui::GetStyle().Colors[ImGuiCol_Button].z * 255.0f),
                    ImU32(BackgroundTransparency * 255.0f)
                ),
                ImGui::GetStyle().FrameRounding
            );

            // Draw image texture.

            for (const auto& Render : RunnerItems) {
                float XasixTemp = Render.Position;

                ImVec2 ImgOutSize = {}, ImgUV[2] = {}, BgImgLimit = ImVec2(IMGUI_ITEM_SPACING * 3.0f, RenderBgSize.x - IMGUI_ITEM_SPACING);
                // 渲染裁剪.
                CalcImgCropping(
                    ImgOutSize, 
                    ImgUV[0], ImgUV[1],
                    XasixTemp, 
                    TmpSquareImageLen, 
                    BgImgLimit
                );

                ImGui::SetCursorPos(ImVec2(XasixTemp, IMGUI_ITEM_SPACING * 5.5f));
                ImGui::Image((void*)(intptr_t)Render.ItemTexture, ImgOutSize, ImgUV[0], ImgUV[1]);

                if (XasixTemp > BgImgLimit.x && XasixTemp < BgImgLimit.y) {
                    ImGui::GetWindowDrawList()->AddLine(
                        ImVec2(ImGui::GetWindowPos().x + XasixTemp + IMGUI_ITEM_SPACING, ImGui::GetWindowPos().y + IMGUI_ITEM_SPACING * 5.5f),
                        ImVec2(ImGui::GetWindowPos().x + XasixTemp + IMGUI_ITEM_SPACING, ImGui::GetWindowPos().y + IMGUI_ITEM_SPACING * 5.5f + TmpSquareImageLen * 0.12f),
                        IM_COL32(0, 255, 240, 255), 1.2f
                    );
                }

                if (Render.Position < RenderBgSize.x / 2.0f && (Render.Position + TmpSquareImageLen) > RenderBgSize.x / 2.0f) {

                    TmpHeadlining = Render.ItemPrizename + "  [" + to_string(Render.ItemUnique) + "]";
                    PrizeTemp = Render;
                }
            }
            ImVec2 CurTmp = ImGui::GetCursorPos();

            // 文字居中.
            ImGui::SetCursorPos(ImVec2(
                IMGUI_ITEM_SPACING + RenderBgSize.x / 2.0f - ImGui::CalcTextSize(TmpHeadlining.c_str()).x / 2.0f,
                RenderBgSize.y + IMGUI_ITEM_SPACING * 5.0f
            ));

            ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 0.92f), TmpHeadlining.c_str());
            ImGui::SetCursorPos(CurTmp);

            // Draw center line.

            ImGui::GetWindowDrawList()->AddLine(
                ImVec2(
                    ImGui::GetCursorScreenPos().x + RenderBgSize.x / 2.0f,
                    PosTemp.y + IMGUI_ITEM_SPACING * 0.5f
                ),
                ImVec2(
                    ImGui::GetCursorScreenPos().x + RenderBgSize.x / 2.0f,
                    ImGui::GetCursorScreenPos().y + IMGUI_ITEM_SPACING * 0.5f
                ),
                IM_COL32(
                    ImU32(CenterLineColor.x * 255.0f),
                    ImU32(CenterLineColor.y * 255.0f),
                    ImU32(CenterLineColor.z * 255.0f),
                    ImU32(CenterLineColor.w * 255.0f)
                ),
                CenterLineWidth
            );

            ImGui::SetCursorPos(ImVec2(IMGUI_ITEM_SPACING, RenderBgSize.y + IMGUI_ITEM_SPACING * 5.0f));

            if (ImGui::Button(PrizePoolViewButtonTitle.c_str()))
                FlagViewPrize = !FlagViewPrize;

            ImGui::SameLine();
            if (ImGui::Button(PrizePoolViewWinPoolTitle.c_str()))
                FlagViewWinPrize = !FlagViewWinPrize;

            ImGui::SameLine();
            ImGui::Text("Speed: %0.3f", MoveVector);

            if (FlagViewPrize)    secondary_window(RunnerItems, PrizePoolViewButtonTitle.c_str(), FlagViewPrize);
            if (FlagViewWinPrize) secondary_window(WinPrizePool, PrizePoolViewWinPoolTitle.c_str(), FlagViewWinPrize, false);

            if (MoveVector <= 0.0f && FlagLotteryDraw)
                WinPrizePool.push_back(PrizeTemp);

            if (MoveVector <= 0.0f)
                FlagLotteryDraw = false; // 抽奖完成.

            CalcRunnerStatus(1.0f, IMGUI_ITEM_SPACING * 3.0f - TmpSquareImageLen, TmpSquareImageLen);
            ImGui::SetCursorPos(ImVec2(IMGUI_ITEM_SPACING, RenderBgSize.y + IMGUI_ITEM_SPACING * 9.0f));

            ExtFunc();
        }
        ImGui::End();

        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    void ImLotteryDraw::ImLD_Init() {
        CreateRunner(0.0f, TmpSquareImageLen, 1);
    }
}