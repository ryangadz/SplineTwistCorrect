 #pragma once
 
 #include "Engine/UserDefinedStruct.h"
 #include "StartEndScale.generated.h"


 USTRUCT (BlueprintType)
 struct FStartEndScale2
 {
    GENERATED_USTRUCT_BODY()


    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Start", Category = "SplineTwistCorrect")
    FVector2D Start;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "End", Category = "SplineTwistCorrect")
    FVector2D End;

public:
    FStartEndScale2()
    {
        Start = FVector2D(1,1);
        End   = FVector2D(1,1);
    }
 };