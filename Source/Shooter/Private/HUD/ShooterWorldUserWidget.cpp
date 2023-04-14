#include "HUD/ShooterWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void UShooterWorldUserWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!IsValid(AttachedActor))
    {
        RemoveFromParent();
        return;
    }

    FVector2D ScreenPosition;
    bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition);

    if (bIsOnScreen) 
    {
        float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

        ScreenPosition /= Scale;

        if (ParentSizeBox)
        {
            ParentSizeBox->SetRenderTranslation(ScreenPosition);
        }
    }

    if (ParentSizeBox)
    {
        ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
    }
}
