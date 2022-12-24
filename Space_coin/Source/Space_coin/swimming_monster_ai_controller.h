#pragma once

#include "game_info.h"
#include "monster_ai_controller.h"
#include "swimming_monster_ai_controller.generated.h"

UCLASS()
class SPACE_COIN_API Aswimming_monster_ai_controller : public Amonster_ai_controller
{
	GENERATED_BODY()

public:
	Aswimming_monster_ai_controller();

protected:
	UBehaviorTree* bt_asset;
	UBlackboardData* bb_asset;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	void SetTarget(UObject* target);

	AMyCharacter* GetTarget() const;

};
