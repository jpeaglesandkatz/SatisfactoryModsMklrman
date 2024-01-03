#pragma once

#include "CoreMinimal.h"
#include "Subsystem/ModSubsystem.h"
#include "Hologram/FGHologram.h"
#include "Equipment/FGBuildGunBuild.h"
#include "Hologram/FGBuildableHologram.h"
#include "Hologram/FGResourceExtractorHologram.h"
#include "InfiniteNudge_ConfigurationStruct.h"
#include "FGPlayerController.h"
#include "Hologram/FGPipeAttachmentHologram.h"
#include "Hologram/FGWireHologram.h"
#include "FGInputLibrary.h"
#include "Hologram/FGConveyorBeltHologram.h"
#include "Hologram/FGPipelineHologram.h"
#include "Hologram/FGWireHologram.h"
#include "Hologram/FGRailroadTrackHologram.h"
#include "Hologram/FGWallAttachmentHologram.h"

#include "InfiniteNudge_Subsystem.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(InfiniteNudge_Log, Display, All);
UCLASS()
class INFINITENUDGE_API AInfiniteNudge_Subsystem : public AModSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void NudgeHologram(AFGHologram* hologram, float xDirection, float yDirection, AFGPlayerController* controller);
	void NudgeBelt(AFGConveyorBeltHologram* hologram, float xDirection, float yDirection, AFGPlayerController* controller);
	void NudgePipe(AFGPipelineHologram* hologram, float xDirection, float yDirection, AFGPlayerController* controller);
	void NudgeWire(AFGWireHologram* hologram, float xDirection, float yDirection, AFGPlayerController* controller);
	void NudgeRailroadTrack(AFGRailroadTrackHologram* hologram, float xDirection, float yDirection, AFGPlayerController* controller);
	void NudgeWallAttachment(AFGWallAttachmentHologram* hologram, float xDirection, float yDirection, AFGPlayerController* controller);

	void GetConfigValues(UObject* worldContext, APlayerController* controller);
	float GetCurrentNudgeAmount(APlayerController* controller);

	FVector CalcPivotAxis(const EAxis::Type DesiredAxis, const FVector& ViewVector, const FQuat& ActorQuat);
	FVector CalcViewAxis(const EAxis::Type DesiredAxis, const FVector& ViewVector, const FQuat& ActorQuat, EAxis::Type& FoundAxis, bool& Inverted);
	int GetClosestLookAngle(AActor* actor, APlayerController* controller);
	FVector GetLookVector(int lookAngle);

	float TinyNudgeAmount;
	float SmallNudgeAmount;
	float LargeNudgeAmount;

	FKey TinyNudgeKey;
	FKey SmallNudgeKey;
	FKey LargeNudgeKey;
	FKey VerticalNudgeKey;
};
