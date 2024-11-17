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
