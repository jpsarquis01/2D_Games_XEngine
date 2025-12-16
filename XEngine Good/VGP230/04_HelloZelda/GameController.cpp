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
    
    // Spawn initial gun pickup
    PickupManager::Get()->SpawnPickup(1, PT_GUN);
}

void GameController::Update(float deltaTime)
{
    if (mPlayer.GetHealth() <= 0)
    {
        return;  
    }

    TileMap::Get()->Update(deltaTime);
    mPlayer.Update(deltaTime);

    // Update enemies with player position so they can chase
    EnemyManager::Get()->UpdatePlayerPosition(mPlayer.GetPosition());

    // Handle weapon-specific actions
    if (mPlayer.GetCurrentWeapon() == WT_GUN)
    {
        HandleShooting(deltaTime);
    }
    else if (mPlayer.GetCurrentWeapon() == WT_PUNCH)
    {
        HandlePunch(deltaTime);
    }

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

    // --- HUD: Weapon, Ammo and Health ---
    const int screenWidth = X::GetScreenWidth();
    const int screenHeight = X::GetScreenHeight();

    // Health counter
    char healthText[64];
    sprintf_s(healthText, "Health: %d", mPlayer.GetHealth());

    float healthPosX = static_cast<float>(screenWidth) - 150.0f;
    float healthPosY = static_cast<float>(screenHeight) - 80.0f; 

    X::DrawScreenText(healthText, healthPosX, healthPosY, 20.0f, X::Colors::Red);

    // Current weapon
    char weaponText[64];
    if (mPlayer.GetCurrentWeapon() == WT_PUNCH)
    {
        sprintf_s(weaponText, "Weapon: PUNCH");
    }
    else
    {
        sprintf_s(weaponText, "Weapon: GUN");
    }
    
    float weaponPosX = static_cast<float>(screenWidth) - 150.0f;
    float weaponPosY = static_cast<float>(screenHeight) - 50.0f;
    X::DrawScreenText(weaponText, weaponPosX, weaponPosY, 20.0f, X::Colors::Yellow);

    // Ammo counter (only for gun)
    if (mPlayer.GetCurrentWeapon() == WT_GUN)
    {
        char ammoText[64];
        sprintf_s(ammoText, "Ammo: %d", mPlayer.GetAmmo());

        float ammoPosX = static_cast<float>(screenWidth) - 150.0f;
        float ammoPosY = static_cast<float>(screenHeight) - 20.0f;

        X::DrawScreenText(ammoText, ammoPosX, ammoPosY, 20.0f, X::Colors::White);
    }
    
    // Controls hint
    X::DrawScreenText("1: Punch | 2: Gun | LMB: Attack", 10.0f, static_cast<float>(screenHeight) - 30.0f, 16.0f, X::Colors::White);
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

void GameController::HandlePunch(float deltaTime)
{
    if (!X::IsMousePressed(X::Mouse::LBUTTON))
        return;
    
    mPlayer.PerformPunch();
    
    if (!mPlayer.IsPunching())
        return;
    
    // Check punch collision with enemies
    const X::Math::Rect& punchRect = mPlayer.GetPunchRect();
    const std::vector<Enemy*>& enemies = EnemyManager::Get()->GetEnemies();
    
	// Knockback for enemies
    for (Enemy* enemy : enemies)
    {
        if (enemy->IsActive() && X::Math::Intersect(punchRect, enemy->GetRect()))
        {
            // Calculate knockback direction
            X::Math::Vector2 knockbackDir;
            knockbackDir.x = cosf(mPlayer.GetPunchDirection());
            knockbackDir.y = sinf(mPlayer.GetPunchDirection());
            enemy->ApplyKnockback(knockbackDir, 400.0f); // 400 = force multiplier
        }
    }
    
    // Check punch collision with destructible tiles
    CheckDestructibleTiles();
}

void GameController::CheckDestructibleTiles()
{
    if (!mPlayer.IsPunching())
        return;
        
    const X::Math::Rect& punchRect = mPlayer.GetPunchRect();
    TileMap::Get()->DamageDestructibleTiles(punchRect, 1); // Player does 1 damage per punch
}