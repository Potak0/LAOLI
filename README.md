# LAOLI - STM32 MP3 Player Controller

[English](#english) | [中文](#中文)

---

## English

A STM32L031K6U6-based MP3 player control system with VFD display and rotary encoder control functionality.

### Project Overview

LAOLI is an embedded firmware project for controlling MP3 player modules and displaying playback information through a VFD (Vacuum Fluorescent Display). The system supports user interaction via rotary encoder and communicates with MP3 modules through UART.

### Key Features

- 🎵 MP3 player control (play/pause/previous/next)
- 📺 8-character VFD display control
- 🎛️ Rotary encoder input processing  
- 🔊 Volume control
- 📡 UART communication protocol
- ⚡ Low-power design (STM32L0 series)

### Quick Start

1. **Prerequisites**: ARM GCC toolchain, OpenOCD, ST-Link debugger
2. **Clone**: `git clone https://github.com/Potak0/LAOLI.git`
3. **Build**: `make clean && make -j4`
4. **Flash**: `openocd -f openocd.cfg -c "program build/aaalaoli.elf verify reset exit"`

### Hardware Connections

| STM32 Pin | Function | Description |
|-----------|----------|-------------|
| PA5 | VFD CLK | Clock signal |
| PA6 | VFD CS | Chip select |
| PA7 | VFD DIN | Data input |
| PA15 | VFD EN | Enable signal |
| PB0 | VFD RESET | Reset signal |
| PA2/PA3 | UART TX/RX | MP3 module communication |
| PA0/PA1 | Encoder A/B | Rotary encoder inputs |

For detailed documentation, see the Chinese section below.

---

## 中文

# LAOLI - STM32 MP3播放器控制器

一个基于STM32L031K6U6微控制器的MP3播放器控制系统，具有VFD显示屏和旋转编码器控制功能。

## 项目概述

LAOLI是一个嵌入式固件项目，用于控制MP3播放器模块并通过VFD (真空荧光显示器) 显示播放信息。该系统支持通过旋转编码器进行用户交互，并通过UART与MP3模块通信。

### 主要功能

- 🎵 MP3播放器控制（播放/暂停/上一曲/下一曲）
- 📺 8字符VFD显示屏控制
- 🎛️ 旋转编码器输入处理
- 🔊 音量控制
- 📡 UART通信协议
- ⚡ 低功耗设计（STM32L0系列）

## 硬件规格

### 微控制器
- **MCU**: STM32L031K6U6
- **架构**: ARM Cortex-M0+ 32位
- **封装**: UFQFPN32
- **闪存**: 32KB
- **SRAM**: 8KB
- **工作频率**: 最高32MHz

### 外设接口
- **VFD控制器**: S1201芯片（通过GPIO模拟SPI）
- **UART**: 9600波特率，用于MP3模块通信
- **定时器**: TIM2配置为编码器模式
- **DMA**: 用于UART接收
- **GPIO**: 用于LED指示、功放控制等

## 硬件连接

### VFD显示屏连接 (S1201控制器)
| STM32引脚 | VFD信号 | 功能 | 电气特性 |
|-----------|---------|------|----------|
| PA5 | CLK | 时钟信号 | 3.3V逻辑，上升沿有效 |
| PA6 | CS | 片选信号 | 3.3V逻辑，低电平有效 |
| PA7 | DIN | 数据输入 | 3.3V逻辑，MSB先传输 |
| PA15 | EN | 使能信号 | 3.3V逻辑，高电平使能 |
| PB0 | RESET | 复位信号 | 3.3V逻辑，低电平复位 |

**VFD电源要求:**
- VDD: 5V (显示管高压电源)
- VCC: 3.3V (逻辑电源)
- 电流消耗: 约50-200mA (取决于显示内容和亮度)

### UART连接（MP3模块）
| STM32引脚 | 信号 | 功能 | 配置 |
|-----------|------|------|------|
| PA2 | TX | 发送数据到MP3模块 | 9600-8-N-1 |
| PA3 | RX | 接收MP3模块数据 | DMA支持 |

**推荐MP3模块:** DFPlayer Mini 或兼容模块
- 工作电压: 3.2V-5.0V
- 支持格式: MP3, WAV
- 存储: Micro SD卡

### 旋转编码器
| STM32引脚 | 信号 | 功能 | 配置 |
|-----------|------|------|------|
| PA0 | ENC_A | 编码器A相 | TIM2_CH1，上拉输入 |
| PA1 | ENC_B | 编码器B相 | TIM2_CH2，上拉输入 |

**编码器规格:**
- 类型: 增量式旋转编码器
- 脉冲数: 20-30 PPR (推荐)
- 输出: 开集电极或推挽输出

### 功放和电源控制
| STM32引脚 | 功能 | 说明 |
|-----------|------|------|
| PB3 | 功放控制1 | 低电平有效 |
| PB4 | 功放控制2 | 低电平有效 |
| PB5 | 电源控制 | 高电平使能 |

### LED指示灯
| STM32引脚 | 功能 | 颜色建议 |
|-----------|------|----------|
| PB6 | 系统状态LED | 绿色 |
| PB7 | 工作状态LED | 蓝色 |

### 电路保护建议
- 在所有GPIO输出端串联220Ω限流电阻
- VFD高压电源添加滤波电容 (100μF + 100nF)
- MCU电源添加去耦电容 (100nF靠近VDD引脚)
- 添加ESD保护二极管到暴露的接口

## 开发环境设置

### 必要工具
1. **STM32CubeMX** - 硬件配置工具
2. **Visual Studio Code** - 主要IDE
3. **STM32 for VS Code扩展** - STM32开发支持
4. **ARM GCC工具链** - 编译器
5. **OpenOCD** - 调试和烧录工具
6. **ST-Link** - 硬件调试器

### 环境配置

#### 1. 安装VS Code和扩展
```bash
# 安装STM32 for VS Code扩展
# 在VS Code中搜索"STM32 for VSCode"并安装
```

#### 2. 安装ARM GCC工具链
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install gcc-arm-none-eabi

# macOS
brew install --cask gcc-arm-embedded

# Windows
# 下载并安装GNU Arm Embedded Toolchain
```

#### 3. 安装OpenOCD
```bash
# Ubuntu/Debian
sudo apt install openocd

# macOS
brew install openocd

# Windows
# 下载并安装OpenOCD for Windows
```

## 编译和烧录

### 编译项目
```bash
# 克隆项目
git clone https://github.com/Potak0/LAOLI.git
cd LAOLI

# 编译
make clean
make -j4

# 生成的文件位于build/目录
```

### 烧录程序
```bash
# 使用OpenOCD烧录
openocd -f openocd.cfg -c "program build/aaalaoli.elf verify reset exit"
```

### VS Code调试
1. 打开项目文件夹
2. 按F5开始调试
3. 选择"Debug Microcontroller"配置

## 软件架构

### 文件结构
```
LAOLI/
├── Src/                    # 源代码
│   ├── main.c             # 主程序
│   ├── vfd.c              # VFD显示屏驱动
│   ├── stm32l0xx_it.c     # 中断处理
│   ├── stm32l0xx_hal_msp.c # HAL MSP配置
│   └── system_stm32l0xx.c # 系统配置
├── Inc/                    # 头文件
│   ├── main.h             # 主头文件
│   ├── vfd.h              # VFD驱动头文件
│   └── stm32l0xx_hal_conf.h # HAL配置
├── Drivers/               # STM32 HAL驱动
├── build/                 # 编译输出（gitignore）
├── Makefile              # 构建脚本
├── aaalaoli.ioc          # STM32CubeMX配置
├── openocd.cfg           # OpenOCD配置
└── README.md             # 项目文档
```

### 核心模块

#### 1. MP3控制模块
支持的命令：
- 播放/暂停控制
- 上一曲/下一曲
- 音量调节
- 播放模式设置
- 状态查询

#### 2. VFD显示模块
- S1201控制器驱动
- 8字符文本显示
- 亮度控制
- 滚动文本支持

#### 3. 用户交互模块
- 旋转编码器输入检测
- 按键处理
- LED状态指示

## MP3协议说明

系统使用自定义UART协议与MP3模块通信：

### 命令格式
```c
// 播放命令
{0x04, 0xFB, 0x01, 0x01, 0x01}

// 暂停命令  
{0x04, 0xFB, 0x01, 0x02, 0x02}

// 音量设置
{0x06, 0xF9, 0x02, 0x01, volume, checksum}
```

### 状态查询
系统会循环查询以下状态：
1. 播放模式
2. 当前曲目
3. 曲目名称
4. 播放时间
5. 音量级别

## 使用说明

### 基本操作
1. **上电启动**: 系统自动初始化VFD显示和MP3模块
2. **显示测试**: 启动后显示"ABCDEFGH"测试字符
3. **音乐播放**: 自动切换到SD卡模式并开始播放
4. **旋转控制**: 使用编码器控制音量或选择曲目
5. **状态显示**: VFD显示当前播放信息

### LED指示
- **PB6**: 系统状态指示灯（闪烁）
- **PB7**: 工作状态指示灯（闪烁）

## 自定义和扩展

### 添加新功能
1. 在`main.c`中添加新的命令定义
2. 在主循环中添加相应的处理逻辑
3. 更新VFD显示内容

### 修改显示内容
编辑`vfd.c`中的显示函数：
```c
// 显示自定义文本
S1201_WriteStr(0, "YOUR TEXT");
S1201_show();
```

### 更改通信协议
在`main.c`中修改命令数组和通信函数。

## 故障排除

### 常见问题

**Q: VFD显示无内容**
- 检查VFD电源连接
- 确认GPIO连接正确
- 检查使能信号（PA15）

**Q: MP3模块无响应**
- 检查UART连接（PA2/PA3）
- 确认波特率设置（9600）
- 检查MP3模块电源

**Q: 编码器无反应**
- 检查TIM2配置
- 确认编码器连接（PA0/PA1）
- 检查编码器电源

**Q: 编译错误**
- 确认ARM GCC工具链安装
- 检查路径配置
- 更新HAL库版本

### 调试技巧
1. 使用VS Code调试器设置断点
2. 通过UART输出调试信息
3. 使用示波器检查信号时序
4. 检查系统时钟配置

## 性能优化

### 功耗优化
- 使用STM32L0低功耗特性
- 配置适当的时钟频率
- 在空闲时进入睡眠模式

### 代码优化
- 优化显示更新频率
- 减少不必要的UART通信
- 使用DMA减少CPU负载

## 贡献指南

### 开发规范
1. 遵循现有的代码风格
2. 添加适当的注释
3. 测试新功能的兼容性
4. 更新相关文档

### 提交流程
1. Fork项目仓库
2. 创建功能分支
3. 提交更改并测试
4. 创建Pull Request

## 许可证

本项目基于STMicroelectronics的许可证条款开发。详细信息请参考源代码中的许可证声明。

## 联系方式

如有问题或建议，请通过GitHub Issues联系。

---

## 代码示例

### VFD显示文本
```c
// 初始化VFD
VFD_init();

// 显示8个字符
S1201_WriteStr(0, "HELLO123");
S1201_show();

// 逐个字符显示
for(int i = 0; i < 8; i++) {
    S1201_WriteOneChar(i, '0' + i);
}
S1201_show();
```

### MP3控制命令
```c
// 播放音乐
HAL_UART_Transmit(&huart2, CMD_SET_PLAY, sizeof(CMD_SET_PLAY), HAL_MAX_DELAY);

// 暂停播放
HAL_UART_Transmit(&huart2, CMD_SET_PAUSE, sizeof(CMD_SET_PAUSE), HAL_MAX_DELAY);

// 调节音量 (0-30)
uint8_t volume_cmd[] = {0x06, 0xF9, 0x02, 0x01, 0x15, 0x17}; // 音量21
HAL_UART_Transmit(&huart2, volume_cmd, sizeof(volume_cmd), HAL_MAX_DELAY);
```

### 读取编码器
```c
// 获取编码器计数
int encoder_count = (short)__HAL_TIM_GET_COUNTER(&htim2);
int direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? -1 : 1;

// 处理编码器变化
if(encoder_count != last_count) {
    if(direction > 0) {
        // 顺时针旋转 - 增加音量
        volume_up();
    } else {
        // 逆时针旋转 - 减少音量
        volume_down();
    }
    last_count = encoder_count;
}
```

## 调试和开发

### VS Code调试配置
项目已配置VS Code调试环境：
1. 安装Cortex-Debug扩展
2. 连接ST-Link调试器
3. 按F5开始调试

### 关键调试点
- `main()` 函数开始处
- `VFD_init()` 初始化完成后
- UART发送/接收回调函数
- 编码器中断处理

### 日志输出
如需添加调试输出，可通过UART发送：
```c
char debug_msg[] = "Debug: System initialized\r\n";
HAL_UART_Transmit(&huart2, (uint8_t*)debug_msg, strlen(debug_msg), HAL_MAX_DELAY);
```

### 常用调试命令
```bash
# 编译并查看大小
make clean && make -j4 && arm-none-eabi-size build/aaalaoli.elf

# 查看内存映射
arm-none-eabi-objdump -h build/aaalaoli.elf

# 反汇编代码
arm-none-eabi-objdump -d build/aaalaoli.elf > disassembly.txt
```

## 版本历史

- **v1.0**: 初始版本，基本MP3控制和VFD显示功能
- 支持的STM32CubeMX版本: 6.14.1
- 支持的HAL库版本: STM32L0xx HAL Driver

## 技术参数

### 系统规格
- **工作电压**: 3.3V
- **工作温度**: -40°C 到 +85°C
- **时钟频率**: 16MHz HSI
- **UART波特率**: 9600 bps
- **VFD刷新率**: ~200ms循环

### 内存使用
- **Flash使用**: ~15KB (约47%)
- **RAM使用**: ~2KB (约25%)
- **DMA通道**: 1个 (USART2_RX)

## 相关资源

### 文档链接
- [STM32L031K6U6数据手册](https://www.st.com/resource/en/datasheet/stm32l031k6.pdf)
- [STM32L0 HAL用户手册](https://www.st.com/resource/en/user_manual/um1749-description-of-stm32l0-hal-and-lowlayer-drivers-stmicroelectronics.pdf)
- [STM32CubeMX用户指南](https://www.st.com/resource/en/user_manual/um1718-stm32cubemx-for-stm32-configuration-and-initialization-c-code-generation-stmicroelectronics.pdf)

### 开发工具
- [STM32CubeMX下载](https://www.st.com/zh/development-tools/stm32cubemx.html)
- [STM32 for VS Code扩展](https://marketplace.visualstudio.com/items?itemName=stmicroelectronics.stm32-vscode-extension)
- [ARM GCC工具链](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)

## 更新日志

### 最新更改
- 优化VFD显示刷新逻辑
- 改进MP3模块通信协议
- 添加DMA接收支持
- 增强编码器响应速度

### 已知问题
- 编码器在快速旋转时可能丢失脉冲
- VFD显示在低温下可能出现延迟
- 长时间运行后UART可能需要重新初始化

### 后续计划
- [ ] 添加蓝牙控制支持
- [ ] 实现LCD显示替代方案
- [ ] 优化功耗管理
- [ ] 添加OTA更新功能

**注意**: 本项目为嵌入式开发项目，需要相应的硬件设备进行测试和验证。在进行任何硬件连接之前，请仔细核对引脚定义和电压等级。