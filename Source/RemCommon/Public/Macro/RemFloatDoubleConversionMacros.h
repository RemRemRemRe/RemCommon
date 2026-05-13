// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#define REM_ENABLE_FLOAT_DOUBLE_CONVERSION_MACRO 1

#if REM_ENABLE_FLOAT_DOUBLE_CONVERSION_MACRO

#define REM_QUAT_DOUBLE(...) FQuat4d{ __VA_ARGS__ }
#define REM_ROTATOR_DOUBLE(...) FRotator3d{ __VA_ARGS__ }

#define REM_VECTOR2_DOUBLE(...) FVector2d{ __VA_ARGS__ }
#define REM_VECTOR3_DOUBLE(...) FVector3d{ __VA_ARGS__ }
#define REM_VECTOR4_DOUBLE(...) FVecto4d{ __VA_ARGS__ }

#define REM_QUAT_FLOAT(...) FQuat4f{ __VA_ARGS__ }
#define REM_ROTATOR_FLOAT(...) FRotator3f{ __VA_ARGS__ }

#define REM_VECTOR2_FLOAT(...) FVector2f{ __VA_ARGS__ }
#define REM_VECTOR3_FLOAT(...) FVector3f{ __VA_ARGS__ }
#define REM_VECTOR4_FLOAT(...) FVector4f{ __VA_ARGS__ }

#else

#define REM_QUAT_DOUBLE(...) __VA_ARGS__
#define REM_ROTATOR_DOUBLE(...) FRotator3d{ __VA_ARGS__ }

#define REM_VECTOR2_DOUBLE(...) __VA_ARGS__
#define REM_VECTOR3_DOUBLE(...) __VA_ARGS__
#define REM_VECTOR4_DOUBLE(...) __VA_ARGS__

#define REM_QUAT_FLOAT(...) __VA_ARGS__
#define REM_ROTATOR_FLOAT(...) FRotator3f{ __VA_ARGS__ }

#define REM_VECTOR2_FLOAT(...) __VA_ARGS__
#define REM_VECTOR3_FLOAT(...) __VA_ARGS__
#define REM_VECTOR4_FLOAT(...) __VA_ARGS__

#endif
