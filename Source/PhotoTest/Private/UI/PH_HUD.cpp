// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PH_HUD.h"
#include "Widgets/PH_CameraWidget.h"
#include "Engine/TextureRenderTarget2D.h"


void APH_HUD::BeginPlay() {


	cameraWidget_ = CreateWidget<UPH_CameraWidget>(PlayerOwner, cameraBP_);

	if (cameraWidget_) {
		cameraWidget_->AddToViewport();
	}

}

void APH_HUD::SetImageTexture(UTextureRenderTarget2D* Texture) {

	cameraWidget_->SetImageTexture(Texture);

}