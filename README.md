# imgui-gamelottery
基于imgui的游戏抽奖(类似轮盘)扩展

### 2023-10-09 version 0.0.1
- 极简调用
- 窗口可扩展
- 初期测试版本

### 2023-10-11 version 1.0.0
- "imgui-lottery-draw" 改名为 "imgui-gamelottery"
- 增加主窗口可设置背景图片
- 整理了源码

![IMG](https://github.com/rcszc/imgui-gamelottery/blob/main/gamelottery_test_image)

~~0.0.1A~~ 移到old分支.

## 1.0.0A

1.0.0 抽奖系统初始化部分
```cpp
// Param: 抽奖主窗口标题, 查看"奖池"按钮标题, 查看"赢得的奖品"按钮标题
ImGLO::ImGameLottery::ImGameLottery(const char* WindowTitle, const char* ViewButton, const char* ViewWin);
// 例:
ImGLO::ImGameLottery* Test = new ImGLO::ImGameLottery(u8"测试抽奖", u8"查看奖池", u8"赢得的奖品");
```

1.0.0 抽奖系统放入(Push)"奖品"项, "Unique"如果相同则会Push失败
```cpp
// Param:  "奖品"名称, "奖品"唯一标识, "奖品"图片(OGL纹理句柄), "奖品"简介(在"查看"中左键鼠标可看见)
// Return: 是否加载成功
bool ImGLO::ImGameLottery::PushItem(std::string PrizeName, size_t Unique, uint32_t GLTexture, std::string Description = {});

// 在"PushItem"全部调用完后紧跟:
void ImGLO::ImGameLottery::InitPrizePool();
```

1.0.0 以上是在程序初始化阶段调用, 以下为渲染循环中调用项; 抽奖开始 & 渲染Tick, "RunTick"中的"WinOpen","WinFlags"与ImGui::Begin中的后两个参数相同
```cpp
// 开始抽奖
// Return: False:当前抽奖进行中
bool ImGLO::ImGameLottery::StartLottery();

// Param: 在抽奖主窗口渲染完必要组件后还可以向后添加imgui组件
void ImGLO::ImGameLottery::RunTick(std::function<void()> ExtFunc = {}, bool WinOpen = true, ImGuiWindowFlags WinFlags = NULL);

// 例:
LDobj.RunTick([&LDobj]() {
        if (ImGui::Button("TEST")) {
            LDobj.StartLottery();
        }
    }
);
```

 1.0.0 获取用户抽到的奖品数据, 每次抽奖结束会push_back进去
```cpp
std::vector<PrizeItem> ImGLO::ImGameLottery::WinPrizePool = {};
```

1.0.0 设置主窗口背景纹理
```cpp
// OpenGL 纹理句柄
uint32_t ImGLO::ImGameLottery::WindowBackgroundImg;
```

1.0.0 抽奖通过随机"转盘"负加速度以及一个一定初始速度来实现随机, emmm目前可能还是存在点问题(不清楚)
```cpp
float ImGameLotteryCalc::CalculateTick::Acceleration; // 转盘加速度
float ImGameLotteryCalc::CalculateTick::SpeedVector;  // 转盘速度

// 产生随机数:
random_device create_random;
mt19937 _gen(create_random());
uniform_real_distribution<float> _dis(SRAND_UNIQUE_MIN, SRAND_UNIQUE_MAX);
"SRAND_UNIQUE_MIN","SRAND_UNIQUE_MAX" 在"imgui_gamelottery.hpp"文件中
```

## 0.0.1 附加
"ImGui-LotteryDraw.zip"为VisualStudio2022的项目可以开袋即食玩玩, 早期尝试版本建议不要使用, 在整理框架重写中. (dalao勿喷)

## 1.0.0 附加
"imgui-gamelottery-vs2022.zip"为VisualStudio2022的项目可以开袋即食, 整理完了可以使用, 不定时更新(坑开太多了).
