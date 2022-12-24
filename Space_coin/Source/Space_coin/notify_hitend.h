#pragma once

#include "game_info.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "notify_hitend.generated.h"

UCLASS()
class SPACE_COIN_API Unotify_hitend : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;

private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
