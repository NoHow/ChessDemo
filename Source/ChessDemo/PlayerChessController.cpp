// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerChessController.h"
#include "Engine/StaticMesh.h"
#include "BoardCell.h"
#include "Engine/Engine.h"
#include "FigureBase.h"
#include "BoardCell.h"
#include "Containers/Array.h"
#include "ChessAI.h"
#include "ChessBoard.h"
#include "PawnFigure.h"
#include "Blueprint/UserWidget.h" 

APlayerChessController::APlayerChessController()
{
    bEnableClickEvents = true;
    bEnableTouchEvents = true;

    mAI = CreateDefaultSubobject<UChessAI>(TEXT("AI"));
    ensure(mAI);
}

void APlayerChessController::SetupInputComponent()
{
    Super::SetupInputComponent();
}

void APlayerChessController::OnCellClick(UBoardCell* cell)
{
    if (!ensure(cell))
    {
        return;
    }

    ProcessClick(cell, cell->GetFigure());
}

void APlayerChessController::OnFigureClick(AFigureBase* figure)
{
    if (!ensure(figure))
    {
        return;
    }

    ProcessClick(figure->GetCurrentCell(), figure);
}

void APlayerChessController::ProcessClick(UBoardCell* cell, AFigureBase* figure)
{
    if (mCurrentPlayer != ChessTeam::White || mIsMovePaused)
    {
        return;
    }

    if (m_ActiveFigure && m_ActiveFigure->GetTeam() == ChessTeam::White)
    {
        //If it's not the same try to move there figure
        if (m_ActiveFigure->MoveTo(cell))
        {
            m_ActiveFigure = nullptr;
           
            if (!mIsMovePaused)
            {
                EndTurn();
            }
        }
        else if (figure && figure->GetTeam() == ChessTeam::White)
        {
            m_ActiveFigure->LiftDown();
            m_ActiveFigure = figure;
        }

        if (m_ActiveFigure)
        {
            m_ActiveFigure->LiftUp();
        }
    }
    //If there are no active figures - make this one active
    else if(figure && figure->GetTeam() == ChessTeam::White)
    {
        m_ActiveFigure = figure;
        m_ActiveFigure->LiftUp();
    }
}

void APlayerChessController::SetBoard(AChessBoard* board)
{
    mChessBoard = board;
}

void APlayerChessController::SetSelectScreen(UUserWidget* widget)
{
    check(widget);
    mSelectScreen = widget;
}

void APlayerChessController::EndTurn()
{
    mIsGameOver = mChessBoard->CheckMateUpdate(mCurrentPlayer);

    if (mCurrentPlayer == ChessTeam::Dark)
    {
        mCurrentPlayer = ChessTeam::White;
    }
    else if(mCurrentPlayer == ChessTeam::White)
    {
        mCurrentPlayer = ChessTeam::Dark;

        if (!mIsGameOver)
        {
            mAI->ScanBoard(mChessBoard);
        }
    }
}

void APlayerChessController::GetPawnPromotion(APawnFigure* figure)
{
    check(figure);
    mReplacementFigure = figure;

    FigureType desiredFigure = FigureType::Invalid;
    if (mCurrentPlayer == ChessTeam::Dark)
    {
        OnFigureSelect(mAI->ChoosePromotion());
    }
    else if (mCurrentPlayer == ChessTeam::White)
    {
        mSelectScreen->SetIsEnabled(true);
        mSelectScreen->SetRenderOpacity(1.f);
        mIsMovePaused = true;
    }
}

void APlayerChessController::OnFigureSelect(FigureType type)
{
    mReplacementFigure->OnReplacementSelect(type);
    mIsMovePaused = false;
    EndTurn();

    mReplacementFigure = nullptr;
}