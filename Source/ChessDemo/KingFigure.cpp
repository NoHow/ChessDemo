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

void AKingFigure::GetPossibleMoves(TArray<TPair<int32, int32>>& moves)
{
	GetMovesBase(moves, 1);

	//Check to see if king will be in check state on possible move
	//TArray<TPair<int32, int32>> checkMoves;
	//checkMoves.Reserve(8);

	//for (const auto* move : moves)
	//{
	//	if(GetCheckStatus())
	//}
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
	
	for (const auto& figure : figures)
	{
		TArray<TPair<int32, int32>> moves;
		figure->GetPossibleMoves(moves);

		for (const auto& move : moves)
		{
			if (move.Key == boardPosition.Key && move.Value == boardPosition.Value)
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.5f, FColor::Yellow, FString("CHECK"));
				return true;
			}
		}
	}

	return false;
}