// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardCell.h"
#include "FigureBase.h"
#include "ChessBoard.h"
#include "PlayerChessController.h"

// Sets default values for this component's properties
UBoardCell::UBoardCell()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	OnClicked.AddDynamic(this, &UBoardCell::ClickResponse);
	OnInputTouchBegin.AddDynamic(this, &UBoardCell::TouchResponce);
}


// Called when the game starts
void UBoardCell::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UBoardCell::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UBoardCell::SetFigure(AFigureBase* figure)
{	
	mCurrentFigure = figure;
	if (mCurrentFigure)
	{
		mCurrentFigure->LiftDown();

		FVector newLocation = FVector{ GetComponentLocation().X, GetComponentLocation().Y, mCurrentFigure->GetActorLocation().Z };
		mCurrentFigure->SetActorLocation(newLocation, false, nullptr, ETeleportType::ResetPhysics);
	}
	return true;
}

AFigureBase* UBoardCell::GetFigure() const
{
	return mCurrentFigure;
}

void UBoardCell::ClickResponse(UPrimitiveComponent* component, FKey key)
{
	UBoardCell* cell = Cast<UBoardCell>(component);
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerChessController* controller = Cast<APlayerChessController>(world->GetFirstPlayerController());
		if (ensure(cell) && ensure(controller))
		{
			controller->OnCellClick(cell);
		}
	}

}

void UBoardCell::TouchResponce(const ETouchIndex::Type Key, UPrimitiveComponent* component)
{
	if (ensure(component))
	{
		ClickResponse(component);
	}
}

void UBoardCell::SetBoardPosition(TPair<uint8, uint8> position)
{
	mBoardPosition = position;
}

TPair<uint8, uint8> UBoardCell::GetBoardPosition() const
{
	return mBoardPosition;
}