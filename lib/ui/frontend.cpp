#include "ui/Frontend.h"
#include "Main/Byapp.h"
#include "foundation/foundation.h"
#include <QDebug>
#include <QMessageBox>
#include <boost/format.hpp>
#include <exception>
#include <QtPlugin>

Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)

namespace Bygone {
namespace Frontend {
    int run(int argc, char* argv[])
    try {
        // TODO
        //yrui::Foundation _(argc, argv);
        Byapp app(argc, argv);

        app.setApplicationDisplayName("亦三国");
        app.setApplicationName("亦三国");

        return app.exec();
    } catch (std::exception& e) {
        QMessageBox::information(nullptr, "Error", e.what());
        return 1;
    }
}
}
