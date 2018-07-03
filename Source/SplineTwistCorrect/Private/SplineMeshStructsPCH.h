 #pragma once
 
 #include "SplineMeshStructsPCH.generated.h"


 USTRUCT (BlueprintType)
 struct FStartEndScale
 {
    GENERATED_USTRUCT_BODY()


    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Start", Category = "SplineTwistCorrect")
    FVector2D Start;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "End", Category = "SplineTwistCorrect")
    FVector2D End;

public:
    FStartEndScale()
    {
        Start = FVector2D(1,1);
        End   = FVector2D(1,1);
    }
 };