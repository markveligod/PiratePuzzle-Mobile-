// Pirate Puzzle. Contact: markveligod@gmail.com


#include "Game/AI/Pirate/PirateAICharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogPirateAICharacter, All, All);

// Sets default values
APirateAICharacter::APirateAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void APirateAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}



