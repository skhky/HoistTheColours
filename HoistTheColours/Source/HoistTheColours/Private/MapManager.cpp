#include "MapManager.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

AMapManager::AMapManager()
{
    PrimaryActorTick.bCanEverTick = false;
    // Prefer setting bReplicates directly in constructor to avoid "SetReplicates called on non-initialized actor" warnings
    bReplicates = true;

    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;
}

bool AMapManager::FindFirstRevealableTileForNation(int32 NationId, int32& OutTileId) const
{
    for (const auto& Pair : Tiles)
    {
        const FTileInfo& Info = Pair.Value;
        if (Info.TileType == ETileType::TT_Event && !Info.bIsPublic)
        {
            OutTileId = Info.TileId;
            return true;
        }
    }
    return false;
}

// Note: FindFirstRevealableTileForNation was removed from header; keep functionality internal if needed later.

void AMapManager::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        // For now, initialize a tiny test map. Full generator will be implemented later.
        InitializeMap();
    }
}

void AMapManager::InitializeMap()
{
    Tiles.Empty();

    // Minimal placeholder map: 5 tiles in a line
    for (int32 i = 0; i < 5; ++i)
    {
        FTileInfo Info;
        Info.TileId = i;
        Info.TileType = (i == 2) ? ETileType::TT_Event : ETileType::TT_Normal;
        Info.bIsPublic = false;
        if (i > 0) Info.AdjacentIds.Add(i - 1);
        if (i < 4) Info.AdjacentIds.Add(i + 1);
        Tiles.Add(Info.TileId, Info);
    }

    UE_LOG(LogTemp, Log, TEXT("MapManager: Initialized %d tiles"), Tiles.Num());

    // Initialize replicated array to match initial tiles
    ReplicatedTiles.Empty();
    for (const auto& Pair : Tiles)
    {
        ReplicatedTiles.Add(Pair.Value);
    }
}

bool AMapManager::HasTile(int32 TileId) const
{
    return Tiles.Contains(TileId);
}

const FTileInfo* AMapManager::GetTileInfo(int32 TileId) const
{
    const FTileInfo* Found = Tiles.Find(TileId);
    return Found;
}

bool AMapManager::RevealTile(int32 TileId)
{
    FTileInfo* Found = Tiles.Find(TileId);
    if (!Found) return false;
    if (Found->bIsPublic) return false;
    Found->bIsPublic = true;
    UE_LOG(LogTemp, Log, TEXT("MapManager: Tile %d revealed"), TileId);
    // Update replicated array entry and mark dirty for replication
    for (FTileInfo& RT : ReplicatedTiles)
    {
        if (RT.TileId == TileId)
        {
            RT.bIsPublic = true;
            break;
        }
    }
    // Force replication update
    ForceNetUpdate();
    return true;
}

void AMapManager::OnRep_ReplicatedTiles()
{
    int32 PublicCount = 0;
    for (const FTileInfo& Info : ReplicatedTiles)
    {
        if (Info.bIsPublic) ++PublicCount;
    }
    UE_LOG(LogTemp, Log, TEXT("MapManager (client): OnRep_ReplicatedTiles public=%d total=%d"), PublicCount, ReplicatedTiles.Num());
}

void AMapManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMapManager, ReplicatedTiles);
}

bool AMapManager::CanRevealTile(int32 TileId, int32 NationId) const
{
    // Placeholder: only existence and not-public are checked here.
    // Real adjacency/ownership checks will be implemented when map rules are finalized.
    const FTileInfo* Found = Tiles.Find(TileId);
    if (!Found) return false;
    if (Found->bIsPublic) return false;
    return true;
}
