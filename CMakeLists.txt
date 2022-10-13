cmake_minimum_required(VERSION 3.20)
project(PhysicsSim)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_INCLUDE_DIR C:/Dev/SDL2-devel-2.0.22-VC/SDL2-2.0.22/include)
set(SDL2_LIB_DIR C:/Dev/SDL2-devel-2.0.22-VC/SDL2-2.0.22/lib/x64)

set(SDL2_IMAGE_INCLUDE_DIR C:/Dev/SDL2-image/include)
set(SDL2_IMAGE_LIB_DIR C:/Dev/SDL2-image/lib/x64)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR} ${SDL2_IMAGE_LIB_DIR})

add_executable(PhysicsSim main.cpp ECS.h Components/Components.h Components/TransformComponent.h App.h Systems/Systems.h Systems/GravitationalSystem.h Systems/RenderSystem.h Vec2.h Components/RenderComponent.h App.cpp Components/Components.cpp Systems/GravitationalSystem.cpp Systems/RenderSystem.cpp Utility.cpp Utility.h Systems/PhysicsSystem.cpp Systems/PhysicsSystem.h Components/MotionComponent.h Components/PhysicalComponent.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_image)
