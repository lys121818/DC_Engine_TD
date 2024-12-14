#include "DC_ObjectComponent.h"
#include <Hash.h>
namespace DC_Engine
{
    const DC_ObjectComponent::Id kTransformId = YS_Util::HashString("TransformComponent");
    const DC_ObjectComponent::Id kImageId = YS_Util::HashString("ImageRenderComponent");
    const DC_ObjectComponent::Id kSoundId = YS_Util::HashString("SoundComponent");
    const DC_ObjectComponent::Id kTriggerBoxId = YS_Util::HashString("TriggerBoxComponent");
    const DC_ObjectComponent::Id kEventActionId = YS_Util::HashString("EventActionComponent");
    const DC_ObjectComponent::Id kDynamicBodyId = YS_Util::HashString("DynamicBodyComponent");
    const DC_ObjectComponent::Id kStaticBodyId = YS_Util::HashString("StaticBodyComponent");
    const DC_ObjectComponent::Id kTileId = YS_Util::HashString("TileSetComponent");

    DC_ObjectComponent::DC_ObjectComponent(GameObject* pOwner, std::string name)
        : m_pOwner(pOwner), m_id(YS_Util::HashString(name))
    {
    }


}
