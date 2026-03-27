## VSCode 调试查看寄存器

- 工程使用 **cortex-debug**，`.vscode/launch.json` 中已启用 `showCpuRegisters: true`，启动 `Debug: JLINK` 配置后，左侧 **Run and Debug → VARIABLES → CPU Registers** 会显示核心寄存器。
- `svdFile` 指向仓库内的 `STM32F429.svd`，在 **Peripherals** 视图中可展开外设寄存器。
- 若未看到寄存器视图，可在调试侧栏右上角菜单选择 **Show View → Registers/Peripherals** 重新打开。***
