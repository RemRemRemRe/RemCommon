// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include <concepts>
#include <type_traits>
#include <utility>
#include "UObject/WeakObjectPtr.h"

#include "Macro/RemAssertionMacros.h"
#include "Templates/RemLifetimeBoundPtr.h"
#include "RemNotNull.h"

namespace Rem
{
namespace Private
{
/**
 * @brief Builds the handle, picking the constructor the storage form offers: the compact form stores
 * the liveness only, the general form stores the liveness plus the pointee.
 * @note `TPointee` is `TInterface` or `const TInterface`, `TDerived` is the (possibly const) worker.
 */
template <typename TPointee, typename TLiveness, typename TDerived>
[[nodiscard]] TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, TPointee> MakeLifetimeBoundHandle(TDerived& Object)
{
    using FHandle = TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, TPointee>;

    UObject* const Owner   = Object.GetLifetimeBoundOwner();
    auto* const TypedOwner = Cast<TLiveness>(Owner);

#if REM_WITH_DEVELOPMENT_ONLY_CODE

    // A mismatch means the implementer declared a liveness type that is not its owner's type, so every
    // handle it builds would be invalid. Report it and let the invalid handle flow on. The condition is
    // development-only: in Shipping the null cast below already yields an invalid handle, so the guard
    // would only cost a compare.
    RemCheckCondition(Owner == nullptr || TypedOwner != nullptr);

#endif

    const auto Liveness = TWeakObjectPtr<TLiveness>{TypedOwner};

    if constexpr (FHandle::bCompactStorage)
    {
        // the pointee is derivable from the liveness payload, so nothing else is stored
        return FHandle{Liveness};
    }
    else
    {
        return FHandle{Liveness, MakeNotNull(static_cast<TPointee*>(&Object))};
    }
}
}

/**
 * @brief The interface @p TInterface of @p Object, bound to the object's declared owner.
 * @tparam TLiveness Liveness type of the handle. Defaults to the erased `UObject`, which is the
 *         general storage (liveness plus pointee). Pass the owner type - for a worker that *is* its
 *         own owner, its own type - to get the compact storage when the owner implements the
 *         interface: the pointee is then derivable from the liveness payload with a compile-time
 *         offset, so the handle holds one pointer instead of two.
 * @pre `TInterface` is a base of `TDerived` (asserted at compile time).
 * @note This entry point keeps `UObject` as its default, unlike the CRTP facade below: a USTRUCT
 *       implementer's liveness is never the struct itself, and `TDerived` is deduced last, so it
 *       cannot be the default of a parameter declared before it.
 * @note This is the form a USTRUCT implementer uses: a USTRUCT may list one struct base only and
 * UHT reads an unguarded base as a struct parent, so the interface base goes inside `#if CPP` and
 * the CRTP helper below cannot be used there - a UCLASS gets the accessor as a member instead.
 * @note The owner value comes from the virtual, so `TLiveness` only picks the static type: a
 * mismatched type yields an invalid handle, never a wrong object.
 */
template <typename TInterface, typename TLiveness = UObject, typename TDerived>
[[nodiscard]] TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, TInterface> GetLifetimeBoundObjectPtr(TDerived& Object);

/** @copydoc GetLifetimeBoundObjectPtr() */
template <typename TInterface, typename TLiveness = UObject, typename TDerived>
[[nodiscard]] TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, const TInterface> GetLifetimeBoundObjectPtr(
    const TDerived& Object);

/**
 * @brief CRTP facade for IRemGetLifetimeBoundOwner implementers: the typed accessor on top of
 * the protocol's virtual.
 *
 * @tparam TDerived The implementing class, e.g. `TGetLifetimeBoundObjectPtr<URemMyWorker>`.
 * @tparam TLiveness Liveness type of the handles it hands out; see Rem::GetLifetimeBoundObjectPtr.
 *         Defaults to @p TDerived - the worker whose owner is itself, which is also the compact
 *         storage when the worker implements the interface. Name the owner's type when the owner is
 *         a different object: `AActor` for a component owned by its actor, the erased `UObject` when
 *         any UObject may be the owner.
 *
 * @note An implementer lists two bases, written literally (UHT does not expand a macro in a base
 * list): `IRemGetLifetimeBoundOwnerInterface` (UHT only registers an interface it finds in the base
 * list - one reached through a template base stays invisible to reflection, so
 * `Cast<IRemGetLifetimeBoundOwnerInterface>` would silently return null) and this facade, which adds
 * the typed accessor. The protocol's override is the implementer's own. The liveness stays a template
 * argument because the override is virtual and returns the erased `UObject*`: only the implementer
 * knows the owner's type, and that type is what decides the storage form.
 */
template <typename TDerived, typename TLiveness = TDerived>
class TGetLifetimeBoundObjectPtr
{
public:
    /**
     * @brief This object's interface @p TInterface, bound to this object's owner.
     * @pre `TInterface` is a base of `TDerived` (asserted at compile time).
     */
    template <typename TInterface>
    [[nodiscard]] TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, TInterface> GetLifetimeBoundObjectPtr();

    /** @copydoc GetLifetimeBoundObjectPtr() */
    template <typename TInterface>
    [[nodiscard]] TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, const TInterface> GetLifetimeBoundObjectPtr() const;
};

template <typename TInterface, typename TLiveness, typename TDerived>
TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, TInterface> GetLifetimeBoundObjectPtr(TDerived& Object)
{
    static_assert(std::is_base_of_v<TInterface, TDerived>,
        "TInterface has to be an interface the derived type implements");

    return Private::MakeLifetimeBoundHandle<TInterface, TLiveness>(Object);
}

template <typename TInterface, typename TLiveness, typename TDerived>
TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, const TInterface> GetLifetimeBoundObjectPtr(const TDerived& Object)
{
    static_assert(std::is_base_of_v<TInterface, TDerived>,
        "TInterface has to be an interface the derived type implements");

    return Private::MakeLifetimeBoundHandle<const TInterface, TLiveness>(Object);
}

template <typename TDerived, typename TLiveness>
template <typename TInterface>
TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, TInterface> TGetLifetimeBoundObjectPtr<TDerived, TLiveness>::
GetLifetimeBoundObjectPtr()
{
    return Rem::GetLifetimeBoundObjectPtr<TInterface, TLiveness>(*static_cast<TDerived*>(this));
}

template <typename TDerived, typename TLiveness>
template <typename TInterface>
TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, const TInterface> TGetLifetimeBoundObjectPtr<TDerived, TLiveness>::
GetLifetimeBoundObjectPtr() const
{
    return Rem::GetLifetimeBoundObjectPtr<TInterface, TLiveness>(*static_cast<const TDerived*>(this));
}
}
