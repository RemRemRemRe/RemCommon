[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/RemRemRemRe/RemCommon)

制作 Unreal Engine 游戏所需的基础工具 🎮

> **English version**: [README.md](README.md)

两份语言版本保持精确同步 —— 在同一改动中同步更新。

# 功能亮点 ✨

## 潜伏定时器（Latent timer）

`FTimerManager` 的替代定时器实现 —— 有[专门页面](https://remremremre.github.io/posts/Latent-Timer/)介绍。

## 数学

角度工具 —— `FRemCircularAngleRange`（最短角辅助、环绕范围）；曲线求值
（`Rem::EvaluateCurve01`）与阻尼辅助在 `RemMath` / `RemMathCore`。

## Gameplay 标签

公共 tag 运算与 tag↔字符串互转（`RemGameplayTagStatics`）、tag 数组
（`RemGameplayTagArray`）、带分类的 tag（可作 map 键，`RemGameplayTagWithCategory`）。

## 结构体与组件容器

无需手工管理生命周期的托管存储与视图：`RemInstancedStructContainer`（托管结构体存储）、
`RemScopedStructContainer`（调用作用域内的异构结构体）、`RemComponentContainer`
（托管组件）、`RemComponentViewContainer`（基于 `TConstStructView` 的组件视图）。

## 反射与属性遍历

`RemIteratePropertiesOfType` 按类型遍历 `UPROPERTY` 成员 —— 含容器与
`FInstancedStruct` 递归。`RemReflectedFunctionData` / `RemReflectedFunctionCallData`
调用反射函数。

## 断言与守卫

`RemCheck*`（开发者错误守卫）/ `RemEnsure*`（运行时可达状态）、`RemOnScopeExitWeak`、
`RemNotNull`（非空 `UObject` 语义）、`RemCastFn`、`RemBinarySearch`、`RemInstanceOf`
（编译期 instance-of trait）。

## 日常辅助

资产加载（`RemAssetLoadingStatics`）、对象 statics（`RemObjectStatics`）、异步
overlap/trace（`RemWorldCollisionAsync`）、网络角色过滤位（`RemNetFilterBits`）、
类型化子系统访问（`Rem*Subsystem.inl`）、接口 statics（`RemInterfaceStatics`）、
动画 statics（`RemAnimationStatics`）、枚举工具（`RemEnumStatics`、组合枚举）、
concepts（`RemConcepts` / `RemCppConcepts` / `RemStructConcepts`）、核心杂项
（`RemMisc`、`RemStd.inl`、`RemCoreStatics.inl`）、日志（`RemCommonLog`）、
ALS 集成宏（`RemAlsMacros`）。

# 安装与依赖

- 需要 **RemStrongAlias** 与 **RemFmt** 插件。
- 在项目中启用 `RemCommon`（分类：Gameplay）。插件包含 Runtime 模块 `RemCommon`
  与仅编辑器测试模块 `RemCommonTest`。

# 构建与测试

- 测试为 BDD spec（`DEFINE_SPEC` + `Describe`/`It`，"should ..." 命名），位于
  `RemCommonTest` 模块（`UncookedOnly` —— 永不打进包），统一使用 `Rem.Common` 前缀。
- 用项目的自动化测试运行器无头运行（`Automation RunTests` + `Rem.Common` 过滤），
  使用项目的编辑器开发配置 —— 套件覆盖整个公共 API（潜伏定时器、数学、标签、
  容器、模板、宏、子系统）。

# 编码规范

[See here 👈](https://github.com/RemRemRemRe/RemSkills/blob/main/rem-cpp-best-practices/SKILL.md)

# License

[MIT](LICENSE) —— 与 ALS 相关的部分遵循
[LICENSE-ALS-Refactored](LICENSE-ALS-Refactored)。

## Star History

<a href="https://star-history.com/#RemRemRemRe/RemCommon&Date">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=RemRemRemRe/RemCommon&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=RemRemRemRe/RemCommon&type=Date" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=RemRemRemRe/RemCommon&type=Date" />
 </picture>
</a>
