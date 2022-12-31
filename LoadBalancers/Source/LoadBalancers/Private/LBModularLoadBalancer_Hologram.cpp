#include "LBModularLoadBalancer_Hologram.h"
#include "FGColoredInstanceMeshProxy.h"
#include "FGProductionIndicatorInstanceComponent.h"
#include "LoadBalancersModule.h"

//DEFINE_LOG_CATEGORY(LoadBalancers_Log);
#pragma optimize("", off)
void ALBModularLoadBalancer_Hologram::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALBModularLoadBalancer_Hologram, LastSnapped);
	DOREPLIFETIME(ALBModularLoadBalancer_Hologram, ActiveGroupLeader);
}

void ALBModularLoadBalancer_Hologram::BeginPlay()
{
	mOutlineSubsystem = ALBOutlineSubsystem::Get(GetWorld());
	Super::BeginPlay();
}

AActor* ALBModularLoadBalancer_Hologram::Construct(TArray<AActor*>& out_children, FNetConstructionID netConstructionID)
{
	UnHighlightAll();
	return Super::Construct(out_children, netConstructionID);
}

void ALBModularLoadBalancer_Hologram::Destroyed()
{
	UnHighlightAll();
	Super::Destroyed();
}

bool ALBModularLoadBalancer_Hologram::TrySnapToActor(const FHitResult& hitResult)
{
	const bool SnapResult = Super::TrySnapToActor(hitResult);
	if (SnapResult)
	{
		ALBBuild_ModularLoadBalancer* Default = GetDefaultBuildable<ALBBuild_ModularLoadBalancer>();
		if (ALBBuild_ModularLoadBalancer* SnappedBalancer = Cast<ALBBuild_ModularLoadBalancer>(hitResult.GetActor()))
		{
			if (ActiveGroupLeader != SnappedBalancer->GroupLeader)
			{
				UnHighlightAll();
			}

			LastSnapped = SnappedBalancer;
			ActiveGroupLeader = SnappedBalancer->GroupLeader;

			HighlightAll(SnappedBalancer->GetGroupModules());
			FRotator Rot = GetActorRotation();
			Rot.Yaw += 180 * GetScrollRotateValue();

			SetActorRotation(Rot);
		}
	}
	else
	{
		if (ActiveGroupLeader)
		{
			UnHighlightAll();
			LastSnapped = nullptr;
			ActiveGroupLeader = nullptr;
		}
	}

	return SnapResult;
}

void ALBModularLoadBalancer_Hologram::Scroll(int32 delta)
{
	Super::SetScrollMode(EHologramScrollMode::HSM_ROTATE);
	if(GetSnappedBuilding() && Cast<ALBBuild_ModularLoadBalancer>(GetSnappedBuilding()))
	{
		auto pContr = this->GetNetOwningPlayer()->GetPlayerController(GetWorld());
		if (pContr && pContr->IsInputKeyDown(EKeys::LeftControl))
		{
			Super::Scroll(delta);
		}
		else if (pContr && pContr->IsInputKeyDown(EKeys::LeftShift))
		{
			Super::SetScrollMode(EHologramScrollMode::HSM_RAISE_LOWER);
			Super::Scroll(delta);
		}
		else
		{
			SetScrollRotateValue(GetScrollRotateValue() + delta);
		}
	}
	else
	{
		Super::Scroll(delta);
	}
}

bool ALBModularLoadBalancer_Hologram::IsValidHitResult(const FHitResult& hitResult) const
{
	bool SuperBool = Super::IsValidHitResult(hitResult);

	// We clear the outline here if it invalid hit (in some cases it still hold the old outline because he switch instandly to Invalid)
	if (!SuperBool && mOutlineSubsystem)
	{
		if (mOutlineSubsystem->HasAnyOutlines())
		{
			mOutlineSubsystem->ClearOutlines();
		}
	}
	AActor* hitActor = hitResult.GetActor();
	ALBBuild_ModularLoadBalancer* cl = Cast <ALBBuild_ModularLoadBalancer>(hitActor);
	if (hitActor && cl)
	{
		return true;
	}

	return SuperBool;
}

void ALBModularLoadBalancer_Hologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
	AActor* hitActor = hitResult.GetActor();
	ALBBuild_ModularLoadBalancer* HitBalancer = Cast <ALBBuild_ModularLoadBalancer>(hitActor);
	if (hitActor && HitBalancer)
	{
		FRotator addedRotation = FRotator(0, 0, 0);
		if (this->GetRotationStep() != 0)
		{
			addedRotation.Yaw = this->GetScrollRotateValue();
		}
		if (ActiveGroupLeader != HitBalancer->GroupLeader)
		{
			UnHighlightAll();
		}

		LastSnapped = HitBalancer;
		ActiveGroupLeader = HitBalancer->GroupLeader;
		HighlightAll(HitBalancer->GetGroupModules());

		if (hitResult.ImpactNormal == FVector(0, 0, -1))
		{
			SetActorLocationAndRotation(HitBalancer->GetActorLocation() + FVector(0, 0, -200), HitBalancer->GetActorRotation() + addedRotation);
			this->mSnappedBuilding = HitBalancer;
			this->CheckValidPlacement();
		}
		else if (hitResult.ImpactNormal == FVector(0, 0, 1))
		{
			SetActorLocationAndRotation(HitBalancer->GetActorLocation() + FVector(0, 0, 200), HitBalancer->GetActorRotation() + addedRotation);
			this->mSnappedBuilding = HitBalancer;
			this->CheckValidPlacement();
		}
		else if (hitResult.ImpactNormal.Y < -0.5)
		{
			SetActorLocationAndRotation(HitBalancer->GetActorLocation() + FVector(74, -210, 0), HitBalancer->GetActorRotation() + addedRotation);
			this->mSnappedBuilding = HitBalancer;
			this->CheckValidPlacement();
		}
		else if (hitResult.ImpactNormal.Y > 0.5)
		{
			SetActorLocationAndRotation(HitBalancer->GetActorLocation() + FVector(-75, 210, 0), HitBalancer->GetActorRotation() + addedRotation);
			this->mSnappedBuilding = HitBalancer;
			this->CheckValidPlacement();
		}
		else
		{
			Super::SetHologramLocationAndRotation(hitResult);
		}
	}
	else
	{
		Super::SetHologramLocationAndRotation(hitResult);
	}
}

void ALBModularLoadBalancer_Hologram::UnHighlightAll()
{
	if (mOutlineSubsystem)
	{
		mOutlineSubsystem->ClearOutlines(true);
	}
}

void ALBModularLoadBalancer_Hologram::ConfigureActor(AFGBuildable* inBuildable) const
{
	if (ALBBuild_ModularLoadBalancer* Balancer = Cast<ALBBuild_ModularLoadBalancer>(inBuildable))
	{
		if (LastSnapped)
		{
			Balancer->GroupLeader = LastSnapped->GroupLeader;
		}
		else
		{
			Balancer->GroupLeader = Balancer;
		}
	}

	Super::ConfigureActor(inBuildable);
}

void ALBModularLoadBalancer_Hologram::HighlightAll(TArray<ALBBuild_ModularLoadBalancer*> actorsToOutline)
{
	if (actorsToOutline.Num() <= 0)
	{
		return;
	}

	if (ActiveGroupLeader)
	{
		mOutlineSubsystem->SetOutlineColor(mHoloColor, true);
		for (ALBBuild_ModularLoadBalancer* OutlineActor : actorsToOutline)
		{
			if (mOutlineSubsystem)
			{
				mOutlineSubsystem->CreateOutline(OutlineActor, true);
			}
		}
	}
}
#pragma optimize("", on)