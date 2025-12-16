#include "GameController.h"
#include "TileMap.h"
#include "CollisionManager.h"
#include "PickupManager.h"
#include "EnemyManager.h"
#include "Bullet.h"

GameController* GameController::mInstance = nullptr;

GameController::GameController()
    : Entity()
{
}

GameController::~GameController()
{
}

GameController* GameController::Get()
{
    if (mInstance == nullptr)
    {
        mInstance = new GameController();
    }
    return mInstance;
}

// Entity override
void GameController::Load()
{
    CollisionManager::Get()->Load();
    TileMap::Get()->Load();
    PickupManager::Get()->Load();
    EnemyManager::Get()->Load();
    mBulletPool.Load();
    mPlayer.Load();
}

void GameController::Update(float deltaTime)
{
    if (mPlayer.GetHealth() <= 0)
    {
        return;  
    }

    TileMap::Get()->Update(deltaTime);
    mPlayer.Update(deltaTime);

    HandleShooting(deltaTime);

    mBulletPool.Update(deltaTime);
    EnemyManager::Get()->Update(deltaTime);
    CollisionManager::Get()->Update(deltaTime);
    PickupManager::Get()->Update(deltaTime);
}

void GameController::Render()
{
    TileMap::Get()->Render();
    PickupManager::Get()->Render();
    mPlayer.Render();
    EnemyManager::Get()->Render();
    mBulletPool.Render();

    CollisionManager::Get()->Render();

    // --- HUD: Ammo and Health bottom-right ---
    const int screenWidth = X::GetScreenWidth();
    const int screenHeight = X::GetScreenHeight();

    // Health counter
    char healthText[64];
    sprintf_s(healthText, "Health: %d", mPlayer.GetHealth());

    float healthPosX = static_cast<float>(screenWidth) - 150.0f;
    float healthPosY = static_cast<float>(screenHeight) - 60.0f; 

    X::DrawScreenText(healthText, healthPosX, healthPosY, 20.0f, X::Colors::Red);

    // Ammo counter
    char ammoText[64];
    sprintf_s(ammoText, "Ammo: %d", mPlayer.GetAmmo());

    float ammoPosX = static_cast<float>(screenWidth) - 150.0f;
    float ammoPosY = static_cast<float>(screenHeight) - 30.0f;

    X::DrawScreenText(ammoText, ammoPosX, ammoPosY, 20.0f, X::Colors::White);
}

void GameController::Unload()
{
    mPlayer.Unload();
    mBulletPool.Unload();
    EnemyManager::Get()->Unload();
    TileMap::Get()->Unload();
    PickupManager::Get()->Unload();
    CollisionManager::Get()->Unload();
}

void GameController::HandleShooting(float deltaTime)
{
    // Only shoot if have ammo
    if (!mPlayer.CanShoot())
        return;

    if (!X::IsMousePressed(X::Mouse::LBUTTON))
        return;

    X::Math::Vector2 playerPos = mPlayer.GetPosition();

    int mouseX = X::GetMouseScreenX();
    int mouseY = X::GetMouseScreenY();
    X::Math::Vector2 mousePos(static_cast<float>(mouseX), static_cast<float>(mouseY));

    X::Math::Vector2 dir = mousePos - playerPos;
    if (X::Math::MagnitudeSqr(dir) == 0.0f)
        return;

    dir = X::Math::Normalize(dir);
    float rotation = atan2f(dir.y, dir.x);

    Bullet* bullet = mBulletPool.GetBullet();
    if (bullet != nullptr)
    {
        bullet->SetActive(playerPos, rotation, 2.0f); // 2 seconds lifetime
        mPlayer.ConsumeAmmo();

    }
}