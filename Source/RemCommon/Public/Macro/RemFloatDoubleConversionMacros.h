// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#define REM_ENABLE_FLOAT_DOUBLE_CONVERSION_MACRO 1

#if REM_ENABLE_FLOAT_DOUBLE_CONVERSION_MACRO

#define REM_QUAT_DOUBLE(...) FQuat4d{ __VA_ARGS__ }

#define REM_VECTOR2_DOUBLE(...) FVector2d{ __VA_ARGS__ }
#define REM_VECTOR3_DOUBLE(...) FVector3d{ __VA_ARGS__ }
#define REM_VECTOR4_DOUBLE(...) FVecto4d{ __VA_ARGS__ }

#define REM_QUAT_FLOAT(...) FQuat4f{ __VA_ARGS__ }

#define REM_VECTOR2_FLOAT(...) FVector2f{ __VA_ARGS__ }
#define REM_VECTOR3_FLOAT(...) FVector3f{ __VA_ARGS__ }
#define REM_VECTOR4_FLOAT(...) FVector4f{ __VA_ARGS__ }

#else

#define REM_DOUBLE_QUAT(...) __VA_ARGS__

#define REM_DOUBLE_VECTOR2(...) __VA_ARGS__
#define REM_DOUBLE_VECTOR3(...) __VA_ARGS__
#define REM_DOUBLE_VECTOR4(...) __VA_ARGS__

#define REM_FLOAT_QUAT(...) __VA_ARGS__

#define REM_FLOAT_VECTOR2(...) __VA_ARGS__
#define REM_FLOAT_VECTOR3(...) __VA_ARGS__
#define REM_FLOAT_VECTOR4(...) __VA_ARGS__

#endif
