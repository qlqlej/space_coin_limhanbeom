#pragma once

#include "game_info.h"
#include "GameFramework/Actor.h"
#include "trigger_sand.generated.h"

UCLASS()
class SPACE_COIN_API Atrigger_sand : public AActor
{
	GENERATED_BODY()
	
public:	
	Atrigger_sand();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* volume_sphere;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};