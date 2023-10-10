// imgui_gamelottery.
#include <chrono>
#include <random>

#include "imgui_gamelottery.hpp"

using namespace std;

namespace ImGLO {

    void ImGameLottery::comp_mouse_windowa(ImVec2 WindowSize, bool Focus, ImVec2 MouseLen) {

        ImGui::SetNextWindowPos(ImVec2(GetImguiIO.MousePos.x + MouseLen.x, GetImguiIO.MousePos.y + MouseLen.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(WindowSize, ImGuiCond_Always);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.16f, 0.16f, 0.16f, 0.95f)); // Push1.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 7.2f);                      // Push2.

        ImGui::Begin("", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

        if (Focus)
            ImGui::SetWindowFocus();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.72f, 0.92f));      // Push3.
    }

    void ImGameLottery::comp_mouse_windowb() {
        ImGui::PopStyleColor(); // Pop3'

        ImGui::End();
        ImGui::PopStyleVar();   // Pop2'
        ImGui::PopStyleColor(); // Pop1'
    }

    void ImGameLottery::comp_window_secondary(std::vector<PrizeItem>& DataIn, const char* Name, bool& OpenFlag, bool View) {
        ImGui::Begin(Name, &OpenFlag);
        {
            uint32_t CountLine = NULL, CountUnique = NULL;
            for (const auto& Handle : DataIn) {

                ImGui::PushID(CountUnique);
                ImGui::BeginGroup();
                {
                    if (View) {
                        ImGui::ImageButton(to_string(CountUnique).c_str(), (void*)(intptr_t)Handle.ItemTexture, ImVec2(TmpSquareImageLen, TmpSquareImageLen));
                        if (ImGui::IsItemHovered() && GetImguiIO.MouseDown[0] && View) {

                            comp_mouse_windowa(ImVec2(192.0f, 192.0f));
                            ImGui::TextWrapped(Handle.ItemDescription.c_str());
                            comp_mouse_windowb();
                        }
                    }
                    else
                        ImGui::Image((void*)(intptr_t)Handle.ItemTexture, ImVec2(TmpSquareImageLen, TmpSquareImageLen));
                    // 计算文字居中.
                    ImGui::Indent((TmpSquareImageLen - ImGui::CalcTextSize(Handle.ItemPrizename.c_str()).x + IMGUI_ITEM_SPACING) / 2.0f);
                    ImGui::Text(Handle.ItemPrizename.c_str());
                }
                ImGui::EndGroup();
                ImGui::PopID();

                CountLine++;
                if (CountLine >= uint32_t(ImGui::GetWindowSize().x / TmpSquareImageLen) - 1)
                    CountLine = NULL;
                else
                    ImGui::SameLine();
                CountUnique++;
            }
        }
        ImGui::End();
    }

    ImGameLottery::ImGameLottery(const char* WindowTitle, const char* ViewButton, const char* ViewWin) {
        MainWindowTitle = WindowTitle;
        PrizePoolViewButtonTitle = ViewButton;
        PrizePoolViewWinTitle = ViewWin;
    }

    bool ImGameLottery::PushItem(std::string PrizeName, size_t Unique, uint32_t GLTexture, std::string Description) {
        bool regflag = true;

        for (const auto& PrizeItem : PrizeItems) {
            if (Unique == PrizeItem.ItemUnique)
                regflag = false;
        }

        if (regflag)
            PrizeItems.push_back(PrizeItem(GLTexture, Unique, PrizeName, Description));
        return regflag;
    }

    bool ImGameLottery::StartLottery() {
        if (!FlagGameLottery) {

            // 真随机性源 =种子=> MT19937 => 随机数(负加速度).
            random_device create_random;
            mt19937 _gen(create_random());
            uniform_real_distribution<float> _dis(SRAND_UNIQUE_MIN, SRAND_UNIQUE_MAX);

            FlagGameLottery = true;
            SpeedVector = 15.0f;
            Acceleration = -_dis(_gen);

            return true;
        }
        else
            return false;
    }

    void ImGameLottery::RunTick(std::function<void()> ExtFunc, bool WinOpen, ImGuiWindowFlags WinFlags) {
        ImVec2    RenderBgSize = BackgroundRectSize;
        PrizeItem PrizeTemp    = {};

        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));

        ImGui::Begin(MainWindowTitle.c_str(), &WinOpen, WinFlags);
        {
            // item width min = 384.0f.
            if (ImGui::GetWindowSize().x >= 384.0f)
                RenderBgSize.x = ImGui::GetWindowSize().x - IMGUI_ITEM_SPACING * 2.0f;
            else
                RenderBgSize.x = 384.0f;
            ImVec2 PosTemp = ImGui::GetCursorScreenPos();

            ImGui::GetWindowDrawList()->AddImage(
                (void*)(intptr_t)WindowBackgroundImg,
                ImGui::GetWindowPos(),
                ImVec2(
                    ImGui::GetWindowPos().x + ImGui::GetWindowSize().x,
                    ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - IMGUI_ITEM_SPACING
                ),
                ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
                IM_COL32(
                    ImU32(255.0f * 0.5f),
                    ImU32(255.0f * 0.5f),
                    ImU32(255.0f * 0.5f),
                    255
                )
            );
            
            // Draw background rect.

            ImGui::GetWindowDrawList()->AddRectFilled(
                ImGui::GetCursorScreenPos(),
                ImVec2(
                    ImGui::GetCursorScreenPos().x + RenderBgSize.x,
                    ImGui::GetCursorScreenPos().y + RenderBgSize.y
                ),
                IM_COL32(
                    ImU32(ImGui::GetStyle().Colors[ImGuiCol_Button].x * 255.0f * 0.32f),
                    ImU32(ImGui::GetStyle().Colors[ImGuiCol_Button].y * 255.0f * 0.32f),
                    ImU32(ImGui::GetStyle().Colors[ImGuiCol_Button].z * 255.0f * 0.32f),
                    ImU32(BackgroundTransparency * 255.0f)
                ),
                ImGui::GetStyle().FrameRounding
            );

            // Draw image texture.

            for (const auto& Render : RunnerItems) {
                float MoveTemp = Render.Position;

                ImVec2 ImgOutSize = {}, ImgOutUV[2] = {};
                ImVec2 BackgroundLimit = ImVec2(IMGUI_ITEM_SPACING * 3.0f, RenderBgSize.x - IMGUI_ITEM_SPACING);

                // 渲染裁剪.
                CalcImgCropping(
                    ImgOutSize,
                    ImgOutUV[0], ImgOutUV[1],
                    MoveTemp,
                    TmpSquareImageLen,
                    BackgroundLimit
                );

                ImGui::SetCursorPos(ImVec2(MoveTemp, IMGUI_ITEM_SPACING * 5.5f));
                ImGui::Image((void*)(intptr_t)Render.ItemTexture, ImgOutSize, ImgOutUV[0], ImgOutUV[1]);

                if (MoveTemp > BackgroundLimit.x && MoveTemp < BackgroundLimit.y) {
                    ImGui::GetWindowDrawList()->AddLine(
                        ImVec2(ImGui::GetWindowPos().x + MoveTemp + IMGUI_ITEM_SPACING, ImGui::GetWindowPos().y + IMGUI_ITEM_SPACING * 5.5f),
                        ImVec2(ImGui::GetWindowPos().x + MoveTemp + IMGUI_ITEM_SPACING, ImGui::GetWindowPos().y + IMGUI_ITEM_SPACING * 5.5f + TmpSquareImageLen * 0.12f),
                        IM_COL32(0, 255, 240, 255), 
                        1.2f
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
            if (ImGui::Button(PrizePoolViewWinTitle.c_str()))
                FlagViewPrizeWin = !FlagViewPrizeWin;

            ImGui::SameLine();
            ImGui::Text("Speed: %0.3f", SpeedVector);

            if (FlagViewPrize)    comp_window_secondary(RunnerItems, PrizePoolViewButtonTitle.c_str(), FlagViewPrize);
            if (FlagViewPrizeWin) comp_window_secondary(WinPrizePool, PrizePoolViewWinTitle.c_str(), FlagViewPrizeWin, false);

            { // 抽奖完成.
                if (SpeedVector <= 0.0f && FlagGameLottery)
                    WinPrizePool.push_back(PrizeTemp);

                if (SpeedVector <= 0.0f)
                    FlagGameLottery = false;
            }

            CalcRunnerStatus(1.0f, IMGUI_ITEM_SPACING * 3.0f - TmpSquareImageLen, TmpSquareImageLen);
            ImGui::SetCursorPos(ImVec2(IMGUI_ITEM_SPACING, RenderBgSize.y + IMGUI_ITEM_SPACING * 9.0f));

            ExtFunc();
        }
        ImGui::End();

        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    void ImGameLottery::InitPrizePool() {
        CreateRunner(0.0f, TmpSquareImageLen, 1);
    }
}