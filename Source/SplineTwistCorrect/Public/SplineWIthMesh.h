// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "SplineTwistCorrectBPLibrary.h"
#include "SplineWithMesh.generated.h"


UCLASS( ClassGroup=(SplineTwistCorrect), meta=(BlueprintSpawnableComponent) )
class SPLINETWISTCORRECT_API USplineWithMesh : public USplineComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USplineWithMesh(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Static Mesh", Category = "SplineMesh Properties")
	class UStaticMesh *StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Sub-Segment Length", Category = "SplineMesh Properties")
	float SubSegmentLength = 100.f;

	UPROPERTY()
	float Length = 100.f;

	UPROPERTY()
	int32 Number = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<class USplineMeshComponent*> SplineMeshArray;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USplineMeshComponent* SplineMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent *Root;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class AActor *Actor;

	void AddMesh();

	void RemoveMesh();

virtual void PostLoadSubobjects( FObjectInstancingGraph* OuterInstanceGraph ) override;

virtual void PostInitProperties() override;
//virtual void PreInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PreEditChange(UProperty* PropertyThatWillChange) override;
#endif 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
//	void UpdateSpline() override;
//	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
