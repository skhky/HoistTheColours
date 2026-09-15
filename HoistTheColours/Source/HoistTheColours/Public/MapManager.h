// MapManager.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "Components/SceneComponent.h"
#include "MapManager.generated.h"

UCLASS()
class HOISTTHECOLOURS_API AMapManager : public AActor
{
    GENERATED_BODY()

public:
    AMapManager();

    // Initialize map generation (server only)
    void InitializeMap();

    // Return whether a tile id exists
    bool HasTile(int32 TileId) const;

    // Get tile info (const). Returns nullptr if not found.
    const FTileInfo* GetTileInfo(int32 TileId) const;

    // Reveal a tile (mark public). Returns true if changed to public.
    bool RevealTile(int32 TileId);

    // Check if a specific tile id is revealable for the given nation
    bool IsTileRevealableForNation(int32 TileId, int32 NationId) const;

    // Check whether a tile is revealable for a given nation/source (placeholder)
    bool CanRevealTile(int32 TileId, int32 NationId) const;

    // Find a revealable tile for the given nation. Returns true and sets OutTileId if found.
    bool FindFirstRevealableTileForNation(int32 NationId, int32& OutTileId) const;


protected:
    virtual void BeginPlay() override;

    // Simple storage: TileId -> FTileInfo
    UPROPERTY()
    TMap<int32, FTileInfo> Tiles;

    // Root scene component to ensure actor has a valid root for replication relevance
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* RootScene;

    // Replicated array of tile infos to propagate bIsPublic to clients
    UPROPERTY(ReplicatedUsing=OnRep_ReplicatedTiles)
    TArray<FTileInfo> ReplicatedTiles;

    UFUNCTION()
    void OnRep_ReplicatedTiles();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // (Replication of tile data will be implemented later)
};
