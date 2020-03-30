// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerChessController.h"
#include "Engine/StaticMesh.h"
#include "BoardCell.h"
#include "Engine/Engine.h"
#include "FigureBase.h"
#include "BoardCell.h"
#include "Containers/Array.h"
#include "ChessAI.h"

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

void APlayerChessController::PlayerTick(float DeltaTime)
{
    if (mCurrentPlayer == ChessTeam::Dark)
    {
        mAI->ScanBoard(mChessBoard);
        mCurrentPlayer = ChessTeam::White;
    }
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
    if (mCurrentPlayer != ChessTeam::White)
    {
        return;
    }

    if (m_ActiveFigure)
    {
        //If it's not the same try to move there figure
        if (m_ActiveFigure->MoveTo(cell))
        {
            cell->SetFigure(m_ActiveFigure);
            m_ActiveFigure = nullptr;
            
            mCurrentPlayer = ChessTeam::Dark;
        }
        else if(figure)
        {
            m_ActiveFigure->LiftDown();
            m_ActiveFigure = figure;
            m_ActiveFigure->LiftUp();
        }
    }
    //If there are no active figures - make this one active
    else if(figure)
    {
        if (figure->GetTeam() == mCurrentPlayer)
        {
            m_ActiveFigure = figure;
            m_ActiveFigure->LiftUp();
        }
    }
}

void APlayerChessController::SetBoard(AChessBoard* board)
{
    mChessBoard = board;
}