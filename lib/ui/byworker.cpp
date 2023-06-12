#include "ui/byworker.h"
#include "kernel/core.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_controller.h"
#include "kernel/fraction/fraction_mgr.h"

Q_DECLARE_METATYPE(std::exception_ptr)

namespace {
struct Module {
    Module() { qRegisterMetaType<std::exception_ptr>(); }
};

static Module module;
}

Bygone::ByWorker::ByWorker(const std::string& config, const int leaderId, QObject* parent)
    : QThread(parent)
    , config_ { config }
    , leaderId_ { leaderId }
{
}

void Bygone::ByWorker::run()
try {
    YLOG() << "Create world...";
    auto core = yrui::Core::create(config_);
    emit scenarioLoaded();

    YLOG() << "Init game...";
    initGame();
    emit gameInited();

    YLOG() << "Start game";
    core->run();
} catch (std::exception&) {
    emit errorCatched(std::current_exception());
}

void Bygone::ByWorker::initGame()
{
    using namespace yrui;

    for (auto frac : use_subsys<Fraction_mgr>().as_range()) {
        if (frac->id() == id_t(leaderId_)) {

        } else {
            auto ctrl = make_uptr<fraction::AI_controller>(frac);

            ctrl->install_ai(core()->make_fraction_ai());

            frac->set_controller(std::move(ctrl));
        }
    }
}
