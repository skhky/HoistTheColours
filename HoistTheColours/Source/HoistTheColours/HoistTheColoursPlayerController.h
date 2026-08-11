#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HoistTheColoursPlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UPathFollowingComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class AHoistTheColoursPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = AI)
	TObjectPtr<UPathFollowingComponent> PathFollowingComponent;

	UPROPERTY(EditAnywhere, Category="Input")
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UNiagaraSystem> FXCursor;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SetDestinationClickAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SetDestinationTouchAction;

	uint32 bMoveToMouseCursor : 1;

	uint32 bIsTouch : 1;

	FVector CachedDestination;

	float FollowTime = 0.0f;

public:
	AHoistTheColoursPlayerController();

protected:
	virtual void SetupInputComponent() override;
	
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	void UpdateCachedDestination();

	void MoveForward(float Value);
	void MoveRight(float Value);
};


