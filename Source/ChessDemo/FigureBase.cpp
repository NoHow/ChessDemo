// Fill out your copyright notice in the Description page of Project Settings.

#include "FigureBase.h"
#include "Kismet/GameplayStatics.h"
#include "ChessDemoGameModeBase.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "PlayerChessController.h"
#include "BoardCell.h"

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

void AFigureBase::Init(ChessTeam team, UBoardCell* cell, AChessBoard* chessBoard)
{
	if (mChessBoard || !chessBoard || team == ChessTeam::Invalid)
	{
		return;
	}

	mChessBoard = chessBoard;
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
	if (!GetPossibleMoves(moves))
	{
		return false;
	}

	const TPair<uint8, uint8>& position = newCell->GetBoardPosition();

	const bool cellValid = moves.ContainsByPredicate([position](const TPair<int32, int32>& possiblePos)
		{
			return ((possiblePos.Key == position.Key) && (possiblePos.Value == position.Value));
		});

	if (!cellValid)
	{
		return false;
	}

	AFigureBase* newCellFigure = newCell->GetFigure();
	if (newCellFigure)
	{
		if (newCellFigure->GetTeam() == mCurrentTeam)
		{
			return false;
		}
		else
		{
			if (ensure(mChessBoard))
			{
				mChessBoard->KillFigure(newCellFigure);
			}
		}
	}
	if (mCurrentCell)
	{
		mCurrentCell->SetFigure(nullptr);
	}
	mCurrentCell = newCell;

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

bool AFigureBase::CheckCellForFigure(TPair<uint8, uint8> cellPosition) const
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
			return true;
		}
	}

	return false;
}

bool AFigureBase::CanMoveToCell(TPair<uint8, uint8> cellPosition) const
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
			if (checkFigure->GetTeam() != mCurrentTeam)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}