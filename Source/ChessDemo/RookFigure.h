// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FigureBase.h"
#include "RookFigure.generated.h"

/**
 * 
 */
UCLASS()
class CHESSDEMO_API ARookFigure : public AFigureBase
{
	GENERATED_BODY()
	
public:
	ARookFigure();

	FigureType GetFigureType() const override;
};
