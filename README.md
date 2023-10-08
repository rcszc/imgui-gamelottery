# imgui-lottery-draw
基于imgui的抽奖(类似轮盘)扩展

### 2023-10-09 version 0.0.1
- 极简调用
- 窗口可扩展
- 初期测试版本

## 0.0.1A

0.0.1 抽奖系统初始化部分
```cpp
// Param: 抽奖主窗口标题, 查看"奖池"按钮标题, 查看"赢得的奖品"按钮标题
ImLD::ImLotteryDraw::ImLotteryDraw(const char* WindowTitle, const char* ViewButton, const char* ViewWin);
// 例:
ImLD::ImLotteryDraw* Test = new ImLD::ImLotteryDraw(u8"测试抽奖", u8"查看奖池", u8"赢得的奖品");
```
0.0.1 抽奖系统放入(Push)"奖品"项, "Unique"如果相同则会Push失败
```cpp
// Param: "奖品"名称, "奖品"唯一标识, "奖品"图片(OGL纹理句柄), "奖品"简介(在"查看"中左键鼠标可看见)
// Return: 是否加载成功
bool ImLotteryDraw::ImLD_PushItem(std::string PrizeName, size_t Unique, uint32_t GLTexture, std::string Description = {});

// 在"ImLD_PushItem"全部调用完后紧跟
void ImLotteryDraw::ImLD_Init();
```
0.0.1 以上是在程序初始化阶段调用, 以下为渲染循环中调用项
```cpp
// 开始抽奖
// Return: False:当前抽奖进行中
bool ImLotteryDraw::ImLD_StartLottery();

// Param: 在抽奖主窗口渲染完必要组件后还可以向后添加imgui组件
void ImLotteryDraw::ImLD_RunTick(std::function<void()> ExtFunc = {});
// 例:
LDobj.ImLD_RunTick([&LDobj]() {          
    if (ImGui::Button("TEST")) {
        LDobj.ImLD_StartLottery();
    }
  }
);

## 0.0.1 附加
早期尝试版本建议不要使用, 在整理框架重写中. (dalao勿喷)
```
