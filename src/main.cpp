#include "windows/mainwindow.h"
#include "util/log.h"
#include <QApplication>

#include <QTimer>
#include <iostream>
#include <string_view>
#include <optional>
#include "common/default.h"
#include "common/util.h"

static auto printHelp(std::string_view argv0) -> void {
    std::cout << "Heathkit ET-3400 Emulator v" << getVersion().toStdString() << '\n';
    std::cout << "by David Khristepher Santos\n\n";
    std::cout << "Usage: " << argv0 << " [options] [file]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -m <path>            Load monitor ROM from file\n";
    std::cout << "  -a <start address>   Set start address for execution (hex)\n";
    std::cout << "  -s <speed>           Set clock speed:\n";
    std::cout << "                          n - Percent of default clock speed (" << (DEFAULT_CLOCK_RATE / 1000) <<
            "KHz)\n";
    std::cout << "                          n[k|M]Hz - speed in Hz, kHz or MHz\n";
    std::cout << "  -d                   Show debugger on startup\n";
    std::cout << "  -l <path>            Load labels from file\n";
#ifdef _DEBUG
    std::cout << "  --log-level <level>  Set log level (error, warn, info, debug)" << '\n';
#endif
    std::cout << "  <file>               Load RAM contents from file\n";
    std::cout.flush();
}

auto main(int argc, char *argv[]) -> int {
    // Handle --help and --log-level before constructing any Qt objects
    for (int i = 1; i < argc; ++i) {
        const std::string_view arg(argv[i]);
        if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            return 0;
        }
        if (arg == "--log-level" && i + 1 < argc) {
            Logger::setLevelFromString(argv[++i]);
        }
    }

    QApplication app(argc, argv);

    app.setStyleSheet(
        "QToolButton  { font-size: 10pt; }"
        "QButton      { font-size: 10pt; }"
        "QComboBox    { font-size: 10pt; }"
        "QCheckBox    { font-size: 10pt; }"
        "QGroupBox    { font-size: 10pt; }"
        "QLabel       { font-size: 10pt; }"
        "QLineEdit    { font-size: 10pt; }"
        "QMenu        { font-size: 10pt; }"
        "QMenuBar     { font-size: 10pt; }"
        "QListView    { font-size: 10pt; }"
        "QPushButton  { font-size: 10pt; }"
        "QRadioButton { font-size: 10pt; }"
        "QTabWidget, QTabBar::tab { font-size: 10pt; }"
    );

    std::vector<std::string_view> args;
    args.reserve(argc - 1);
    for (int i {1}; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    std::optional<std::string_view> path;

    MainWindow window;

    for (auto it {args.begin()}; it != args.end(); ++it) {
        const auto& arg = *it;
        if (arg == "--log-level") {
            ++it; // already applied above, skip the value
        } else if (arg == "-a") {
            std::optional<std::string_view> addr = *++it;
            window.setAddress(std::string(*addr));
        } else if (arg == "-m") {
            path = *++it;
            window.setROM(std::string(*path));
        } else if (arg == "-s") {
            std::optional<std::string_view> speed = *++it;
            window.setSpeed(std::string(*speed));
        } else if (arg == "-d") {
            window.setShowDebugger(true);
        } else if (arg == "-l") {
            std::optional<std::string_view> label = *++it;
            window.setLabel(std::string(*label));
        } else {
            path = arg;
            window.setRAM(std::string(*path));
        }
    }

    window.setWindowTitle("ET-3400 Emulator v" + getVersion());
    window.show();

    QTimer::singleShot(0, &window, &MainWindow::start);

    if (auto result {QApplication::exec()}; result != 0) {
        return result;
    }
    return 0;
}