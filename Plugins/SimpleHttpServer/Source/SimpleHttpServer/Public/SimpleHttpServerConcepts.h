#pragma once

#include <concepts>
#include <type_traits>

template <typename T>
concept struct_type = requires
{
	{ std::remove_cvref_t<T>::StaticStruct() } -> std::convertible_to<UScriptStruct*>;
};

template <typename T>
concept delegate_type = requires(T Param)
{
	{ Param.IsBound() } -> std::same_as<bool>;
	Param.Broadcast;
};
