// Fill out your copyright notice in the Description page of Project Settings.

#include "FigureBase.h"
#include "Kismet/GameplayStatics.h"
#include "ChessDemoGameModeBase.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Containers/Array.h"
#include "Kismet/KismetMathLibrary.h" 
#include "PlayerChessController.h"
#include "BoardCell.h"

UBoardCell* AFigureBase::mPreviousCell = nullptr;
AFigureBase* AFigureBase::mPreviousFigure = nullptr;
AFigureBase* AFigureBase::mKilledFigure = nullptr;
AChessBoard* AFigureBase::mChessBoard = nullptr;

// Sets default values
AFigureBase::AFigureBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));

	check(mMeshComponent);
	SetRootComponent(Cast<USceneComponent>(mMeshComponent));
}

// Called when the game starts or when spawned
void AFigureBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFigureBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFigureBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

const ChessTeam AFigureBase::GetTeam() const
{
	return mCurrentTeam;
}

void AFigureBase::SetCurrentCell(UBoardCell* newCell)
{
	if (ensure(newCell))
	{
		mCurrentCell = newCell;
	}
}

UBoardCell* AFigureBase::GetCurrentCell() const
{
	return mCurrentCell;
}

void AFigureBase::Init(ChessTeam team, UBoardCell* cell)
{
	check(cell);
	SetTeam(team);
	SetCurrentCell(cell);
}
 
void AFigureBase::NotifyActorOnClicked(FKey buttonPressed)
{
	ProcessInteraction();
}

void AFigureBase::NotifyActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex)
{
	ProcessInteraction();
}

void AFigureBase::ProcessInteraction()
{
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerChessController* controller = Cast<APlayerChessController>(world->GetFirstPlayerController());
		if (ensure(controller))
		{
			controller->OnFigureClick(this);
		}
	}
}

float AFigureBase::GetFigureValue() const
{
	if (mCurrentTeam == ChessTeam::White)
	{
		return mFigureValue;
	}
	else if(mCurrentTeam == ChessTeam::Dark)
	{
		return -mFigureValue;
	}

	return 0;
}

bool AFigureBase::MoveTo(UBoardCell* newCell)
{
	if (!newCell)
	{
		return false;
	}

	TArray<TPair<int32, int32>> moves;
	GetPossibleMoves(moves, true);

	const TPair<uint8, uint8>& position = newCell->GetBoardPosition();

	const bool cellValid = moves.ContainsByPredicate([position](const TPair<int32, int32>& possiblePos)
		{
			return ((possiblePos.Key == position.Key) && (possiblePos.Value == position.Value));
		});

	if (!cellValid)
	{
		return false;
	}

	MakeMove(newCell);

	return true;
}

bool AFigureBase::MakeMove(UBoardCell* destinationCell)
{
	if (mKilledFigure)
	{
		mKilledFigure->Destroy();
		mKilledFigure = nullptr;
	}

	AFigureBase* newCellFigure = destinationCell->GetFigure();
	if (newCellFigure)
	{
		if (newCellFigure->GetTeam() == mCurrentTeam)
		{
			return false;
		}
		else
		{
			mChessBoard->KillFigure(newCellFigure);
			mKilledFigure = newCellFigure;
		}
	}

	mCurrentCell->SetFigure(nullptr);
	destinationCell->SetFigure(this);

	mPreviousCell = mCurrentCell;
	mPreviousFigure = this;
	mCurrentCell = destinationCell;

	return true;
}

void AFigureBase::LiftUp()
{
	if (ensure(mMeshComponent) && ensure(mChessBoard))
	{
		const float liftStrength = 40.f;

		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, mChessBoard->GetActorLocation().Z + liftStrength));
		mMeshComponent->SetSimulatePhysics(false);
	}
}

void AFigureBase::LiftDown()
{
	if (ensure(mMeshComponent))
	{
		mMeshComponent->SetSimulatePhysics(true);
	}
}

void AFigureBase::SetTeam(ChessTeam newTeam)
{
	if (!ensure(mMeshComponent))
	{
		return;
	}

	if (newTeam == ChessTeam::Dark && ensure(darkInstance))
	{
		mMeshComponent->SetStaticMesh(darkInstance);
	}
	else if (newTeam == ChessTeam::White && ensure(whiteInstance))
	{
		mMeshComponent->SetStaticMesh(whiteInstance);
	}

	mCurrentTeam = newTeam;
}

void AFigureBase::GetMovesBase(TArray<TPair<int32, int32>>& moves, bool checkUpdate, uint8 checkLimit)
{
	check(mCurrentCell && mChessBoard);

	auto cellPosition = mCurrentCell->GetBoardPosition();
	const uint8 currentRow = cellPosition.Key;
	const uint8 currentColumn = cellPosition.Value;

	auto checkCellPosition = TPair<uint8, uint8>(currentRow, currentColumn);
	UBoardCell* checkCell = nullptr;

	for (uint8 index = 0; index < mCheckDirections.Num(); ++index)
	{
		checkCellPosition.Key = currentRow + mCheckDirections[index].Key;
		checkCellPosition.Value = currentColumn + mCheckDirections[index].Value;

		checkCell = mChessBoard->GetCell(checkCellPosition);

		uint8 checkCounter = 0;
		while (checkCell && checkCounter++ < checkLimit)
		{
			bool isThereFigure = false;
			if (!CanMoveToCell(checkCellPosition, isThereFigure))
			{
				break;
			}

			if (checkUpdate && MakeMove(checkCell))
			{
				if (!mChessBoard->GetCheckStatus(mCurrentTeam))
				{
					moves.Add(TPair<int32, int32>(checkCellPosition.Key, checkCellPosition.Value));
				}

				AFigureBase::CancelMove();
			}
			else
			{
				moves.Add(TPair<int32, int32>(checkCellPosition.Key, checkCellPosition.Value));
			}

			if (isThereFigure)
			{
				break;
			}

			checkCellPosition.Key += mCheckDirections[index].Key;
			checkCellPosition.Value += mCheckDirections[index].Value;

			checkCell = mChessBoard->GetCell(checkCellPosition);
		}
	}
}

bool AFigureBase::CanMoveToCell(TPair<uint8, uint8> cellPosition, bool& isThereFigure)
{
	if (!mChessBoard)
	{
		return false;
	}

	UBoardCell* checkCell = mChessBoard->GetCell(cellPosition);
	if (checkCell)
	{
		AFigureBase* checkFigure = checkCell->GetFigure();
		if (checkFigure)
		{
			isThereFigure = true;
			if (checkFigure->GetTeam() == mCurrentTeam)
			{
				return false;
			}

			return true;
		}

		return true;
	}

	return false;
}

void AFigureBase::GetPossibleMoves(TArray<TPair<int32, int32>>& moves, bool checkUpdate)
{
	GetMovesBase(moves, checkUpdate);
}

UBoardCell* AFigureBase::FindBestMove()
{
	check(mChessBoard);
	TArray<TPair<int32, int32>> moves;
	GetPossibleMoves(moves, true);

	if (moves.Num() == 0)
	{
		return nullptr;
	}

	float maxValue = FLT_MIN;
	UBoardCell* bestMove = nullptr;

	for (const auto& move : moves)
	{
		UBoardCell* cell = mChessBoard->GetCell(TPair<uint8, uint8>(move.Key, move.Value));
		if (!cell)
		{
			continue;
		}

		AFigureBase* valueFrom = cell->GetFigure();
		if (valueFrom)
		{
			const float figureValue = valueFrom->GetFigureValue();
			if (figureValue > maxValue)
			{
				bestMove = cell;
				maxValue = figureValue;
			}
		}
	}

	if (!bestMove && moves.Num() > 0)
	{
		auto& randomMove = moves[UKismetMathLibrary::RandomInteger(moves.Num())];
		bestMove = mChessBoard->GetCell(TPair<uint8, uint8>(randomMove.Key, randomMove.Value));
	}

	return bestMove;
}

FigureType AFigureBase::GetFigureType() const
{
	return FigureType::Invalid;
}

void AFigureBase::CancelMove()
{
	if (mPreviousCell && mPreviousFigure)
	{
		UBoardCell* previousFigureCell = mPreviousFigure->GetCurrentCell();
		if (mKilledFigure)
		{
			mChessBoard->RestoreFigure(mKilledFigure);
			previousFigureCell->SetFigure(mKilledFigure);
			mKilledFigure->SetCurrentCell(previousFigureCell);
		}
		else
		{
			previousFigureCell->SetFigure(nullptr);
		}
		
		mPreviousFigure->SetCurrentCell(mPreviousCell);
		mPreviousCell->SetFigure(mPreviousFigure);

		mPreviousFigure = nullptr;
		mPreviousCell = nullptr;
		mKilledFigure = nullptr;
	}
}

void AFigureBase::SetBoard(AChessBoard* board)
{
	mChessBoard = board;
}