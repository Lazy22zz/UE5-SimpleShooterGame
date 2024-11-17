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
