# Features
## RemEnsureCondition and RemEnsureVariable
Use `RemEnsureCondition` to validate a `Condition`, if the condition is `false`, it could **optionally** execute any number of `InvalidHandlingStatement` you specified, trigger a breakpoint with `AssertionMacro` (ensure, ensureAlways, etc...) or log some `Message`

`RemEnsureVariable` is a "pointer checker version" of `RemEnsureCondition`

### The world between `Let it crash` and `Check everything and do nothing if invalid`
I divide the stages that I can code on into:
1. pre-preprocessor (eg: UBT, UHT)
2. compiler-time 
3. editor-time (working with unreal editor)
4. runtime (game is running without unreal editor)

I want to "check everything" in editor time to make it crash less or no crash which will bring a better editor experience and check unexpected things on site which is better for debugging

I want to "let it crash" (zero unnecessary check) in runtime to gain bit more performance by disable the checking macro

### RemCheckCondition and RemCheckVariable
These two are just alias of `RemEnsureCondition` and `RemEnsureVariable`, but could be disabled separately and the intention is different

Ideally, I could accomplish things mentioned above by check every `Condition` that is only needed at editor-time with `RemCheckCondition`, and disable it at runtime. like:
- null asset reference
- error configurations
- things not matching 

What's more, I could replace `RemEnsureCondition` with `RemCheckCondition` at anytime or vice verse if problems found in play-test, the sequence in time of the whole system is complemented, etc..

## Rem::Common::BitOperation
Give the ability of bitmask to enum class
@see FDetailCustomizationUtilities::EContainerCombination

## Rem::Common::PropertyHelper
Some helper functions dealing with `FProperty` series

### IteratePropertiesOfType

## Rem::Common::ObjectStatics
Some helper functions dealing with `UObject`

### ForeachObjectInArray

## Rem::Common
Some helper functions dealing with other things

### IsValidPtr

### GetNetMode

### IsNetMode

### TIsInstance
check if a template is a instance of another template

## Macros

### REM_VIRTUAL_WARN
An alternative for PURE_VIRTUAL which will cause unwanted process terminated

### BOOL_TO_STRING

### REM_LOG_ROLE, REM_LOG_FUNCTION, REM_LOG_ROLE_FUNCTION

### Macro As Function Overloads
@see `RemEnsureCondition`
