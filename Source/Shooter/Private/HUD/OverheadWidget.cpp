#include "HUD/OverheadWidget.h"
#include "Components/TextBlock.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
    if (DisplayText)
    {
        DisplayText->SetText(FText::FromString(TextToDisplay));
    }
}

void UOverheadWidget::ShowPlayerNetRole(APawn *InPawn)
{
    ENetRole RemoteRole = InPawn->GetRemoteRole();
    FString Role;

    switch (RemoteRole)
    {
        case ENetRole::ROLE_Authority:
            Role = FString("Authority");
            break;
        case ENetRole::ROLE_AutonomousProxy:
            Role = FString("AutonomousClient");
            break;
        case ENetRole::ROLE_SimulatedProxy:
            Role = FString("SimulatedClient");
            break;
        case ENetRole::ROLE_None:
            Role = FString("None");
            break;
    }

    FString LocalRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
    SetDisplayText(LocalRoleString);
}

void UOverheadWidget::NativeDestruct()
{
    RemoveFromParent();
    Super::NativeDestruct();
}
