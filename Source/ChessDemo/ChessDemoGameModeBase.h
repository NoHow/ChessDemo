// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChessBoard.h"
#include "GameFramework/GameModeBase.h"
#include "ChessDemoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CHESSDEMO_API AChessDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite)
    AChessBoard* m_ChessBoard = nullptr;
};
