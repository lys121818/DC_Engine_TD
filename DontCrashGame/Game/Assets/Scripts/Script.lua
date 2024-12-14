g_playerId = g_sandbox.CreateGameObject("GameObjects/Player.xml");
g_sandbox.CreatePlayerView(g_playerId);

function CreateEnemy()
    local enemyId = g_sandbox.CreateGameObject("GameObjects/Enemy.xml");
    g_sandbox.CreateEnemyView(enemyId);
end

kFramerate = 60;

function PlayerMove(deltaX, deltaY)
    local transform = g_sandbox.GameObjects[g_playerId].TransformComponent;
    transform.TryMove(transform.this, deltaX * kFramerate, deltaY * kFramerate);
end

CreateEnemy(); -- create initial enemy