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
