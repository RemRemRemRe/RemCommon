[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/RemRemRemRe/RemCommon)

Basic utilities needed for a Unreal Engine game 🎮

> **中文版**: [README.zh-CN.md](README.zh-CN.md)

Both language versions are maintained in lockstep — update both in the same
change.

# Highlights ✨

## Latent timer

An alternative timer implementation for `FTimerManager` — a
[dedicated page](https://remremremre.github.io/posts/Latent-Timer/) explains it.

## Math

Angle utilities — `FRemCircularAngleRange` (shortest-angle helpers, wrapping
ranges) — curve evaluation (`Rem::EvaluateCurve01`) and damping helpers in
`RemMath` / `RemMathCore`.

## Gameplay tags

Common-tag operations and tag↔string helpers (`RemGameplayTagStatics`), tag
arrays (`RemGameplayTagArray`), and tags paired with a category, usable as map
keys (`RemGameplayTagWithCategory`).

## Struct & component containers

Owned storage and non-owning views without manual lifetime juggling:
`RemInstancedStructContainer` (owned struct storage), `RemScopedStructContainer`
(call-scoped heterogeneous structs), `RemComponentContainer` (owns components),
`RemComponentViewContainer` (component views over `TConstStructView`).

## Reflection & property iteration

`RemIteratePropertiesOfType` walks `UPROPERTY` members by type — containers and
`FInstancedStruct` recursion included. `RemReflectedFunctionData` /
`RemReflectedFunctionCallData` invoke reflected functions.

## Assertions & guards

`RemCheck*` (developer-error guards) / `RemEnsure*` (runtime-possible states),
`RemOnScopeExitWeak`, `RemNotNull` (non-null `UObject` semantics), `RemCastFn`,
`RemBinarySearch`, `RemInstanceOf` (compile-time instance-of trait).

## Everyday helpers

Asset loading (`RemAssetLoadingStatics`), object statics (`RemObjectStatics`),
async overlap/trace helpers (`RemWorldCollisionAsync`), net-role filter bits
(`RemNetFilterBits`), typed subsystem access (`Rem*Subsystem.inl`), interface
statics (`RemInterfaceStatics`), animation statics (`RemAnimationStatics`),
enum utilities (`RemEnumStatics`, combination enums), concepts
(`RemConcepts` / `RemCppConcepts` / `RemStructConcepts`), core misc
(`RemMisc`, `RemStd.inl`, `RemCoreStatics.inl`), logging (`RemCommonLog`),
ALS integration macros (`RemAlsMacros`).

# Install & dependencies

- Requires the **RemStrongAlias** and **RemFmt** plugins.
- Enable `RemCommon` in your project (Category: Gameplay). The plugin ships a
  Runtime module `RemCommon` and an editor-only test module `RemCommonTest`.

# Build & test

- Tests are BDD specs (`DEFINE_SPEC` + `Describe`/`It`, "should ..." names) in
  the `RemCommonTest` module (`UncookedOnly` — never packaged), grouped under
  the `Rem.Common` prefix.
- Run them headless with the project's automation runner (`Automation RunTests`
  with a `Rem.Common` filter) using the project's editor development
  configuration — the suite covers the whole public API (latent timers, math,
  tags, containers, templates, macros, subsystems).

# Coding standard

[See here 👈](https://github.com/RemRemRemRe/RemSkills/blob/main/rem-cpp-best-practices/SKILL.md)

# License

[MIT](LICENSE) — ALS-related parts carry
[LICENSE-ALS-Refactored](LICENSE-ALS-Refactored).

## Star History

<a href="https://star-history.com/#RemRemRemRe/RemCommon&Date">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=RemRemRemRe/RemCommon&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=RemRemRemRe/RemCommon&type=Date" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=RemRemRemRe/RemCommon&type=Date" />
 </picture>
</a>
