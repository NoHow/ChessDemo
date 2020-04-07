// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessAI.h"
#include "ChessBoard.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerChessController.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UChessAI::UChessAI()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UChessAI::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UChessAI::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//Scan board for ai figures and select best move for this turn which is selected by simply selecting
//move with higher figure value (not taking into account figure position)
void UChessAI::ScanBoard(AChessBoard* board)
{
	check(board);

	UBoardCell* bestMove = nullptr;
	AFigureBase* bestFigure = nullptr;

	TMap<AFigureBase*, UBoardCell*> bestMoves;
	const uint8 boardLimit = 9;
	for(auto* figure : board->GetAllFigures())
	{
		if (!figure)
		{
			continue;
		}

		UBoardCell* boardCell = figure->GetCurrentCell();
		check(boardCell);

		if (figure->GetTeam() == mAITeam)
		{
			UBoardCell* currentBestMove = figure->FindBestMove();
			if (currentBestMove)
			{
				bestMoves.Add(figure, currentBestMove);
			}
		}
	}

	float maxValue = FLT_MIN;
	for (const auto& it : bestMoves)
	{
		AFigureBase* currentFigure = it.Key;
		UBoardCell* currentMove = it.Value;

		AFigureBase* valueFrom = currentMove->GetFigure();
		if (!valueFrom)
		{
			continue;
		}

		const float figureValue = valueFrom->GetFigureValue();
		if (figureValue > maxValue)
		{
			bestMove = currentMove;
			bestFigure = currentFigure;
			maxValue = figureValue;
		}
	}

	if (!bestMove && bestMoves.Num() > 0)
	{
		TArray<AFigureBase*> mKeyArray;
		bestMoves.GenerateKeyArray(mKeyArray);

		size_t randomIndex = UKismetMathLibrary::RandomInteger(bestMoves.Num());

		bestFigure = mKeyArray[randomIndex];
		bestMove = *bestMoves.Find(bestFigure);
	}

	if (bestFigure)
	{
		bestFigure->LiftUp();
	}

	FTimerDelegate mTimerDel;
	FTimerHandle mTimerHandle;

	auto& timerManager = GetWorld()->GetTimerManager();
	mTimerDel.BindUFunction(this, FName("OnSelectMove"), bestFigure, bestMove, board);

	const float mAIDelay = 1.5f;
	timerManager.SetTimer(mTimerHandle, mTimerDel, mAIDelay, false);
}

void UChessAI::OnSelectMove(AFigureBase* figure, UBoardCell* cellToMove, AChessBoard* board)
{
	if (figure && cellToMove)
	{
		figure->MoveTo(cellToMove);
	}

	auto controller = Cast<APlayerChessController>(GetWorld()->GetFirstPlayerController());
	check(controller);

	controller->EndTurn();
}

FigureType UChessAI::ChoosePromotion()
{
	TStaticArray<FigureType, 4> possiblePromotions;
	possiblePromotions[0] = FigureType::Bishop;
	possiblePromotions[1] = FigureType::Knight;
	possiblePromotions[2] = FigureType::Queen;
	possiblePromotions[3] = FigureType::Rook;

	return possiblePromotions[UKismetMathLibrary::RandomIntegerInRange(0, possiblePromotions.Num() - 1u)];
}