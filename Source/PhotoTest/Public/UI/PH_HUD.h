// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/PH_CameraWidget.h"
#include "Engine/TextureRenderTarget2D.h"
#include "PH_HUD.generated.h"

/**
 * 
 */
UCLASS()
class PHOTOTEST_API APH_HUD : public AHUD
{
	GENERATED_BODY()

public:

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
		TSubclassOf<UPH_CameraWidget> cameraBP_;

	UPROPERTY()
		UPH_CameraWidget* cameraWidget_;

	UFUNCTION()
		void SetImageTexture(UTextureRenderTarget2D* Texture);
	
};
