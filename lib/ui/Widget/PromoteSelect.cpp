#include "Widget/PromoteSelect.h"
#include "Detail/Filter.h"
#include "kernel/core.h"
#include "ui_PromoteSelect_imp.h"

namespace Fr = Bygone::Frontend;

Fr::PromoteSelect::PromoteSelect(result_type current, result_type max)
    : QDialog {}
    , ui(new Ui::PromoteSelect)
{
    ui->setupUi(this);

    using namespace yrui::person;

    BOOST_ASSERT(current >= 0);
    BOOST_ASSERT(current < max);
    BOOST_ASSERT(max < position_const::leader);

    // auto& table = yrui::core()->string_table();

    from_ = static_cast<Position>(current + 1);

    auto combo = ui->comboBox;

    combo->clear();
    for (auto from = from_; from <= max; from = static_cast<result_type>(from + 1)) {
        combo->addItem(filter_position(from));
    }
}

Fr::PromoteSelect::~PromoteSelect() { delete ui; }

Fr::PromoteSelect::result_type Fr::PromoteSelect::result() const
{
    auto res = from_ + ui->comboBox->currentIndex();

    BOOST_ASSERT(0 < res && res < yrui::person::position_const::leader);

    return static_cast<result_type>(res);
}
