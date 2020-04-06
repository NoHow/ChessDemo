// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopFigure.h"
#include "UObject/ConstructorHelpers.h"
#include "ChessBoard.h"
#include "BoardCell.h" 

ABishopFigure::ABishopFigure()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DarkFigureAsset(TEXT("/Game/Chess/Models/Bishop/BishopDark_SM.BishopDark_SM"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WhiteFigureAsset(TEXT("/Game/Chess/Models/Bishop/BishopWhite_SM.BishopWhite_SM"));

	if (DarkFigureAsset.Succeeded())
	{
		darkInstance = DarkFigureAsset.Object;
	}

	if (WhiteFigureAsset.Succeeded())
	{
		whiteInstance = WhiteFigureAsset.Object;
	}

	mFigureValue = 3.f;

	mCheckDirections.Reserve(4);
	mCheckDirections.Emplace(1, 1);
	mCheckDirections.Emplace(-1, 1);
	mCheckDirections.Emplace(1, -1);
	mCheckDirections.Emplace(-1, -1);
}

FigureType ABishopFigure::GetFigureType() const
{
	return FigureType::Bishop;
}