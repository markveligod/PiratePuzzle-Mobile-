// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/SkeletonCannon/Notifys/SpawnBulletAnimNotify.h"

void USpawnBulletAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    this->OnSpawnBulletAnimNotify.Broadcast();
    Super::Notify(MeshComp, Animation);
}
