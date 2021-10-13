// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SpawnBulletAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSpawnBulletAnimNotifySignature);

/**
 * 
 */
UCLASS()
class PIRATEPUZZLE_API USpawnBulletAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
    FOnSpawnBulletAnimNotifySignature OnSpawnBulletAnimNotify;

protected:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
