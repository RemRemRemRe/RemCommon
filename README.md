[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/RemRemRemRe/RemCommon)

# Features

## Latent timer

An alternative timer implementation for `FTimerManager`, a [seperate page](https://remremremre.github.io/posts/Latent-Timer/) is created for it

## RemEnsureCondition and RemEnsureVariable

Use `RemEnsureCondition` to validate a `Condition`, if the condition is `false`, it could **optionally** execute **any number of** `InvalidHandlingStatement` you specified, trigger a breakpoint with `AssertionMacro` (ensure, ensureAlways, etc.) or `log` some message

`RemEnsureVariable` is a "pointer checker version" of `RemEnsureCondition`

### The world between `Let it crash` and `Check everything and do nothing if invalid`

I divide the stages that I can code on into:

1. pre-preprocessor (eg: UBT, UHT)

2. compiler-time

3. editor-time (working with unreal editor)

4. runtime (game is running without unreal editor)

I want to "check everything" in `editor time` to make it crash less or no crash which will bring a `better editor experience` and check unexpected things on site which is `better for debugging`

I want to "let it crash" (zero unnecessary check) in `runtime` to gain bit more performance by disable the checking macro

### RemCheckCondition and RemCheckVariable

These two are just `conditional` alias of `RemEnsureCondition` and `RemEnsureVariable` ATM, but could be implemented in other way if needed

Ideally, I could accomplish things mentioned above by check everything (any `Condition`) that is only needed at editor-time with `RemCheckCondition`, and disable it at runtime, leaving `RemEnsureCondition` unchanged. like:

- null asset reference

- error configurations

- things not matching

I could replace `RemEnsureCondition` with `RemCheckCondition` (replace ensure with check) if I'm sure it's editor-only, or vice verse if problems found in play-test

## Rem::Enum

### Rem::Enum::BitOperation

Give the ability of bitmask to enum class
@see FDetailCustomizationUtilities::EContainerCombination

### Rem::Enum::ERemCardinalDirection

### Rem::Enum::EContainerCombination


## Rem::Property

Some helper functions dealing with `FProperty` series

### Rem::Property::IteratePropertiesOfType

## Rem::LoadAssets, Rem::IsLoaded

Start loading asset(s) of `any soft reference` type

## Rem::Object

Some helper functions dealing with `UObject` or other things

### Rem::Object::ForeachObjectInArray

### Rem::IsValidPtr

Validate object in a smarter way: `IsValid()` for `concept` type, `::IsValid(Object)` for `UObject`

### Rem::GetNetMode

### Rem::IsNetMode

### Rem::is_instance

check if a template is a instance of another template

### FRemNetFilterBits

### GetFirstLocalPlayer, GetFirstLocalPlayerPawn ...

### FRemGameplayTagWithCategory

a gameplay tag wrapper that let you select tag category at editor-time / editing-time, at `per data level`, eg: in data table

## FRemByteWrapper, FRemIntegerWrapper...

### Rem::ArrayToWrapper

## FRemTimelinePositionBase

## Rem::DrawDebug

### Rem::DrawDebug::DrawDebugCrossHair

## Rem::GameplayTag

### Rem::GameplayTag::GetFirstParent, Rem::GameplayTag::TryGetTagFromString

## Rem::ScopeExit

### Rem::ScopeExit::TWeakScopeExit

## Macros

### REM_WITH_DEVELOPMENT_ONLY_CODE

### REM_DEFINE_GETTERS_RETURN_REFERENCE_SIMPLE

### REM_VIRTUAL_WARN

An alternative for PURE_VIRTUAL which will cause unwanted process terminated

### REM_LOG_ROLE, REM_LOG_FUNCTION, REM_LOG_ROLE_FUNCTION

### Macro As Function Overloads

REM_MULTI_MACRO

@see `RemEnsureCondition`

## Star History

<a href="https://star-history.com/#RemCommon/RemCommon&Date">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=RemCommon/RemCommon&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=RemCommon/RemCommon&type=Date" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=RemCommon/RemCommon&type=Date" />
 </picture>
</a>