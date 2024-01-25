// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PH_CameraWidget.generated.h"

/**
 * 
 */
UCLASS()
class PHOTOTEST_API UPH_CameraWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	// UI
		UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* photoImage_;
	

		// Material we are going to use to create a dynamic material from it
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
			class UMaterialInterface* baseMaterial_;

		// Function to set the texture of the image
		UFUNCTION(BlueprintCallable, Category = "Image")
			void SetImageTexture(UTextureRenderTarget2D* Texture);
};
