#include "transformsystem.hpp"
#include "../components/animationcomponent.hpp"
#include "../components/transformcomponent.hpp"

void TransformSystem::moveRight(Player &selectedPlayer) {
    //TODO to animation system
   // if (animationComponent.getAnimationState() != WALKING)
   //   animationComponent.setAnimationState(WALKING);
  
    selectedPlayer.transformComponent.setXvel(selectedPlayer.transformComponent.getXvel() + m_playerSpeed);
  
    if (selectedPlayer.transformComponent.getXvel() > m_playerSpeedMax)
    selectedPlayer.transformComponent.setXvel(m_playerSpeedMax);
  }
  
  void TransformSystem::moveLeft(Player &selectedPlayer) {
    //TODO to animation system
  //  if (animationComponent.getAnimationState() != WALKING)
  //    animationComponent.setAnimationState(WALKING);
  
  selectedPlayer.transformComponent.setXvel(selectedPlayer.transformComponent.getXvel() - m_playerSpeed);
  
    if (selectedPlayer.transformComponent.getXvel() < -m_playerSpeedMax)
    selectedPlayer.transformComponent.setXvel(-m_playerSpeedMax);
  }
  
  void TransformSystem::moveUp(Player &selectedPlayer) { selectedPlayer.transformComponent.setYvel(-m_playerSpeed); }
  
  // debugging only mostly
  void TransformSystem::moveDown(Player &selectedPlayer) { selectedPlayer.transformComponent.setYvel(m_playerSpeed); }
  
  // debugging only mostly
  void TransformSystem::stopX(Player &selectedPlayer) {
    if (selectedPlayer.transformComponent.getXvel() != 0) {
        selectedPlayer.transformComponent.setXvel(0);
        //TODO to animation system
      //animationComponent.setAnimationState(STILL);
    }
  }
  
  // debugging only mostly
  void TransformSystem::stopYU(Player &selectedPlayer) {
    if (selectedPlayer.transformComponent.velocity.y < 0) {
        selectedPlayer.transformComponent.velocity.y = 0;
        //TODO to animation system
    //  animationComponent.animationFrame = STILL;
    }
  }
  
  // debugging only mostly
  void TransformSystem::stopYD(Player &selectedPlayer) {
    if (selectedPlayer.transformComponent.velocity.y > 0) {
        selectedPlayer.transformComponent.velocity.y = 0;
        //TODO to animation system
    //  animationComponent.animationFrame = STILL;
    }
  }
  
  void TransformSystem::update(Player &selectedPlayer) {
    const float xvel = selectedPlayer.transformComponent.getXvel();
    const float yvel = selectedPlayer.transformComponent.getYvel();
    const int xpos = selectedPlayer.transformComponent.getXpos();
    const int ypos = selectedPlayer.transformComponent.getYpos();
  
    if (!(xvel == 0)) {
        selectedPlayer.transformComponent.setXpos(xpos + xvel);
    }
  
    if (!(yvel == 0)) {
        selectedPlayer.transformComponent.setYpos(ypos + yvel);
    }
  
    if (xvel < 0) {
        selectedPlayer.renderComponent.flip = SDL_FLIP_HORIZONTAL;
    }
  
    if (xvel > 0) {
        selectedPlayer.renderComponent.flip = SDL_FLIP_NONE;
    }
  }