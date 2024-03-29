// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Curves/CurveVector.h"
#include "StartEndScale.h"
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

UENUM(BlueprintType)
enum class EMeshRollType : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Incremental UMETA(DisplayName = "Incremental"),
	E_Random UMETA(DisplayName = "Random")
};

UENUM(BlueprintType)
enum class EMeshScalingType : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Numerical UMETA(DisplayName = "Numerical"),
	E_SplineScale UMETA(DisplayName = "Spline Scale"),
	E_UniformCurve UMETA(DisplayName = "Uniform from Curve"),
	E_NonUniformCurve UMETA(DisplayName = "X and Y from Curve"),
	E_CurveScaled UMETA(DisplayName = "X from Curve Scaled")
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

	UPROPERTY(interp, BlueprintReadWrite,  DisplayName = "Spline Mesh", Category = "SplineMesh Properties", AdvancedDisplay)
	class USplineMeshComponent* SplineMesh;

	UPROPERTY()
	class USceneComponent *Root;

	UPROPERTY()
	class AActor *Actor;


	//Adds spline mesh entire lenght of spline (old mesh are removed automatically)
	UFUNCTION(BlueprintCallable, DisplayName = "Add Mesh", Category = "SplineMesh Properties")
	void AddMesh(class AActor * PActor);

	//Removes all spline mesh components
	UFUNCTION(BlueprintCallable, DisplayName = "Remove Mesh", Category = "SplineMesh Properties")
	void RemoveMesh();

 	UFUNCTION()
	void AddRootToParent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Add Direction Arrows", Category = "Spline Properties")
	bool bAddDirectionArrows = true; 

	UPROPERTY()
	bool bArrows;

	ESplineCoordinateSpace::Type CoordSpace = ESplineCoordinateSpace::Local;

	UPROPERTY( EditAnywhere, BlueprintReadWrite,DisplayName = "Mesh Scaling Type", Category = "SplineMesh Properties")
	EMeshScalingType MeshScalingType;
#if WITH_EDITOR
    virtual bool CanEditChange(const UProperty* InProperty) const override;
#endif
	UPROPERTY(EditAnywhere, BlueprintReadWrite,DisplayName = "Numerical Scaling", Category = "SplineMesh Properties")
	float Scale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,DisplayName = "Scale Curve", Category = "SplineMesh Properties")
	class UCurveVector *ScaleCurve;

	UFUNCTION()
	FStartEndScale2 GetScaleMesh(int i);

	UPROPERTY( EditAnywhere, BlueprintReadWrite,DisplayName = "Mesh Rotation Type", Category = "SplineMesh Properties")
	EMeshRollType MeshRollType;

	UPROPERTY( EditAnywhere, BlueprintReadWrite,DisplayName = "Mesh Roll Increment", Category = "SplineMesh Properties")
	float RollIncrement = 60.f;

	UFUNCTION()
	float GetRollMesh(int i);

	UFUNCTION(BlueprintCallable, DisplayName = "Add Direction Arrows", Category = "Spline Properties")
	void AddDirectionArrows();

	UFUNCTION(BlueprintCallable, DisplayName = "Remove Direction Arrows", Category = "Spline Properties")
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
