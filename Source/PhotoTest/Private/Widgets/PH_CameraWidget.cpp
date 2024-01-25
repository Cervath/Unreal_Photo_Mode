// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PH_CameraWidget.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"

void UPH_CameraWidget::SetImageTexture(UTextureRenderTarget2D* Texture)
{
    if (photoImage_ && Texture)
    {

        // Create a dynamic material instance and set its texture parameter
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(baseMaterial_, this);


        DynamicMaterial->SetTextureParameterValue("PhotoTexture", Texture);

        photoImage_->SetBrushFromMaterial(DynamicMaterial);
    }

}