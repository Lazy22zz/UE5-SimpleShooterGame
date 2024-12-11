- This is expanded
# 1, enable the virbation
- in AplayerController.h
```c++
ENGINE_API void PlayDynamicForceFeedback(float Intensity, float Duration, bool bAffectsLeftLarge, bool bAffectsLeftSmall, bool bAffectsRightLarge, bool bAffectsRightSmall, TEnumAsByte<EDynamicForceFeedbackAction::Type> Action, FLatentActionInfo LatentInfo);
```
- examples:
```c++
void AShooterCharacter::ControllerVibration()
{
	APlayerController* PController = Cast<APlayerController>(GetController());

	if (PController)
	{
		PController -> PlayDynamicForceFeedback(1.0f, 0.4f, true, true, true, true);
	}
}
```
# 2, Enable the switch cameras by blueprint
- Create two camera components
- ![屏幕截图 2024-11-17 113255](https://github.com/user-attachments/assets/0e1d884c-3ca3-4458-9a42-4a1b46a5af05)
# 3, Enable the camerasshake
- In UE5, create a bp_legencycamerashake
- In character.h
- create a `TsubclassOf<UCamerashake> CameraShakeClass`, then
```c++
GetWorld() -> GetFirstPlayerController() -> ClientStartCameraShake(HitCameraShakeClass);
```
# 4, enable the hit-react animation by blend space
- create a ishit variable in the abs_characteranimation
- we need to clarify the direction from shooter
```c++
FVector DamageDirection = CalculateDamageDirection(DamageCauser);
```
- convert 3d to 2d
```c++
	DamageDirectionForAnimation = FVector2D(
		FVector::DotProduct(GetActorRightVector(), DamageDirection),  // X (Left/Right)
		FVector::DotProduct(GetActorForwardVector(), DamageDirection) // Y (Front/Back)
	);
``` 
- ![屏幕截图 2024-11-17 113836](https://github.com/user-attachments/assets/2bc02d25-0a63-400d-949b-992e62d84440) 
- ![屏幕截图 2024-11-17 113855](https://github.com/user-attachments/assets/728ec433-ad59-4dc0-9e74-fbc259ecfcb5)
# 5, enable the dynamic damage widget pop up
- 1, create three collisions for the character: head, body, foot
- ![Screenshot 2024-12-11 122040](https://github.com/user-attachments/assets/e2d87176-aa59-4af1-8c9f-7a99aad0905d)
- 2, create a new dynamicdamageUI: overlay + textblocker
- creating some UI Animations by add Animation.
- ![Screenshot 2024-12-11 122219](https://github.com/user-attachments/assets/1d47f58d-2da7-4494-863e-74d1fc0fbe2f)
- 3, using custom event to enable the animations play, stop
- ![Screenshot 2024-12-11 122801](https://github.com/user-attachments/assets/d243e9d9-3e7c-47bf-b048-d5dccf5c950a)
- ![Screenshot 2024-12-11 122806](https://github.com/user-attachments/assets/26dfc028-e17c-4e0b-88f3-94dc3d98fe61)
- 4, In Gun.h , create a new value to grab the applieddamage
```c++
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Damage")
	float GetDamage = 0.0f;
```
- 5, using FindFuction to find out the new custom event in blueprint.
```c++
UFunction* UObject::FindFunction( FName InName ) const
{
	return GetClass()->FindFunctionByName(InName);
}
```
- 6, using processevent to implement the caught custom blueprint event 
```c++
void UObject::ProcessEvent( UFunction* Function, void* Parms )
```
- 7, implement them in takedamage()
```c++
if (GetDamage > 0.0f && DamageWidgetClass)
	{
		UUserWidget* DamageWidget = CreateWidget<UUserWidget>(GetWorld(), DamageWidgetClass);
        if (DamageWidget)
        {
            DamageWidget->AddToViewport();

			// Set the damage value
			DisplayDamageAni(DamageWidget, GetDamage);

			// Call the function to play a random animation
		    PlayRandomDynamicDamageWidgetAni(DamageWidget);
			
        }
            
    }
```



