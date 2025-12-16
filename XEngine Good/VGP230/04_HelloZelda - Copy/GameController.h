#pragma once
#include "Entity.h"
#include "Player.h"
#include "BulletPool.h"

class GameController : public Entity
{
public:
    ~GameController();
    static GameController* Get();

    // Entity override
    void Load() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Unload() override;

private:
    GameController();
    static GameController* mInstance;

    void HandleShooting(float deltaTime);

    Player mPlayer;
    BulletPool  mBulletPool;
};