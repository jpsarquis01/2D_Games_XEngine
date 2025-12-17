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
    
    // Get player reference
    Player& GetPlayer() { return mPlayer; }

    // Sound
    X::SoundId mBackGround;

private:
    GameController();
    static GameController* mInstance;

    void HandleShooting(float deltaTime);
    void HandlePunch(float deltaTime);
    void CheckDestructibleTiles();

    Player mPlayer;
    BulletPool  mBulletPool;
};