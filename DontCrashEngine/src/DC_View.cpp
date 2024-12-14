#include "DC_View.h"

// Include from Utility
#include <Hash.h>

namespace DC_Engine
{
    const DC_View::Id kPlayerViewId = YS_Util::HashString("PlayerView");
    const DC_View::Id kEnemyViewId = YS_Util::HashString("EnemyView");
    const DC_View::Id kGameViewId = YS_Util::HashString("GameView");
    const DC_View::Id kBulletViewId = YS_Util::HashString("BulletView");
    const DC_View::Id kEndGameId = YS_Util::HashString("EndGameView");
}
