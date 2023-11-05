
#pragma once

namespace Rem::Common
{

template <typename T>
constexpr auto GetTypeName() {
	// https://stackoverflow.com/a/56766138
	std::string_view Name, Prefix, Suffix;
#ifdef __clang__
	name = __PRETTY_FUNCTION__;
	prefix = "auto GetTypeName() [T = ";
	suffix = "]";
#elif defined(__GNUC__)
	name = __PRETTY_FUNCTION__;
	prefix = "constexpr auto GetTypeName() [with T = ";
	suffix = "]";
#elif defined(_MSC_VER)
	Name = __FUNCSIG__;
	Prefix = "auto __cdecl GetTypeName<";
	Suffix = ">(void)";
#endif
	Name.remove_prefix(Prefix.size());
	Name.remove_suffix(Suffix.size());
	return Name;
}

}
