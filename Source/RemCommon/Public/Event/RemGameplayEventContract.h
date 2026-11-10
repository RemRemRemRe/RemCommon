// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Enum/RemLifecyclePhase.h"
#include "GameplayTagContainer.h"
#include "Math/Vector.h"
#include "Templates/SubclassOf.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "RemGameplayEventContract.generated.h"

class AActor;

/**
 * The payload contract shared by event producers and their consumers.
 *
 * A presentation module builds against these types only: it receives an event and reads its
 * lifecycle and spatial slices without linking the gameplay module that produced it. Rich payloads
 * stay in the module that owns them - damage details in the damage module, a projectile's flight
 * data in the projectile module - and travel beside these contracts, never inside them, so one
 * consumer's data does not become every consumer's dependency.
 */

/**
 * identity and lifecycle of one logically continuous event (one skill cast, one damage-over-time instance,
 * one projectile flight). The producer allocates the id; consumers only pair events by it.
 */
USTRUCT(BlueprintType)
struct REMCOMMON_API FRemEventLifecycle
{
    GENERATED_BODY()

    /**
     * unique per logical event within one session; 0 is never used
     *
     * @note no specifiers: UHT has no Blueprint type for a 32-bit unsigned value, and it rejects a Category
     * on a property that is not editor- or Blueprint-visible. Producers and consumers exchange the id in C++.
     */
    UPROPERTY()
    uint32 EventId{};

    UPROPERTY(BlueprintReadWrite, Category = "Rem")
    ERemLifecyclePhase Phase{ERemLifecyclePhase::Start};
};

/** where an event happened and who it involved: the presentation-neutral half of any event's payload */
USTRUCT(BlueprintType)
struct REMCOMMON_API FRemEventSpatial
{
    GENERATED_BODY()

    /** world location the event resolved at */
    UPROPERTY(BlueprintReadWrite, Category = "Rem")
    FVector Location{ForceInitToZero};

    /** surface normal at Location; zero when the event has no surface */
    UPROPERTY(BlueprintReadWrite, Category = "Rem")
    FVector Normal{ForceInitToZero};

    /** who caused the event (caster, projectile's owner); may be null */
    UPROPERTY(BlueprintReadWrite, Category = "Rem")
    TWeakObjectPtr<AActor> Source;

    /** who the event happened to (the victim, the effect's target); may be null */
    UPROPERTY(BlueprintReadWrite, Category = "Rem")
    TWeakObjectPtr<AActor> Target;
};

/**
 * what an event is about, in the terms every module can name: an instance and its kind
 *
 * a consumer that selects on "which ability / which actor class / which projectile" reads this slice, so it needs
 * nothing from the module that produced the event. A producer fills it whenever the event has a subject a
 * presentation could care about.
 */
USTRUCT(BlueprintType)
struct REMCOMMON_API FRemEventSubject
{
    GENERATED_BODY()

    /** the instance: the caster of an ability, the actor that appeared, the projectile in flight, the victim */
    UPROPERTY(BlueprintReadWrite, Category = "Rem")
    TWeakObjectPtr<UObject> Instance;

    /** the kind behind that instance - an ability class, an actor class, a projectile class */
    UPROPERTY(BlueprintReadWrite, Category = "Rem")
    TSubclassOf<UObject> Class;
};

/**
 * the tags a producer attaches to describe what kind of event this is - the combo stage, the damage type, ...
 *
 * optional: a consumer that selects on it reads this slice, so it still needs nothing from the module that
 * produced the event. Producers that have nothing to classify leave it out.
 */
USTRUCT(BlueprintType)
struct REMCOMMON_API FRemEventTags
{
    GENERATED_BODY()

    /** the event's own classification tags; empty when the producer names none */
    UPROPERTY(BlueprintReadWrite, Category = "Rem")
    FGameplayTagContainer Tags;
};

/**
 * allocate an event identity; never 0, and unique within the process
 *
 * the producer allocates one per logical event and carries it in FRemEventLifecycle, so consumers pair a
 * start with its pause/resume/stop without knowing anything about the producer
 */
REMCOMMON_API [[nodiscard]] uint32 AllocateGameplayEventId();
