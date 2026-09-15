// Tile.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tile.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
    TT_Normal UMETA(DisplayName = "Normal"),
    TT_Event UMETA(DisplayName = "Event"),
    TT_Dungeon UMETA(DisplayName = "Dungeon")
};

/**
 * Lightweight tile information structure.
 * Event-specific data is intentionally omitted at this stage.
 */
USTRUCT(BlueprintType)
struct FTileInfo
{
    GENERATED_BODY()

    // Unique id within the map
    UPROPERTY()
    int32 TileId = -1;

    // Tile category
    UPROPERTY()
    ETileType TileType = ETileType::TT_Normal;

    // Whether this tile has been revealed (public) to all players
    UPROPERTY()
    bool bIsPublic = false;

    // Adjacency list of TileIds
    UPROPERTY()
    TArray<int32> AdjacentIds;
};
