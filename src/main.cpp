#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "foundation/foundation.h"
#include "ui/frontend.h"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    try {
        using namespace yrui;
        using namespace Bygone;

        return run(Foundation(argc, const_cast<const char**>(argv)));
    } catch (std::exception& e) {
        QMessageBox::information(nullptr, "Error", e.what());
        return -1;
    }
}
