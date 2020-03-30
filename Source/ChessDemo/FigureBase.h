// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FigureBase.generated.h"

class AChessBoard;
class UBoardCell;

UENUM(BlueprintType)
enum class ChessTeam : uint8
{
	Invalid,
	White,
	Dark
};

UCLASS()
class CHESSDEMO_API AFigureBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFigureBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Init(ChessTeam team, UBoardCell* cell, AChessBoard* chessBoard);

	//All figures should override this method with their movement, writes all cell where figure might step 
	virtual bool GetPossibleMoves(TArray<TPair<int32, int32>>& moves) { return false; };

	//Move figure to desired cell
	virtual bool MoveTo(UBoardCell* newCell);

	void LiftUp();
	void LiftDown();

	const ChessTeam GetTeam() const;

	void SetCurrentCell(UBoardCell* newCell);
	UBoardCell* GetCurrentCell() const;

	float GetFigureValue() const;
protected:
	virtual void BeginPlay() override;

	bool CheckCellForFigure(TPair<uint8, uint8> cellPosition) const;
	bool CanMoveToCell(TPair<uint8, uint8> cellPosition) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	AChessBoard* mChessBoard;

	UPROPERTY()
	ChessTeam mCurrentTeam = ChessTeam::Invalid;

	UPROPERTY()
	UBoardCell* mCurrentCell = nullptr;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* mMeshComponent;

	UStaticMesh* darkInstance = nullptr;
	UStaticMesh* whiteInstance = nullptr;

	TArray<TPair<int32, int32>> mPossibleMoves;

	//Value of figure for to calculate movement for AI
	//I use the 1-3-3-5-9 system of point totals as the most commonly given
	float mFigureValue = 0;

private:
	//True - Dark team, False - White team
	void SetTeam(ChessTeam newTeam);

	void NotifyActorOnClicked(FKey buttonPressed = EKeys::LeftMouseButton) override;
	void NotifyActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex) override;

	void ProcessInteraction();
};
	