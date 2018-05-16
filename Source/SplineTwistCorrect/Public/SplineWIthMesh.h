// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
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

	UPROPERTY( EditAnywhere, Instanced, NoClear,DisplayName = "Offset Spline", Category = "SplineMesh Properties")
	class USplineComponent* OffsetSpline;

	//	UPROPERTY( EditAnywhere, Instanced, NoClear,DisplayName = "Corrected Spline", Category = "SplineMesh Properties")

	class USplineComponent* CorrectedSpline;

	UPROPERTY( EditAnywhere, Instanced, NoClear,DisplayName = "Spline Mesh Array", Category = "SplineMesh Properties")
	TArray<class USplineMeshComponent*> SplineMeshArray;

	UPROPERTY( EditAnywhere, Instanced, NoClear,DisplayName = "Arrow Array", Category = "Spline Properties")
	TArray<class UArrowComponent*> DirectionArrows;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USplineMeshComponent* SplineMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent *Root;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class AActor *Actor;

 	UFUNCTION(BlueprintCallable)
	void AddMesh(class AActor * PActor);


	void RemoveMesh();
	
 UFUNCTION(BlueprintCallable)
	void AddRootToParent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Add Direction Arrows", Category = "Spline Properties")
	bool bAddDirectionArrows = true; 

UPROPERTY()
	bool bArrows;

	ESplineCoordinateSpace::Type CoordSpace = ESplineCoordinateSpace::Local;

	UFUNCTION(BlueprintCallable)
	void AddDirectionArrows();

	UFUNCTION()
	void RemoveDirectionArrows();

#if WITH_EDITORONLY_DATA
	virtual void Serialize(FArchive& Ar) override;
#endif
	virtual void PostLoad() override;


	virtual void OnRegister() override;


//http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UPrimitiveComponent/GetComponentInstanceData/index.html
//Called before we throw away components during RerunConstructionScripts, to cache any data we wish to persist across that operation
//virtual class FActorComponentInstanceData* GetComponentInstanceData() const override;

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
