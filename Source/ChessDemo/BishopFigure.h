// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FigureBase.h"
#include "BishopFigure.generated.h"

/**
 * 
 */
UCLASS()
class CHESSDEMO_API ABishopFigure : public AFigureBase
{
	GENERATED_BODY()

public:
	ABishopFigure();

	bool GetPossibleMoves(TArray<TPair<int32, int32>>& moves) override;
};
