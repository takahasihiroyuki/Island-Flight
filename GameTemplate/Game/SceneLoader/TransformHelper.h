#pragma once
#include <tuple>

namespace nsK2EngineLow
{

    static Vector3 ParseVector3(const nlohmann::json& j)
    {
        return Vector3(j.at("x").get<float>(), j.at("y").get<float>(), j.at("z").get<float>());
    }


    static Quaternion ParseQuaternion(const nlohmann::json& j)
    {
        return Quaternion(j.at("x").get<float>(), j.at("y").get<float>(), j.at("z").get<float>(), j.at("w").get<float>());
    }


    using TransformTuple = std::tuple<Vector3, Quaternion, Vector3>;
    static TransformTuple ParseTransformComponents(const nlohmann::json& transformJson)
    {
        Vector3 position = ParseVector3(transformJson.at("position"));
        Quaternion rotation = ParseQuaternion(transformJson.at("rotation"));
        Vector3 scale = ParseVector3(transformJson.at("scale"));
        return std::make_tuple(position, rotation, scale);
    }

}