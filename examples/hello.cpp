#include <iostream>
#include <fstream>

#include "hesiod/lib.hpp"
#include "hesiod/stream.hpp"
#include "hesiod/logger.hpp"
#include "hesiod/manipulator.hpp"
#include "hesiod/standard_formatter.hpp"

namespace log {

using standard_formatter = hesiod::standard_formatter<char>;

class html_formatter : public hesiod::formatter<char> {
public:
    static std::string start() {
        return "<font face=\"courier new\" size=\"2\" color=\"#FF0000\"";
    }

    static std::string end() {
        return "</font>";
    }

    static std::string line() {
        return "<hr>";
    }
};

static std::ofstream file("log.html", std::ios::out);
static auto info = hesiod::make_logger<char>(hesiod::stream<standard_formatter>(std::cout));
static auto error = hesiod::make_logger<char>(hesiod::stream<standard_formatter>(std::cout), hesiod::stream<html_formatter>(file));

using endl = hesiod::endl_m;

}


int main(int argc, char **argv) {
    hesiod::lib();

    log::info.write("Write % log\n", "info");
    log::error.write("Write % log\n", "error");

    log::info << "Stream " << "info " << "3" << 4 << 5.5 << log::endl;
    log::error << "Stream " << "error " << "3" << 4 << 5.5 << log::endl;

    return 0;
}
