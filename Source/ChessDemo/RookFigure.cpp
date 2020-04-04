// Fill out your copyright notice in the Description page of Project Settings.


#include "RookFigure.h"
#include "UObject/ConstructorHelpers.h"
#include "ChessBoard.h"
#include "BoardCell.h" 

ARookFigure::ARookFigure()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DarkFigureAsset(TEXT("/Game/Chess/Models/Rook/RookDark_SM.RookDark_SM"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WhiteFigureAsset(TEXT("/Game/Chess/Models/Rook/RookWhite_SM.RookWhite_SM"));

	if (DarkFigureAsset.Succeeded())
	{
		darkInstance = DarkFigureAsset.Object;
	}

	if (WhiteFigureAsset.Succeeded())
	{
		whiteInstance = WhiteFigureAsset.Object;
	}

	SetActorScale3D(FVector{ 0.05 });
	mFigureValue = 5.f;

	mCheckDirections.Emplace(1, 0);
	mCheckDirections.Emplace(-1, 0);
	mCheckDirections.Emplace(0, 1);
	mCheckDirections.Emplace(0, -1);
}