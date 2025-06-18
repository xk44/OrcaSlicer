#include "BuildPlate.hpp"

#include <algorithm>
#include <fstream>
#include <random>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Slic3r {

BuildPlateManager& BuildPlateManager::inst()
{
    static BuildPlateManager instance;
    if (instance.m_plates.empty())
        instance.load_defaults();
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

void BuildPlateManager::load_defaults()
{
    m_plates.clear();
    m_plates.push_back({gen_uuid(), "Default Plate", -1, 0, 0, 0.0});
    m_plates.push_back({gen_uuid(), "Cool Plate", 1, 0, 0, 0.0});
    m_plates.push_back({gen_uuid(), "Engineering Plate", 2, 0, 0, 0.0});
    m_plates.push_back({gen_uuid(), "High Temp Plate", 3, 0, 0, 0.0});
    m_plates.push_back({gen_uuid(), "Textured PEI Plate", 4, 0, 0, 0.0});
    m_plates.push_back({gen_uuid(), "Textured Cool Plate", 1, 0, 0, 0.0});
    m_plates.push_back({gen_uuid(), "Supertack Plate", 5, 0, 0, 0.0});
    notify_changed();
}

std::string BuildPlateManager::gen_uuid()
{
    static const char* hex = "0123456789abcdef";
    std::string out;
    out.reserve(32);
    std::random_device rd;
    for (int i = 0; i < 32; ++i)
        out.push_back(hex[rd() % 16]);
    return out;
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

const BuildPlateDef& BuildPlateManager::plate(BedTypeIndex idx) const
{
    static BuildPlateDef dummy{};
    if (idx < m_plates.size())
        return m_plates[idx];
    return dummy;
}

} // namespace Slic3r

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Slic3r::BuildPlateDef, uuid, display_name, bambu_code,
                                   def_bed_temp, def_bed_temp_first, def_z_offset)
