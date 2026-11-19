#include "collisiondetectionsystem.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../utilities/constants.hpp"
#include "../utilities/physics.hpp"
#include <cstdio>

void CollisionDetectionSystem::update() {

  CollisionDetectionSystem::updateCollisionComponents();
  CollisionDetectionSystem::checkTileCollisions();
  CollisionDetectionSystem::checkEntityCollisions();
}

void CollisionDetectionSystem::checkTileCollision(CollisionComponent &cc,
                                                  PositionComponent const &position,
                                                  VelocityComponent const &velocity) const {
  auto &reg = currentLevel->registry;
  const Velocity &vel = velocity.velocity;

  TileGrid const &grid = currentLevel->staticTileGrid;
  constexpr int pad = 32;
  const int minTx = (cc.collisionRect.x - pad) / Constants::TILE_SIZE;
  const int maxTx = (cc.collisionRect.x + cc.collisionRect.w + pad) / Constants::TILE_SIZE;
  const int minTy = (cc.collisionRect.y - pad) / Constants::TILE_SIZE;
  const int maxTy = (cc.collisionRect.y + cc.collisionRect.h + pad) / Constants::TILE_SIZE;

  for (int ty = minTy; ty <= maxTy; ++ty) {
    for (int tx = minTx; tx <= maxTx; ++tx) {
      std::vector<entt::entity> tileEntities = grid.at(tx, ty);
      if (tileEntities.empty() || tileEntities.at(0) == entt::null) {
        continue;
      }

      for (entt::entity tileAtCoordinate : tileEntities) {
        if (tileAtCoordinate == entt::null) {
          continue;
        }
        CollisionComponent const *tileColl = reg.try_get<CollisionComponent>(tileAtCoordinate);
        TileComponent const *tileTile = reg.try_get<TileComponent>(tileAtCoordinate);
        Velocity tileVel = {0, 0};
        if (reg.try_get<VelocityComponent>(tileAtCoordinate)) {
          tileVel = reg.try_get<VelocityComponent>(tileAtCoordinate)->velocity;
        }
        if (!tileColl || !tileTile) {
          continue;
        }
        TILE const &collisionTile = tileTile->tile;

        checkEntityLeftTileCollision(cc, *tileColl, vel, tileVel, collisionTile);
        checkEntityDownLeftTileCollision(cc, *tileColl, vel, tileVel, collisionTile);
        checkEntityRightTileCollision(cc, *tileColl, vel, tileVel, collisionTile);
        checkEntityDownRightTileCollision(cc, *tileColl, vel, tileVel, collisionTile);
        checkEntityDownTileCollision(cc, *tileColl, vel, tileVel, collisionTile);
        checkEntityUpTileCollision(cc, *tileColl, vel, tileVel, collisionTile);
      }
    }
  }
}

void CollisionDetectionSystem::checkTileCollisions() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<CollisionComponent, PositionComponent, VelocityComponent>();
  for (auto entity : view) {
    CollisionComponent &cc = view.get<CollisionComponent>(entity);
    PositionComponent const &pc = view.get<PositionComponent>(entity);
    VelocityComponent const &vc = view.get<VelocityComponent>(entity);
    checkTileCollision(cc, pc, vc);
  }
}

void CollisionDetectionSystem::checkEntityCollisions() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<CollisionComponent, PositionComponent, VelocityComponent>();
  std::vector<entt::entity> entities(view.begin(), view.end());

  for (std::size_t i = 0; i < entities.size(); ++i) {
    for (std::size_t j = i + 1; j < entities.size(); ++j) {
      auto a = entities[i];
      auto b = entities[j];

      auto &collisionA = view.get<CollisionComponent>(a);
      auto &collisionB = view.get<CollisionComponent>(b);

      if (Physics::checkCollision(collisionA.collisionRect, collisionB.collisionRect)) {
        collisionA.collisionIds.emplace_back(b);
        collisionB.collisionIds.emplace_back(a);
      }
    }
  }
}

void CollisionDetectionSystem::checkEntityLeftTileCollision(CollisionComponent &objectCollision,
                                                            CollisionComponent const &tileCollision,
                                                            Velocity objVel,
                                                            Velocity const &tileVel,
                                                            TILE const &collisionTile) const {

  if (Physics::collisionLeft(objectCollision.collisionRect, objVel, tileCollision.collisionRect,
                             tileVel)) {
    if (collisionTile == TILE::SOLID) {
      objectCollision.left.positionDifference = {
          objectCollision.collisionRect.x -
              (tileCollision.collisionRect.x + tileCollision.collisionRect.w),
          0};
    }

    objectCollision.left.collision = true;
    objectCollision.left.tileTypes.emplace_back(collisionTile);
  }
}

void CollisionDetectionSystem::checkEntityRightTileCollision(
    CollisionComponent &objectCollision, CollisionComponent const &tileCollision, Velocity objVel,
    Velocity const &tileVel, TILE const &collisionTile) const {
  if (Physics::collisionRight(objectCollision.collisionRect, objVel, tileCollision.collisionRect,
                              tileVel)) {
    if (collisionTile == TILE::SOLID) {
      objectCollision.right.positionDifference = {(objectCollision.collisionRect.x +
                                                   objectCollision.collisionRect.w) -
                                                  tileCollision.collisionRect.x},
      0;
    }

    objectCollision.right.collision = true;
    objectCollision.right.tileTypes.emplace_back(collisionTile);
  }
}

void CollisionDetectionSystem::checkEntityUpTileCollision(CollisionComponent &objectCollision,
                                                          CollisionComponent const &tileCollision,
                                                          Velocity objVel, Velocity const &tileVel,
                                                          TILE const &collisionTile) const {
  if (Physics::collisionUp(objectCollision.collisionRect, objVel, tileCollision.collisionRect,
                           tileVel)) {
    if (collisionTile == TILE::SOLID) {
      objectCollision.up.positionDifference = {
          0,(objectCollision.collisionRect.y) -
              (tileCollision.collisionRect.y + tileCollision.collisionRect.h)};
    }

    objectCollision.up.collision = true;
    objectCollision.up.tileTypes.emplace_back(collisionTile);
  }
}

void CollisionDetectionSystem::checkEntityDownTileCollision(CollisionComponent &objectCollision,
                                                            CollisionComponent const &tileCollision,
                                                            Velocity objVel,
                                                            Velocity const &tileVel,
                                                            TILE const &collisionTile) const {
  if (Physics::collisionDown(objectCollision.collisionRect, objVel, tileCollision.collisionRect,
                             tileVel)) {
    if (collisionTile == TILE::SOLID || collisionTile == TILE::LADDERSTART) {
      objectCollision.down.positionDifference = {
          0,(objectCollision.collisionRect.y + objectCollision.collisionRect.h) -
              tileCollision.collisionRect.y};
    }

    objectCollision.down.collision = true;
    objectCollision.down.tileTypes.emplace_back(collisionTile);
  }
}

void CollisionDetectionSystem::checkEntityDownRightTileCollision(
    CollisionComponent &objectCollision, CollisionComponent const &tileCollision, Velocity objVel,
    Velocity const &tileVel, TILE const &collisionTile) const {
  if (Physics::collisionDownRight(objectCollision.collisionRect, tileCollision.collisionRect,
                                  objVel)) {
    if (collisionTile == TILE::SOLID || collisionTile == TILE::LADDERSTART) {

      objectCollision.downRight.collision = true;
    }
  }
}

void CollisionDetectionSystem::checkEntityDownLeftTileCollision(
    CollisionComponent &objectCollision, CollisionComponent const &tileCollision, Velocity objVel,
    Velocity const &tileVel, TILE const &collisionTile) const {
  if (Physics::collisionDownLeft(objectCollision.collisionRect, tileCollision.collisionRect,
                                 objVel)) {
    if (collisionTile == TILE::SOLID || collisionTile == TILE::LADDERSTART) {

      objectCollision.downLeft.collision = true;
    }
  }
}

void CollisionDetectionSystem::updateCollisionComponents() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<CollisionComponent, PositionComponent>();

  for (auto entity : view) {
    CollisionComponent &cc = view.get<CollisionComponent>(entity);
    PositionComponent const &pc = view.get<PositionComponent>(entity);
    updateCollisionRect(cc, pc);
    resetCollisions(cc);
  }
}

void CollisionDetectionSystem::updateCollisionRect(
    CollisionComponent &collisionComponent, PositionComponent const &transformComponent) const {
  collisionComponent.collisionRect.x = transformComponent.position.x;
  collisionComponent.collisionRect.y = transformComponent.position.y;

  collisionComponent.center = {
      (collisionComponent.collisionRect.x + collisionComponent.collisionRect.w +
       collisionComponent.collisionRect.x) /
          2,
      (collisionComponent.collisionRect.y + collisionComponent.collisionRect.h +
       collisionComponent.collisionRect.y) /
          2};
}

void CollisionDetectionSystem::resetCollisions(CollisionComponent &collisionComponent) const {
  collisionComponent.down.reset();
  collisionComponent.up.reset();
  collisionComponent.left.reset();
  collisionComponent.right.reset();
  collisionComponent.downRight.reset();
  collisionComponent.downLeft.reset();
}

void CollisionDetectionSystem::checkProjectileCollision(
    ProjectileComponent &projectile, CollisionComponent const &projectileCollision) const {
  projectile.collided = projectileCollision.down.collision || projectileCollision.up.collision ||
                        projectileCollision.left.collision || projectileCollision.right.collision;
}
