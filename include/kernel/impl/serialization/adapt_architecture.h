#ifndef YRUI_SERIALIZATION_RAW_ARCHITECTURE_H_
#define YRUI_SERIALIZATION_RAW_ARCHITECTURE_H_

#include <boost/fusion/include/adapt_struct.hpp>

#include <kernel/detail/raw_architecture.h>

BOOST_FUSION_ADAPT_STRUCT(yrui::detail::Raw_architecture,
    (yrui::id_t, id)

        (std::string, name)

            (yrui::id_t, fraction)(yrui::id_t, mayor)

                (yrui::arch::Archtype, type)

                    (int, farming)(int, max_farming)(int, farming_growth)

                        (int, trade)(int, max_trade)(int, trade_growth)

                            (int, steady)(int, max_steady)

                                (int, security)(int, security_growth)

                                    (int, population)(int, max_population)(int, population_growth)

                                        (int, money)(int, food)

                                            (int, corruption)(int, efficiency))

#endif // !YRUI_SERIALIZATION_RAW_ARCHITECTURE_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
