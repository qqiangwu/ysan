#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <boost/filesystem.hpp>

#include <thread>

#include "foundation/contract.h"
#include "foundation/log_support.h"

#include "kernel/core.h"

#include "ui/bygame.h"
#include "ui/byscreen.h"
#include "ui/form/aboutform.h"
#include "ui/form/introform.h"
#include "ui/form/selectform.h"
#include "ui/form/splashform.h"

#include "kernel/impl/bygameimpl.h"

namespace {
std::vector<std::string> getScenarios(const std::string& scenarioDir)
{
    using namespace boost::filesystem;

    std::vector<std::string> scenarios;

    std::for_each(directory_iterator(scenarioDir), directory_iterator(), [&scenarios](const directory_entry& e) {
        if (e.path().extension() == ".bys") {
            scenarios.push_back(e.path().string());
        }
    });

    return scenarios;
}
}

Bygone::Bygame::Bygame(const yrui::Foundation& fnd)
    : fnd_(fnd)
    , background_ {}
{
    const auto& gui_conf = fnd.gui_conf();
    const auto& app_conf = fnd.app_conf();

    // screen
    screen_.setFixedWidth(app_conf.get("app.width", 960));
    screen_.setFixedHeight(app_conf.get("app.height", 720));
    screen_.show();

    // forms
    auto intro = new IntroForm(gui_conf.get_child("gui.intro"), &screen_);
    auto about = new AboutForm(gui_conf.get_child("gui.about"), &screen_);
    auto select = new SelectForm(
        gui_conf.get_child("gui.select"), getScenarios(app_conf.get<std::string>("app.scenarioDir")), &screen_);
    auto splash = new SplashForm(gui_conf.get_child("gui.splash"), &screen_);

    splash->setObjectName("splash");

    screen_.addWidget(intro);
    screen_.addWidget(about);
    screen_.addWidget(select);
    screen_.addWidget(splash);

    connect(intro, &intro->exitClicked, qApp, &QApplication::quit);
    connect(intro, &intro->startClicked, [this, select] { screen_.setCurrentWidget(select); });
    connect(intro, &intro->aboutClicked, [this, about] { screen_.setCurrentWidget(about); });

    connect(select, &select->cancelClicked, [this, intro] { screen_.setCurrentWidget(intro); });
    connect(select, &select->selectionDone, this, &loadGame);

    connect(about, &about->confirmClicked, [this, intro] { screen_.setCurrentWidget(intro); });
}

void Bygone::Bygame::loadGame(const std::string& config, const int leaderId)
{
    YPRE_CONDITION(background_ == nullptr);

    YLOG() << "Load game...";

    auto& conf = fnd_.gui_conf().get_child("gui.splash.text");
    auto splash = screen_.findChild<SplashForm*>("splash");

    YCONSTRAINT(splash);

    background_ = new ByWorker(config, leaderId, this);

    connect(background_, &ByWorker::started, this, [this, splash, &conf] {
        screen_.setCurrentWidget(splash);
        splash->showMessage(conf.get("createWorld", "Create World").c_str());
    });

    connect(background_, &ByWorker::scenarioLoaded, this,
        [this, splash, &conf] { splash->showMessage(conf.get("initGame", "Init Game").c_str()); });

    connect(background_, &ByWorker::gameInited, this, [this, splash, &conf] {
        auto gameScreen = new ByScreen(*this, &screen_);

        gameScreen->setObjectName("gameScreen");
        screen_.addWidget(gameScreen);
        screen_.setCurrentWidget(gameScreen);
    });

    connect(background_, &ByWorker::errorCatched, this, &reportError);

    background_->start();
}

void Bygone::Bygame::reportError(std::exception_ptr exp)
{
    YLOG_SEV(fatal) << "Detect error";

    try {
        std::rethrow_exception(exp);
    } catch (std::exception& e) {
        QMessageBox::information(nullptr, "Error", e.what());
        throw;
    }
}
