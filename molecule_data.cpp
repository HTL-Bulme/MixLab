#include "molecule_data.hpp"

namespace mixlab {

static const std::vector<ElementInfo> kElements = {
    {"H",  "Wasserstoff", 1, "#B5D4F4", "#0C447C"},
    {"O",  "Sauerstoff",  2, "#9FE1CB", "#085041"},
    {"Na", "Natrium",     1, "#FAC775", "#633806"},
    {"Cl", "Chlor",      1, "#C0DD97", "#27500A"},
    {"Fe", "Eisen",      3, "#F5C4B3", "#712B13"},
    {"C",  "Kohlenstoff",4, "#D3D1C7", "#2C2C2A"},
    {"N",  "Stickstoff", 3, "#CECBF6", "#3C3489"},
    {"S",  "Schwefel",   2, "#FAC775", "#412402"},
};

static const std::vector<MoleculeInfo> kMolecules = {
    {"H2O",  "Wasser", ReactionStatus::Safe,      "Ungefährlich", "Verhältnis korrekt: 2 H + 1 O", "Synthese", "Flüssig"},
    {"H2O2","Wasserstoffperoxid", ReactionStatus::Warning,   "Vorsicht",     "Oxidation", "Synthese", "Flüssig"},
    {"HCl", "Chlorwasserstoff", ReactionStatus::Dangerous, "Gefährlich",   "Synthese", "Gas", "Gas"},
    {"NH3", "Ammoniak", ReactionStatus::Warning,    "Gefährlich",   "Synthese", "Gas", "Gas"},
    {"NaCl","Kochsalz", ReactionStatus::Safe,      "Ungefährlich", "Ionisch", "Kristall", "Kristall"},
    {"CO2", "Kohlendioxid", ReactionStatus::Warning,   "Mäßig",       "Verbrennung", "Gas", "Gas"},
    {"CO", "Kohlenmonoxid", ReactionStatus::Dangerous, "Tödlich",     "Verbrennung", "Gas", "Gas"},
    {"SO2", "Schwefeldioxid", ReactionStatus::Dangerous, "Gefährlich",   "Verbrennung", "Gas", "Gas"},
};

const std::vector<ElementInfo>& getElementData() {
    return kElements;
}

const std::vector<MoleculeInfo>& getMoleculeDatabase() {
    return kMolecules;
}

std::optional<ElementInfo> findElementInfo(const std::string& symbol) {
    for (const auto& item : kElements) {
        if (item.symbol == symbol) {
            return item;
        }
    }
    return std::nullopt;
}

std::optional<MoleculeInfo> findMoleculeInfo(const std::string& formula) {
    for (const auto& item : kMolecules) {
        if (item.formula == formula) {
            return item;
        }
    }
    return std::nullopt;
}

} // namespace mixlab
