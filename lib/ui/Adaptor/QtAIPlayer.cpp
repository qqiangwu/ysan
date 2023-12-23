#include "Adaptor/QtAIPlayer.h"
#include "Debugger.h"
#include "Main/Byapp.h"
#include <thread>

#include "kernel/fraction/fraction.h"
#include "kernel/person/person.h"

namespace Bygone {
namespace Frontend {
    void QtAIPlayer::prepare(const yrui::Timeline& tl)
    {
        qDebug() << this->self()->name() << " is thinking";
        emit aiThinking(this->self()->name().c_str());
    }

    void QtAIPlayer::end(const yrui::Timeline& tl)
    {
#ifdef NDEBUG
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
#endif
    }
}
}
