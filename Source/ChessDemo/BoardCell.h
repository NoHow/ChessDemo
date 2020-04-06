// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "BoardCell.generated.h"

class AFigureBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHESSDEMO_API UBoardCell : public UBoxComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBoardCell();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool SetFigure(AFigureBase* figure);

	AFigureBase* GetFigure() const;

	UFUNCTION()
	void ClickResponse(UPrimitiveComponent* component, FKey key = EKeys::LeftMouseButton);

	UFUNCTION()
	void TouchResponce(const ETouchIndex::Type Key, UPrimitiveComponent* component);

	void SetBoardPosition(TPair<uint8, uint8> position);
	const TPair<uint8, uint8>& GetBoardPosition() const;

private:
	UPROPERTY()
	AFigureBase* mCurrentFigure = nullptr;

	TPair<uint8, uint8> mBoardPosition;
};
