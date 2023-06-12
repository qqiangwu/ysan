#include <QtWidgets/QApplication>

#include <exception>

#include "ui/bygame.h"
#include "ui/frontend.h"

namespace Bygone {
int run(const yrui::Foundation& fnd)
{
    Bygame game(fnd);

    return qApp->exec();
}
}
