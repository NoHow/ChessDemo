// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FigureBase.h"
#include "PlayerChessController.generated.h"

class UChessAI;
class UBoardCell;
class AChessBoard;

UCLASS()
class CHESSDEMO_API APlayerChessController : public APlayerController
{
	GENERATED_BODY()
	
public:
    APlayerChessController();

    void PlayerTick(float DeltaTime) override;
    void SetupInputComponent() override;

    void OnCellClick(UBoardCell* cell);
    void OnFigureClick(AFigureBase* figure);

    void SetBoard(AChessBoard* board);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Game")
    void FinishGame(ChessTeam winner);
private:
    void ProcessClick(UBoardCell* cell, AFigureBase* figure);

private:
    AFigureBase* m_ActiveFigure;
    
    ChessTeam mCurrentPlayer = ChessTeam::White;
    AChessBoard* mChessBoard = nullptr;
    UChessAI* mAI = nullptr;
};
