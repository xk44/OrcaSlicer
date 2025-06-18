#ifndef slic3r_BuildPlate_hpp_
#define slic3r_BuildPlate_hpp_

#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

namespace Slic3r {

using BedTypeIndex = size_t;
// Temporary alias for legacy code using BedType
using BedType = BedTypeIndex;

// Legacy built-in bed type indices kept for compatibility
constexpr BedTypeIndex btDefault    = 0;
constexpr BedTypeIndex btPC        = 1;
constexpr BedTypeIndex btEP        = 2;
constexpr BedTypeIndex btPEI       = 3;
constexpr BedTypeIndex btPTE       = 4;
constexpr BedTypeIndex btPCT       = 5;
constexpr BedTypeIndex btSuperTack = 6;
constexpr BedTypeIndex btCount     = 7;

struct BuildPlateDef {
    std::string uuid;              // unique identifier, generated if empty
    std::string display_name;      // user visible name
    int         bambu_code = -1;   // numeric ID used by Bambu printers (-1 if not mapped)
    int         def_bed_temp = 0;  // default bed temperature
    int         def_bed_temp_first = 0; // default first layer bed temperature
    double      def_z_offset = 0.0; // default z offset
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BuildPlateDef, uuid, display_name, bambu_code,
                                   def_bed_temp, def_bed_temp_first, def_z_offset)

class BuildPlateManager
{
public:
    static BuildPlateManager& inst();

    // Populate with built-in plates if no persistence file exists
    void load_defaults();
    static std::string gen_uuid();

    // Helper to access a plate definition by index
    const BuildPlateDef& plate(BedTypeIndex idx) const;

    // CRUD operations on build plates
    const std::vector<BuildPlateDef>& plates() const { return m_plates; }
    std::optional<size_t> index_from_uuid(const std::string& uuid) const;
    bool add_plate(const BuildPlateDef& def);
    bool update_plate(size_t index, const BuildPlateDef& def);
    bool remove_plate(size_t index);

    // persistence
    bool load(const std::string& path);
    bool save(const std::string& path) const;

    // event dispatch
    // GUI may subscribe to this to refresh comboboxes when plates change
    struct Listener {
        virtual void plates_changed() = 0;
    };
    void add_listener(Listener* l);
    void remove_listener(Listener* l);

private:
    BuildPlateManager() = default;
    std::vector<BuildPlateDef> m_plates;
    std::vector<Listener*> m_listeners;
    void notify_changed();
};

} // namespace Slic3r

#endif // slic3r_BuildPlate_hpp_
