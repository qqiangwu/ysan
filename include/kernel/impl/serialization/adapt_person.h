#ifndef YRUI_SERIALIZATION_RAW_PERSON_H_
#define YRUI_SERIALIZATION_RAW_PERSON_H_

/*!
 *
 * \file     Serialization/raw_person.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    adapt the struct raw_person to be used with parser
 * \ingroup  Serialzation
 *
 */

#include <boost/fusion/include/adapt_struct.hpp>

#include "kernel/detail/raw_person.h"

BOOST_FUSION_ADAPT_STRUCT(yrui::detail::Raw_person,

    (yrui::id_t, id)(std::string, name)

        (int, born_year)(int, showed_year)(int, lifespan)

            (yrui::id_t, showed_city)(yrui::id_t, relative)(yrui::id_t, fraction)

                (int, fight)(int, leadership)(int, wisdom)(int, politics)

                    (int, potential)

                        (int, fight_exp)(int, leadership_exp)(int, wisdom_exp)(int, politics_exp)

                            (int, loyalty)(int, max_loyalty)

                                (int, troop)(int, max_troop)(int, morale)(int, drill)

                                    (yrui::id_t, stayed_city)

                                        (int, feat)

                                            (yrui::person::Position, position)

                                                (std::vector<yrui::id_t>, fixed_skill))
#endif // !YRUI_SERIALIZATION_RAW_PERSON_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
