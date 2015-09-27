#include <iostream>
#include "hesiod/lib.hpp"
#include "hesiod/logger.hpp"
#include "hesiod/manipulator.hpp"

namespace log {

class html_error_formatter {
public:
    static std::string start() {
        return R"(<font face="courier new" size="2" color="#FF0000")";
    }

    static std::string end() {
        return "</font>";
    }

    static char line() {
        return '\n';
    }
};

class html_info_formatter {
public:
    static std::string start() {
        return R"(<font face="courier new" size="2" color="#00FF00")";
    }

    static std::string end() {
        return "</font>";
    }

    static std::string line() {
        return "</hr>";
    }
};

using simple_logger_t = hesiod::logger<char>;
static simple_logger_t info(std::cout);
static simple_logger_t error(std::cout);

static auto endl = hesiod::endl_m<simple_logger_t>();

}

int main(int argc, char **argv) {
    hesiod::lib();

    log::info.write("This is an % log\n", "info");
    log::error.write("This is an % log\n", "error");

    log::info << "1" << "2" << "3" << 4 << 5.5 << log::endl;
    log::error << "1" << "2" << "3" << 4 << 5.5 << log::endl;

    return 0;
}
