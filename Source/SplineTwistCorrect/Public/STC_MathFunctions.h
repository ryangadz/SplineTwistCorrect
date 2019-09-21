// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine.h"
#include "STC_MathFunctions.generated.h"

/**
 * 
 */
UCLASS()
class SPLINETWISTCORRECT_API USTC_MathFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


    /** Input a radius for points on a icosahedron. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Calculate Icosahedron", Keywords = "SplineTwistCorrect TwistCorrect SplineCorrect"), Category = "STC Math Functions")
	static void CalcIcosahedron(
		TArray<FVector> &Vertices ,
       // TArray<FVector&> Vertices(),
        const float Radius=10);

	/** Input a radius for points on a icosahedron. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Find Next Spline Location", Keywords = "SplineTwistCorrect TwistCorrect SplineCorrect"), Category = "STC Math Functions")
	static bool FindNextSplineLocation(
		UObject* WorldContextObject,
		FVector &NextLocation,
		FVector &NextUpVector,
        const float SegmentLength=10,
		const FVector LastLocation = FVector(0.f,0.f,0.f),
		const FVector LastDirection = FVector(1.f,0.f,0.f),
		const int32 Seed=0);
    

};
