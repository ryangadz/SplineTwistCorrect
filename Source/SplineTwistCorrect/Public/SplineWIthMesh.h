// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "SplineTwistCorrectBPLibrary.h"
#include "SplineWithMesh.generated.h"

UENUM(BlueprintType)
enum class EMeshToUse : uint8
{
	E_Default UMETA(DisplayName = "Default"),
	E_Array UMETA(DisplayName = "Index From Array"),
	E_Random UMETA(DisplayName = "Random From Array")
};

UCLASS( ClassGroup=(SplineTwistCorrect), meta=(BlueprintSpawnableComponent) )
class SPLINETWISTCORRECT_API USplineWithMesh : public USplineComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USplineWithMesh(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Mesh To Use", Category = "SplineMesh Properties")
	EMeshToUse MeshToUse;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Sub-Segment Length", Category = "SplineMesh Properties")
	float SubSegmentLength = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Static Mesh Default", Category = "SplineMesh Properties")
	class UStaticMesh *StaticMeshDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Static Mesh Array", Category = "SplineMesh Properties")
	TArray <class UStaticMesh*> StaticMeshArray;

	UPROPERTY()
	class UStaticMesh *StaticMesh;

	UPROPERTY()
	float Length = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, DisplayName = "Sub-Segment Number", Category = "Spline Properties")
	int32 Number;

	//UPROPERTY( EditAnywhere,  NoClear,DisplayName = "Offset Spline", Category = "SplineMesh Properties")
	UPROPERTY()
	class USplineComponent* OffsetSpline;

 	//	UPROPERTY( EditAnywhere, Instanced, NoClear,DisplayName = "Corrected Spline", Category = "SplineMesh Properties")
	UPROPERTY()
	class USplineComponent* CorrectedSpline;

	//UPROPERTY( EditAnywhere, NoClear,DisplayName = "Spline Mesh Array", Category = "SplineMesh Properties")
	TArray<class USplineMeshComponent*> SplineMeshArray;

	UPROPERTY( EditAnywhere, BlueprintReadWrite,DisplayName = "Material Default", Category = "SplineMesh Properties")
	class UMaterialInterface* MaterialDefault;

	UPROPERTY( EditAnywhere, BlueprintReadWrite,DisplayName = "Material Array", Category = "SplineMesh Properties")
	TArray<class UMaterialInterface*> MaterialArray;



	//UPROPERTY( EditAnywhere, BlueprintReadWrite,DisplayName = "Material", Category = "SplineMesh Properties")
	class UMaterialInterface* Material;


	UPROPERTY()
	TArray<class UArrowComponent*> DirectionArrows;

	UPROPERTY()
	class UArrowComponent* Arrow;

	UPROPERTY()
	class USplineMeshComponent* SplineMesh;

	UPROPERTY()
	class USceneComponent *Root;

	UPROPERTY()
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
