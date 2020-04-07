// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FigureBase.h"
#include "ChessAI.generated.h"

class AChessBoard;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHESSDEMO_API UChessAI : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChessAI();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ScanBoard(AChessBoard* board);
	FigureType ChoosePromotion();

private:
	UFUNCTION()
	void OnSelectMove(AFigureBase* figure, UBoardCell* cellToMove, AChessBoard* board);

private:
	ChessTeam mAITeam = ChessTeam::Dark;
};
