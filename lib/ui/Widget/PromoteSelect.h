#ifndef PROMOTESELECT_H
#define PROMOTESELECT_H

#include "kernel/person/position.h"
#include <QDialog>

namespace Ui {
class PromoteSelect;
}

namespace Bygone {
namespace Frontend {
    class PromoteSelect : public QDialog {
        Q_OBJECT

    public:
        using result_type = yrui::person::Position;

    public:
        PromoteSelect(result_type current, result_type max);
        ~PromoteSelect();

    public:
        result_type result() const;

    private:
        Ui::PromoteSelect* ui;
        result_type from_;
    };
}
}

#endif // PROMOTESELECT_H
