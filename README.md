
# Camera Photo in Unreal C++

## Index

- [Introduction](#introduction)
	- [Credits](#credits)
 	- [Work Flow](#work-flow)
  		- [Unreal Version and Template](#unreal-version-and-template)
    		- [Folder Hierarchy](#folder-hierarchy)
      		- [Unreal Editor](#unreal-editor)
        	- [Nomenclature](#nomenclature)
- [Tutorial](#tutorial)
	- [The USceneCaptureComponent2D](#the-uscenecapturecomponent2d)
 	- [Display Picture on Screen](#display-picture-on-screen)
  		- [Texture and Material](#texture-and-material)
  		- [Camera Widget](#camera-widget)
  		- [The HUD Wrapper](#the-hud-wrapper)
  		- [Connect all With Game Mode](#connect-all-with-a-game-mode)
  		- [SceneCapture Settings](#scenecapture-settings)
    	- [Save Images in Game (Not Disk)](#save-images-in-game)	 
  	    

## Introduction
This all starts because I wanted to make a photography game in Unreal with C++, but none of the information I found about it was what I needed, so with this tutorial, I will try to help anyone who wants to implement this mechanic in their game.

In this tutorial you will learn how to take screenshots, save them in the game (not on disk) and display them on screen. As I progress in my other project I will be uploading new features.

This is what you will be able to achieve with the tutorial:



https://github.com/Cervath/Unreal_Camera_Mode/assets/99689238/160fc2fd-efeb-430b-9bbe-65bd3dbc35cf



Future changes to be implemented in this tutorial:



https://github.com/Cervath/Unreal_Camera_Mode/assets/99689238/1c5ebd5e-7201-4227-a649-da24f91e44ee



### Credits
Much of what is going to be done in this tutorial is taken from this series of videos: https://www.youtube.com/watch?v=kD6-YhuxQNs&t

What I have done is to translate from Blueprints to C++.
### Work Flow

#### Unreal Version and template
To implement this I work in the 5.2.1 version and use the FirstPerson template.

#### Folder Hierarchy

##### Code
- PhotoTest
    - Private/Public
        - Components
        - GameMode
        - UI
        - Widgets
  
##### Unreal Editor
- Content
    - Core
        - Camera
            - Material
        - GameMode
        - UI

#### Nomenclature
The variables are lowerCamelCase, and if they are class variables they will end with a _, will have the prefix Category = " Design | " to be able to search them better in the editor.

e.g:
```c++
	UPROPERTY(VisibleAnywhere, Category = "Design | example")
		int32 exampleVar_;
```
All classes start with PH_ to make the classes related to this project clearer and easier to find.

e.g:

PH_ExampleClas

## Tutorial

It would be best to have a class that handles the whole camera, but let's take the character as the main entity for taking pictures.

For this tutorial we will need to have 2 inputs already associated, one to take pictures and one to navigate between them.
### The USceneCaptureComponent2D

Create a input to manage de shoot.

In order to capture an image from our camera we will need a USceneCaptureComponent2D, this component will give us everything we need to capture the scene and modify it.

In this case I put the scene capture in the Character base template.

in the h
```c++

class USceneComponent;

class APhotoTestCharacter : public ACharacter
{

private:

	UPROPERTY(VisibleAnywhere, Category = "Design | Capture")
		USceneCaptureComponent2D* sceneCapture_;
```
in the cpp
```c++
#include "Components/SceneCaptureComponent2D.h"
...
APhotoTestCharacter::APhotoTestCharacter()
{
	
	...template code...
	
//Attatch with the camare we want to follow and have the focus.
	sceneCapture_= CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Photo"));
	sceneCapture_->SetupAttachment(FirstPersonCameraComponent);

}
```
You should get this in the blueprint.

 <img> ![01](https://github.com/Cervath/CameraMode/assets/99689238/d24f81e4-50af-492e-a9cc-0ea298b3d807)

Remember to do a dll cleanup if it doesn't show up, touching the constructor gives problems.

To save the image that we are taking, we will need a TextureRenderTarget2D(Render Target), can be created before or directly from the same tab( I use this) and name it CurrentRenderTarget . 

![02](https://github.com/Cervath/CameraMode/assets/99689238/be4a689a-fa53-4050-aa4e-76516f9c0d34)


Enter the texture and change the resolution to make it look good.

![16](https://github.com/Cervath/CameraMode/assets/99689238/b4a0481b-d90a-4cb2-afd7-6f8d5e14e925)


Now we create a function to be called when we press a button and we will make our first step to make captures.

In the h
```c++
UFUNCTION()
	void TakePicture(const FInputActionValue& Value);
```

In the cpp
```c++
void APhotoTestCharacter::TakePicture(const FInputActionValue& Value){
	sceneCapture_->CaptureScene();
}
```

Compile and It's time to test if it works.

![03](https://github.com/Cervath/CameraMode/assets/99689238/f098aa78-d54e-4163-8a55-b35064ad7635)


### Display picture on screen
Now every picture we take will be saved in that texture. But if we want to be able to take more pictures, save them and display them, we will need to save them in an array of RenderTextures.

But before that, we are going to create and configure a widget to manage the display.

#### Texture and material
The way we are doing it, we are going to need a material containing the RenderTarget, so let's create one and generate an instance of it. drag the texture and link it to the color, don't forget to parameterize and give it a name.

For this to be displayed on the screen we need the domain material to be a User Interface.


![4,1](https://github.com/Cervath/CameraMode/assets/99689238/1d54b19c-dc80-49ee-ab75-0dbccd845da4)

(I took this photo at the end of the tutorial, that's why the texture looks black).
![04](https://github.com/Cervath/CameraMode/assets/99689238/f9c6c233-d068-4c62-a691-72b3c7587df5)


![05](https://github.com/Cervath/CameraMode/assets/99689238/4ed4dca1-34e5-4207-bee9-10959686412d)


#### Camera Widget
Let's create a class that inherits from UserWidget to display the pictures.

![06](https://github.com/Cervath/CameraMode/assets/99689238/83086a19-9d78-4439-84fa-e1f17ef6e5e1)


For now we only need one variable.

In the h
```c++
//Bind widget to use in the blueprint
public:
UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* photoImage_;
```
Now create a Blueprint of this widget.

![07](https://github.com/Cervath/CameraMode/assets/99689238/79bd93ca-b19c-465f-bb71-b10aef8d402b)


Create a canvas and insert an image. The most important thing is that it has the same name as the variable of our c++ class, which in my case is photoImage_.

![08](https://github.com/Cervath/CameraMode/assets/99689238/f7a6a134-5b92-4baf-956e-8bd3916eca48)



In the image, in the appearance section, let's change the brush to the material instance we create before.

![09](https://github.com/Cervath/CameraMode/assets/99689238/bcf29067-fc22-4eb8-abf7-c1d34c43a2a1)


You can test that now the photos you take are saved in the material and in the widget image.

#### The HUD wrapper

We create a class that inherits from HUD and we will use it as a wrapper for the CameraWidget, for now, it will help us to display the widget on screen.

In the h:
```c++
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/PH_CameraWidget.h"
#include "Engine/TextureRenderTarget2D.h"
#include "PH_HUD.generated.h"


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
	
};
```

In the cpp:

```c++
#include "Widgets/PH_CameraWidget.h"
#include "Engine/TextureRenderTarget2D.h"


void APH_HUD::BeginPlay() {

	cameraWidget_ = CreateWidget<UPH_CameraWidget>(PlayerOwner, cameraBP_);

	if (cameraWidget_) {
		cameraWidget_->AddToViewport();
	}

}
```
And in the blueprint change the Camera BP to the CameraWidget BP.

![11](https://github.com/Cervath/CameraMode/assets/99689238/9cde570a-703c-4e55-b16c-9986048121a7)


 #### Connect all with a game mode

We will create a class that inherits from GameMode and we will create a blueprint from it.

![10](https://github.com/Cervath/CameraMode/assets/99689238/d26c8e33-0b0e-44c1-b6ad-51bd330450f9)



In the blueprint change HUD Class and Default Pawn Class to you BP.


![12](https://github.com/Cervath/CameraMode/assets/99689238/2c7d6409-34a7-444d-b604-5fdc4e728d2b)

Now in Project Settings->Maps & Modes change default GameMode to you BP.

![13](https://github.com/Cervath/CameraMode/assets/99689238/75e2df7c-6444-4484-873e-1e70da1482c7)


And finally in the World Settings change the GameMode Override.

![14](https://github.com/Cervath/CameraMode/assets/99689238/f362aac9-76b8-47f2-a22a-ab48c00ac818)


#### SceneCapture settings

If you run the game you should be able to see your image on the screen, but it looks more like a video, so let's change some settings in Scene Capture component.

Uncheck Capture every frame and Capture in motion and check Always persist rendering state (try unchecking to see the difference).

![15](https://github.com/Cervath/CameraMode/assets/99689238/793bc74c-aaa9-4739-a80e-b0d173385554)


### Save images in game

Now we are going to save all the photos we take and browse through them.

In the CameraWidget h:

```c++
		// Material we are going to use to create a dynamic material from it
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
			class UMaterialInterface* baseMaterial_;

		// Function to set the texture of the image
		UFUNCTION(BlueprintCallable, Category = "Image")
			void SetImageTexture(UTextureRenderTarget2D* Texture);
```

In the cpp:

```c++
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
```

Now in the HUD h:

```c++
UFUNCTION()
		void SetImageTexture(UTextureRenderTarget2D* Texture);

```

In the cpp:

```c++
void APH_HUD::SetImageTexture(UTextureRenderTarget2D* Texture) {
	cameraWidget_->SetImageTexture(Texture);
}
```
Now we are going to change our TakePictures function, but for that we will need to some things.

In the h:

```c++
public:

	// To pass to the camera widget the material that we will use to create dynamic materials.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
		class UMaterialInterface* baseMaterial_;
		
private:

	// Link to pass the screenshots to the viewport
	UPROPERTY(VisibleAnywhere, Category = "Design | HUD")
		class APH_HUD* hud_;
		
	// Array to save all images
	UPROPERTY(VisibleAnywhere, Category = "Design | Capture")
			TArray<UTextureRenderTarget2D*> targets_;

```
In the cpp:

```c++
#include "Engine/TextureRenderTarget2D.h"
#include "UI/PH_HUD.h"
#include "Kismet/GameplayStatics.h"

...
void APhotoTestCharacter::BeginPlay()
{
	... Template Code ...

	hud_ = Cast<APH_HUD>(UGameplayStatics::GetPlayerController(GetOwner(), 0)->GetHUD());
	
	hud_->cameraWidget_->baseMaterial_ = baseMaterial_;
}

void APhotoTestCharacter::TakePicture(const FInputActionValue& Value){

	// Texture dimensions
	int32 Width = 1920;
	int32 Height = 1080;

	// Create render target 2D
	UTextureRenderTarget2D* MyRenderTarget = NewObject<UTextureRenderTarget2D>(this, UTextureRenderTarget2D::StaticClass());
	MyRenderTarget->InitAutoFormat(Width, Height);
	MyRenderTarget->UpdateResourceImmediate(true);

	// We will save the output of sceneCapture_ in this texture
	sceneCapture_->TextureTarget = MyRenderTarget;

	// Other settings you want to change on the SceneCaptureComponent2D
	
	
	// Capture the scene
	sceneCapture_->CaptureScene();

	// We add it to our texture array and pass it to the hud to handle the change.
	targets_.Add(MyRenderTarget);
	hud_->cameraWidget_->SetImageTexture(MyRenderTarget);

}
```
And now we just need to be able to switch between images.

In the h:

```c++

private:

		
	// Iterator to switch between images
	UPROPERTY(VisibleAnywhere, Category = "Design | Capture")
		int32 ite_;
		
	// Function to switch between images, REMEMBER TO CREATE A NEW INPUT TO TO LAUNCH THIS FUNCTION
	UFUNCTION()
		void NextImage(const FInputActionValue& Value);
```
In the cpp:
```c++
void APhotoTestCharacter::NextImage(const FInputActionValue& Value) {

	ite_ = (ite_ + 1) % targets.Num();

	hud->cameraWidget_->SetImageTexture(targets[ite_]);

}
```
You already have a camera!

Surely you have noticed that at the 4th picture you take, you get a shadow map error, and the pictures come out with black lines.
In order to fix this, we will need to set r.Shadow.Virtual.Enable to 0 ( 1 to activate). 

We have 2 ways, from the DefaultEngine.ini file (that we can find in the Config folder).
![engine](https://github.com/Cervath/Unreal_Camera_Mode/assets/99689238/65e68bd1-cdf9-4b38-8ed0-122001f09ced)

or from the command line in the editor to see it while we are playing.

![comandos](https://github.com/Cervath/Unreal_Camera_Mode/assets/99689238/ab23d325-ac9d-4771-bcb2-723271b4d923)

But by doing this we will lose some quality in the shadows.

## Fin
And that would be it! Now you should be able to take pictures, save them in the game and navigate through a visual interface.
If you have any questions about the tutorial or how it has been explained, please contact me.
