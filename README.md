# Features
## EnsureCondition and EnsurePointer
Use `EnsureCondition` to validate a `Condition`, if the condition is `false`, it could **optionally** execute any number of `InvalidHandlingStatement` you specified, trigger a breakpoint with `AssertionMacro` (ensure, ensureAlways, etc...) or log some `Message`

`EnsurePointer` is a "pointer checker version" of `EnsureCondition`

### The world between `Let it crash` and `Check everything and do nothing if invalid`
I divide the stages that I can code on into:
1. pre-preprocessor (eg: UBT, UHT)
2. compiler-time 
3. editor-time (working with unreal editor)
4. runtime (game is running without unreal editor)

I want to "check everything" in editor time to make it crash less or no crash which will bring a better editor experience and check unexpected things on site which is better for debugging

I want to "let it crash" (zero unnecessary check) in runtime to gain bit more performance by disable the checking macro

### CheckCondition and CheckPointer
These two are just alias of `EnsureCondition` and `EnsurePointer`, but could be disabled separately and the intention is different

Ideally, I could accomplish things mentioned above by check every `Condition` that is only needed at editor-time with `CheckCondition`, and disable it at runtime. like:
- null asset reference
- error configurations
- things not matching 

What's more, I could replace `EnsureCondition` with `CheckCondition` at anytime or vice verse if problems found in play-test, the sequence in time of the whole system is complemented, etc..

## Common::BitOperation
Give the ability of bitmask to enum class
@see FDetailCustomizationUtilities::EContainerCombination

## Common::PropertyHelper
Some helper functions dealing with `FProperty` series

### IteratePropertiesOfType

## Common::ObjectStatics
Some helper functions dealing with `UObject`

### ForeachObjectInArray

## Common
Some helper functions dealing with other things

### IsValidPtr

### GetNetMode

### IsNetMode

### TIsInstance
check if a template is a instance of another template

## Macros

### VIRTUAL_WARN
An alternative for PURE_VIRTUAL which will cause unwanted process terminated

### BOOL_TO_STRING

### UE_LOG_ROLE, UE_LOG_FUNCTION, UE_LOG_ROLE_FUNCTION

### Macro As Function Overloads
@see `EnsureCondition`
