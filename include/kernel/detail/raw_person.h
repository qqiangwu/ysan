#ifndef YRUI_DETAIL_RAW_PERSON_H_
#define YRUI_DETAIL_RAW_PERSON_H_

#include <vector>

#include "kernel/config.h"
#include "kernel/person/position.h"

namespace yrui {
namespace detail {
    struct Raw_person {
        id_t id {};
        std::string name {};

        int born_year {};
        int showed_year {};
        int lifespan {};

        id_t showed_city {};
        id_t relative {};

        id_t fraction {};

        //! \name   person abilities(currently)
        //! @{
        int fight {};
        int leadership {};
        int wisdom {};
        int politics {};
        //! @}

        //! \name   person potential abilities
        //! persons can gain exp to increase their abilities.
        //! @{
        int potential {};
        //! @}

        //! \name   exp
        //! @{
        int fight_exp {};
        int leadership_exp {};
        int wisdom_exp {};
        int politics_exp {};
        //! @}

        //! \name   loyalty
        //! @{
        int loyalty {};
        int max_loyalty {};
        //! @}

        //! \name   troop
        //! @{
        int troop {};
        int max_troop {};
        int morale {};
        int drill {};
        //! @}

        id_t stayed_city {};

        int feat {};

        person::Position position;

        std::vector<id_t> fixed_skill;
    };
} // of namespace detail
} // of namespace yrui

#endif // !YRUI_DETAIL_RAW_PERSON_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
