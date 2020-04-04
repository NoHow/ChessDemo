// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessAI.h"
#include "ChessBoard.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Kismet/KismetMathLibrary.h" 

// Sets default values for this component's properties
UChessAI::UChessAI()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	TMap<AFigureBase*, UBoardCell*> bestMoves;
	const uint8 boardLimit = 9;
	for (uint8 row = 1; row < boardLimit; row++)
	{
		for (uint8 column = 1; column < boardLimit; column++)
		{
			UBoardCell* boardCell = board->GetCell(TPair<uint8, uint8>(row, column));
			check(boardCell);

			AFigureBase* figure = boardCell->GetFigure();
			if (!figure)
			{
				continue;
			}

			if (figure->GetTeam() == mAITeam)
			{
				UBoardCell* currentBestMove = figure->FindBestMove();
				if (currentBestMove)
				{
					bestMoves.Add(figure, currentBestMove);
				}
			}
		}
	}

	UBoardCell* bestMove = nullptr;
	AFigureBase* bestFigure = nullptr;
	float maxValue = FLT_MIN;
	for (const auto& it : bestMoves)
	{
		AFigureBase* currentFigure = it.Value;
		UBoardCell* currentMove = it.Key;

		AFigureBase* valueFrom = currentFigure->GetFigure();
		if (!valueFrom)
		{
			continue;
		}

		const float figureValue = currentFigure->GetFigureValue() + valueFrom->GetFigureValue();
		if (figureValue > maxValue)
		{
			bestMove = currentMove;
			bestFigure = currentFigure;
			maxValue = figureValue;
		}
	}

	if (bestMove)
	{
		bestFigure->MoveTo(bestMove);
		bestMove->SetFigure(bestFigure);
	}
	else
	{
		TArray<AFigureBase*> mKeyArray;
		bestMoves.GenerateKeyArray(mKeyArray);
		
		size_t randomIndex = UKismetMathLibrary::RandomInteger(bestMoves.Num());
		
		AFigureBase* randFigure = mKeyArray[randomIndex];
		auto randMove = bestMoves.Find(randFigure);

		randFigure->MoveTo(*randMove);
		(*randMove)->SetFigure(randFigure);
	}
}