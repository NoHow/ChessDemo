// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FigureBase.h"
#include "PlayerChessController.generated.h"

class UChessAI;
class UBoardCell;
class AChessBoard;
class APawnFigure;

UCLASS(Blueprintable)
class CHESSDEMO_API APlayerChessController : public APlayerController
{
	GENERATED_BODY()
	
public:
    APlayerChessController();

    void SetupInputComponent() override;

    void OnCellClick(UBoardCell* cell);
    void OnFigureClick(AFigureBase* figure);

    void SetBoard(AChessBoard* board);

    UFUNCTION(BlueprintCallable, Category="UI")
    void SetSelectScreen(UUserWidget* widget);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnFigureSelect(FigureType type);

    void EndTurn();

    void GetPawnPromotion(APawnFigure* figure);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Game")
    void FinishChessGame(ChessTeam winner);
private:
    void ProcessClick(UBoardCell* cell, AFigureBase* figure);

private:
    AFigureBase* m_ActiveFigure;
    bool mIsGameOver = false;
    
    ChessTeam mCurrentPlayer = ChessTeam::White;
    AChessBoard* mChessBoard = nullptr;
    UChessAI* mAI = nullptr;

    bool mIsMovePaused = false;
    UUserWidget* mSelectScreen = nullptr;
    APawnFigure* mReplacementFigure = nullptr;
};
