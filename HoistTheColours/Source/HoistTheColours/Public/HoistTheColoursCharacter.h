// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HoistTheColoursCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UUserWidget;
class UTextBlock;

/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class AHoistTheColoursCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY()
	TObjectPtr<UUserWidget> PlayerHUD;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	TSubclassOf<UUserWidget> PlayerHUDClass;

public:

	/** Constructor */
	AHoistTheColoursCharacter();

	/** Initialization */
	virtual void BeginPlay() override;

	/** Update */
	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns the camera component **/
	UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent.Get(); }

	/** Returns the Camera Boom component **/
	USpringArmComponent* GetCameraBoom() const { return CameraBoom.Get(); }

	/** éwíËñºÇÃ TextBlock Çï‘Ç∑Åië∂ç›ÇµÇ»ÇØÇÍÇŒ nullptrÅj */
	UTextBlock* GetPlayerHUDTextBlock(const FName& TextBlockName) const;

};

