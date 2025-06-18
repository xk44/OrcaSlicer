#include <catch2/catch.hpp>
#include "libslic3r/BuildPlate.hpp"
#include <filesystem>
using namespace Slic3r;

TEST_CASE("BuildPlateManager CRUD and serialization", "[BuildPlateManager]") {
    auto &mgr = BuildPlateManager::inst();
    mgr.load_defaults();
    size_t start_count = mgr.plates().size();
    BuildPlateDef def{BuildPlateManager::gen_uuid(), "Test Plate", 9, 60, 65, 0.1};
    REQUIRE(mgr.add_plate(def));
    auto idx_opt = mgr.index_from_uuid(def.uuid);
    REQUIRE(idx_opt);
    size_t idx = *idx_opt;
    REQUIRE(mgr.plates().size() == start_count + 1);
    BuildPlateDef def2 = def;
    def2.display_name = "Renamed";
    REQUIRE(mgr.update_plate(idx, def2));
    REQUIRE(mgr.plate(idx).display_name == "Renamed");
    std::filesystem::path p = std::filesystem::temp_directory_path() / "plates_test.json";
    REQUIRE(mgr.save(p.string()));
    mgr.load_defaults();
    REQUIRE(mgr.plates().size() == start_count);
    REQUIRE(mgr.load(p.string()));
    REQUIRE(mgr.plates().size() == start_count + 1);
    REQUIRE(mgr.plate(idx).display_name == "Renamed");
    REQUIRE(mgr.remove_plate(idx));
    REQUIRE(mgr.plates().size() == start_count);
    std::filesystem::remove(p);
}

TEST_CASE("BuildPlateManager UUID generation", "[BuildPlateManager]") {
    std::string u1 = BuildPlateManager::gen_uuid();
    std::string u2 = BuildPlateManager::gen_uuid();
    REQUIRE(u1 != u2);
    REQUIRE(u1.size() == 32);
    REQUIRE(u2.size() == 32);
}
