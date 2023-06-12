#ifndef BYAPP_H
#define BYAPP_H

#include <QtCore/QObject>
#include <QtWidgets/QStackedWidget>

#include <exception>
#include <map>
#include <memory>

#include "foundation/foundation.h"
#include "kernel/component_fwd.h"
#include "ui/byworker.h"

namespace Bygone {
class Bygame : public QObject {
    Q_OBJECT

public:
    Bygame(const yrui::Foundation& fnd);

private slots:
    void loadGame(const std::string& scenarioConfig, const int leaderId);
    void reportError(std::exception_ptr exp);

private:
    const yrui::Foundation& fnd_;

    QStackedWidget screen_;
    ByWorker* background_ {};

private:
    class Impl;
    friend class Impl;
};
}

#endif // BYAPP_H
