#pragma once

#include "Modules/ModuleManager.h"
#include "Hologram/FGHologram.h"
#include "Hologram/FGFactoryBuildingHologram.h"
#include "Equipment/FGBuildGunBuild.h"
#include "Hologram/FGFoundationHologram.h"
#include "InfiniteZoopSubsystem.h"

struct FZoopStruct
{
		AFGFoundationHologram* FoundationHologram = nullptr;
		int32 X = 0;
		int32 Y = 0;
		int32 Z = 0;
		//FIntVector LatestZoop = FIntVector(0, 0, 0);
		bool firstPassComplete = false;
		bool secondPassComplete = false;
		bool inScrollMode = false;
};

DECLARE_LOG_CATEGORY_EXTERN(InfiniteZoop_Log, Display, All);
class FInfiniteZoopModule : public FDefaultGameModuleImpl {
public:
	virtual void StartupModule() override;

	virtual bool IsGameModule() const override { return true; }

	void ScrollHologram(AFGHologram* self, int32 delta);

	bool SetZoopAmount(AFGFactoryBuildingHologram* self, const FIntVector& Zoop);

	void OnZoopUpdated(UFGBuildGunStateBuild* self, float currentZoop, float maxZoop, const FVector& zoopLocation);

	void ApplyMaxZoopClamp(AFGFactoryBuildingHologram* self, FIntVector& Zoop);

	void SetZoopFromHitResult(AFGFoundationHologram* self, const FHitResult& hitResult);

	bool OnRep_DesiredZoop(AFGFactoryBuildingHologram* self);

	int32 GetBaseCostMultiplier(const AFGFactoryBuildingHologram* self);

	void CreateDefaultFoundationZoop(AFGFoundationHologram* self, const FHitResult& hitResult);

	bool UpdateZoop(AFGFoundationHologram* self);

	void ConstructZoop(AFGFoundationHologram* self, TArray<AActor*>& out_children);

	int32 GetStructIndex(AFGFoundationHologram* self);

	bool IsZoopMode(AFGFoundationHologram* self);

	TMap<AFGHologram*, FIntVector> HologramsToZoop;
	TMap<AFGFoundationHologram*, FIntVector> CheckedFoundations;
	TArray<FZoopStruct*> FoundationsBeingZooped;

	FVector CalcPivotAxis(const EAxis::Type DesiredAxis, const FVector& ViewVector, const FQuat& ActorQuat);
};