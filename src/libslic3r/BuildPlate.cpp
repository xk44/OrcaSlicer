#include "BuildPlate.hpp"

#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Slic3r {

BuildPlateManager& BuildPlateManager::inst()
{
    static BuildPlateManager instance;
    return instance;
}

std::optional<size_t> BuildPlateManager::index_from_uuid(const std::string& uuid) const
{
    auto it = std::find_if(m_plates.begin(), m_plates.end(), [&](const BuildPlateDef& d){ return d.uuid == uuid; });
    if (it == m_plates.end())
        return std::nullopt;
    return size_t(std::distance(m_plates.begin(), it));
}

bool BuildPlateManager::add_plate(const BuildPlateDef& def)
{
    if (index_from_uuid(def.uuid))
        return false;
    m_plates.push_back(def);
    notify_changed();
    return true;
}

bool BuildPlateManager::update_plate(size_t index, const BuildPlateDef& def)
{
    if (index >= m_plates.size())
        return false;
    m_plates[index] = def;
    notify_changed();
    return true;
}

bool BuildPlateManager::remove_plate(size_t index)
{
    if (index >= m_plates.size())
        return false;
    m_plates.erase(m_plates.begin() + index);
    notify_changed();
    return true;
}

bool BuildPlateManager::load(const std::string& path)
{
    std::ifstream ifs(path);
    if (!ifs.is_open())
        return false;
    json j;
    ifs >> j;
    m_plates.clear();
    for (const auto& item : j)
        m_plates.push_back(item.get<BuildPlateDef>());
    notify_changed();
    return true;
}

bool BuildPlateManager::save(const std::string& path) const
{
    std::ofstream ofs(path);
    if (!ofs.is_open())
        return false;
    json j = m_plates;
    ofs << j.dump(2);
    return true;
}

void BuildPlateManager::add_listener(Listener* l)
{
    if (std::find(m_listeners.begin(), m_listeners.end(), l) == m_listeners.end())
        m_listeners.push_back(l);
}

void BuildPlateManager::remove_listener(Listener* l)
{
    auto it = std::find(m_listeners.begin(), m_listeners.end(), l);
    if (it != m_listeners.end())
        m_listeners.erase(it);
}

void BuildPlateManager::notify_changed()
{
    for (Listener* l : m_listeners)
        if (l)
            l->plates_changed();
}

} // namespace Slic3r

namespace nlohmann {

template <> struct adl_serializer<Slic3r::BuildPlateDef> {
    static void to_json(json& j, const Slic3r::BuildPlateDef& p)
    {
        j = json{
            {"uuid", p.uuid},
            {"display_name", p.display_name},
            {"bambu_code", p.bambu_code},
            {"def_bed_temp", p.def_bed_temp},
            {"def_bed_temp_first", p.def_bed_temp_first},
            {"def_z_offset", p.def_z_offset}
        };
    }

    static void from_json(const json& j, Slic3r::BuildPlateDef& p)
    {
        j.at("uuid").get_to(p.uuid);
        j.at("display_name").get_to(p.display_name);
        j.at("bambu_code").get_to(p.bambu_code);
        j.at("def_bed_temp").get_to(p.def_bed_temp);
        j.at("def_bed_temp_first").get_to(p.def_bed_temp_first);
        j.at("def_z_offset").get_to(p.def_z_offset);
    }
};

} // namespace nlohmann
