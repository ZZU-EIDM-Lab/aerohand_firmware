# Aero Hand Open 固件

这是用于 **Seeed Studio XIAO ESP32S3** 的 Aero Hand Open 固件，基于 [Chestnut Robotics 的开源项目](https://github.com/Chestnut-Robotics/aero-hand-open)改造，并作为独立的 PlatformIO 项目维护。这里仅包含 ESP32 固件；对应的[上位机 SDK 与图形界面](https://github.com/lava-xa/aerohand_sdk)在另一个仓库。

## 当前功能

- 支持左右手构建，默认构建右手版本；舵机总线使用 ID `0`～`6`。
- 上电或欠压复位时自动归位，也可由上位机发送 `HOMING` 命令手动归位。其他类型的复位不会自动归位。
- 归位结束后，拇指外展（通道 `0`）移动到伸直与抓握行程之间的 `0.8` 位置，其余通道移动到伸直位置。
- 支持位置与力矩控制，以及位置、速度、电流和温度读取。默认位置控制力矩上限为 `400`。
- 批量读取时按舵机分别处理回包：某一路无响应不会清空其余六路的读数；无响应通道会在查询响应中标记为无效。

## 硬件与连接

| 项目 | 配置 |
| --- | --- |
| 控制板 | Seeed Studio XIAO ESP32S3 |
| 舵机总线 | `Serial2`，`1,000,000` baud |
| 舵机总线引脚 | XIAO `RX = GPIO2`、`TX = GPIO3` |
| USB 串口 | `921600` baud |
| 舵机 ID | `0`～`6` |

固件通过 PlatformIO 的 `RIGHT_HAND` / `LEFT_HAND` 构建标志选择左右手。**烧录前务必确认构建环境与实际机械手一致。**上电及欠压复位会触发七个舵机归位，请先留出运动空间，避免机械干涉。

## 构建

安装 PlatformIO Core，或在 VS Code 中安装 PlatformIO 扩展后，在本目录运行：

```sh
pio run -e right_hand
pio run -e left_hand
```

不指定环境时，`platformio.ini` 默认选择 `right_hand`。首次构建会按配置下载 ESP32 平台和 `FTServo_Arduino` `2.0.0` 依赖。

每个环境的构建产物位于 `.pio/build/<环境名>/`：

| 文件 | 用途 |
| --- | --- |
| `firmware.bin` | 应用固件，写入地址 `0x10000` |
| `firmware.factory.bin` | 包含引导程序、分区表及应用固件的完整镜像，从地址 `0x0` 写入 |

使用上位机的“烧录固件”功能时，应选择对应环境的 **`firmware.bin`**。该功能按 `0x10000` 地址写入，不能选择 `firmware.factory.bin`。

## 烧录与串口监视

连接对应的机械手后，使用 PlatformIO 烧录：

```sh
pio run -e right_hand -t upload
pio device monitor -b 921600
```

左手将 `right_hand` 改为 `left_hand`。如 PlatformIO 未自动找到串口，可先用 `pio device list` 查看设备，再在上传命令后加 `--upload-port <串口>`。

烧录过程可能使设备复位；固件只在检测到上电或欠压复位时自动归位。如果需要重新归位，可重新上电或使用上位机的“回零”命令。

## 串口协议要点

USB 串口的命令和响应均为 **16 字节**：首字节是操作码，第二字节是保留字节或状态位，后面是七个小端序 16 位数值。位置控制的七路目标值范围为 `0`～`65535`，对应各通道的伸直位置到抓握位置。

| 操作码 | 功能 |
| --- | --- |
| `0x01` | 手动归位 `HOMING` |
| `0x11` / `0x12` | 位置控制 `CTRL_POS` / 力矩控制 `CTRL_TOR` |
| `0x22` / `0x23` | 读取位置 `GET_POS` / 速度 `GET_VEL` |
| `0x24` / `0x25` | 读取电流 `GET_CURR` / 温度 `GET_TEMP` |
| `0x31` / `0x32` | 设置速度 `SET_SPE` / 力矩 `SET_TOR` |

四种读取响应的第二字节使用有效位：最高位 `bit 7` 为 `1` 表示该响应带有效位掩码；`bit 0`～`bit 6` 分别表示七个舵机是否成功回包。无响应通道的数据槽填 `0`，应根据有效位判断，不能把它当作真实测量值。其余响应仍使用原有格式。配套上位机会把无响应通道显示为“离线”。

## 来源与许可

本项目由上游仓库的 `firmware/main` 固件改造而来，迁移时参考的上游提交为 [`7bb2f23485a7d440960c21046da642de132b7745`](https://github.com/Chestnut-Robotics/aero-hand-open/commit/7bb2f23485a7d440960c21046da642de132b7745)。原项目的[文档](https://chestnut-robotics.github.io/aero-hand-open/)可用于了解硬件与基本操作。本仓库中的归位姿态、默认力矩和逐路读取行为为后续修改，不能与上游预编译固件混用说明。

本项目沿用 Apache License 2.0，详见 [LICENSE](LICENSE)。
