// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "RemGetLifetimeBoundOwnerInterface.generated.h"

/**
 * liveness/identity protocol of a worker object
 *
 * a worker (a tag event source, a team source, a tag mutex source) is held through a
 * Rem::TLifetimeBoundPtr whose liveness is the UObject that bounds its lifetime - typically the
 * actor the worker belongs to. This interface is how the worker declares that object, so a consumer
 * that only has an interface pointer can still build the handle.
 *
 * this protocol and Rem::TGetLifetimeBoundObjectPtr are two cooperating pieces, not one thing:
 * - the interface is the *erased* entry point: one virtual with a uniform signature, reachable with
 *   `Cast<IRemGetLifetimeBoundOwnerInterface>(Cast<UObject>(Anything))`, which is how a factory that only
 *   has a `UObject*`/interface pointer declares the identity;
 * - the CRTP helper is the *typed* accessor on top of it, resolving the interface pointer with a
 *   compile-time `static_cast` (so it works for USTRUCT implementers too, where `Cast<UObject>`
 *   cannot) and keeping the liveness type the implementer declared.
 * They cannot be a single entity: a UINTERFACE cannot be a template (UHT), and a template cannot be
 * reflected (so `Cast<>` would have nothing to find).
 *
 * the uniform signature is deliberate: a class implementing several worker interfaces implements
 * this primitive once, and no two worker interfaces collide on the name.
 */
UINTERFACE(MinimalAPI)
class URemGetLifetimeBoundOwnerInterface : public UInterface
{
    GENERATED_BODY()
};

class REMCOMMON_API IRemGetLifetimeBoundOwnerInterface
{
    GENERATED_BODY()

public:
    /**
     * @brief Identity of the worker: the UObject whose lifetime bounds it, i.e. the payload of the
     * handle a consumer stores.
     * @return Null when the worker has no living owner yet; the handle is then invalid.
     * @note Add this interface plus `Rem::TGetLifetimeBoundObjectPtr` to the base list and
     * override this method. The returned expression has to convert to `UObject*`, which needs the
     * owner's type to be complete: when the header only forward-declares it, declare the override
     * there and define it in the `.cpp`.
     */
    [[nodiscard]] virtual UObject* GetLifetimeBoundOwner() const = 0;
};
