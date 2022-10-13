#include "GravitationalSystem.h"
#include "../Components/Components.h"

extern Coordinator gCoordinator;
constexpr double GravConstant = 6.67e-11;

void GravitationalSystem::Update() {
    std::vector<Entity> entityVec{};
    std::vector <std::tuple<Entity, Entity>> combinations{};

    for (auto const &entity: entities) {
        entityVec.emplace_back(entity);
    }

    for (int i = 0; i < entityVec.size(); ++i) {
        for (int j = i; j < entityVec.size(); ++j) {
            if (i == j) continue;
            auto first = entityVec[i];
            auto second = entityVec[j];

            combinations.emplace_back(std::make_tuple(first, second));
        }
    }

    for(const auto& [entityOne, entityTwo] : combinations) {
        auto& firstPhys = gCoordinator.GetComponent<PhysicalComponent>(entityOne);
        auto& firstTrans = gCoordinator.GetComponent<TransformComponent>(entityOne);
        auto& secondPhys = gCoordinator.GetComponent<PhysicalComponent>(entityTwo);
        auto& secondTrans = gCoordinator.GetComponent<TransformComponent>(entityTwo);

        auto radius = (firstTrans.position - secondTrans.position).length();

        auto firstAppliedToSecond = (firstTrans.position - secondTrans.position) / (firstTrans.position - secondTrans.position).length();
        auto secondAppliedToFirst = (secondTrans.position - firstTrans.position) / (secondTrans.position - firstTrans.position).length();

        auto forceMag = GravConstant * firstPhys.mass * secondPhys.mass / (radius * radius);

        // (A-B)/||A-B||
        // Vector of force that A applies to B

        auto forceFirst = Vec2<double>(secondAppliedToFirst * forceMag);
        auto forceSecond = Vec2<double>(firstAppliedToSecond * forceMag);

        firstPhys.activeForce += forceFirst;
        secondPhys.activeForce += forceSecond;
    }
}

