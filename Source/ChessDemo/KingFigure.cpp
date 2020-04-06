// Fill out your copyright notice in the Description page of Project Settings.


#include "KingFigure.h"
#include "UObject/ConstructorHelpers.h"
#include "ChessBoard.h"
#include "BoardCell.h"
#include "Engine/Engine.h"

AKingFigure::AKingFigure()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DarkFigureAsset(TEXT("/Game/Chess/Models/King/KingDark_SM.KingDark_SM"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WhiteFigureAsset(TEXT("/Game/Chess/Models/King/KingWhite_SM.KingWhite_SM"));

	if (DarkFigureAsset.Succeeded())
	{
		darkInstance = DarkFigureAsset.Object;
	}

	if (WhiteFigureAsset.Succeeded())
	{
		whiteInstance = WhiteFigureAsset.Object;
	}

	mFigureValue = 10.f;

	mCheckDirections.Reserve(8);
	mCheckDirections.Emplace(1, 0);
	mCheckDirections.Emplace(-1, 0);
	mCheckDirections.Emplace(0, 1);
	mCheckDirections.Emplace(0, -1);
	mCheckDirections.Emplace(1, 1);
	mCheckDirections.Emplace(-1, 1);
	mCheckDirections.Emplace(1, -1);
	mCheckDirections.Emplace(-1, -1);
}

void AKingFigure::GetPossibleMoves(TArray<TPair<int32, int32>>& moves, bool checkUpdate)
{
	GetMovesBase(moves, checkUpdate, 1);
}

FigureType AKingFigure::GetFigureType() const
{
	return FigureType::King;
}

bool AKingFigure::GetCheckStatus()
{
	auto figures = mChessBoard->GetAllFigures();

	TPair<uint8, uint8> boardPosition;
	boardPosition = GetCurrentCell()->GetBoardPosition();
	
	for (auto* figure : figures)
	{
		if (!figure)
		{
			continue;
		}

		TArray<TPair<int32, int32>> moves;
		figure->GetPossibleMoves(moves);

		if (figure->GetTeam() != mCurrentTeam)
		{
			for (const auto& move : moves)
			{
				if (move.Key == boardPosition.Key && move.Value == boardPosition.Value)
				{
					return true;
				}
			}
		}
	}

	return false;
}