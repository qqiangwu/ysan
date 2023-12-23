#include "ScreenView/Mapview.h"
#include "Form/ByScreen.h"
#include "Main/Byapp.h"
#include "Widget/GraphicsCity.h"
#include "Widget/InfoBar.h"
#include "Widget/SelectDialog.h"
#include "Widget/SysMenu.h"
#include "Widget/WorkMenu.h"

#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QPixmap>

#include "kernel/arch/architecture.h"
#include "kernel/arch/architecture_mgr.h"
#include "kernel/common/common_util.h"
#include "kernel/core.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include <boost/algorithm/clamp.hpp>
#include <stdexcept>

namespace Bygone {
namespace Frontend {
    namespace {
        constexpr float scale_factor = 1.01f;

        std::map<yrui::id_t, GraphicsCity*> load_architecutre(Byapp* app, MapView* view, int offx, int offy)
        {
            using namespace yrui::arch;

            std::map<yrui::id_t, GraphicsCity*> ret;

            //! prepare resource
            //! [
            auto scene = view->scene();

            auto& ui = yrui::core()->config("ui");
            auto& map = yrui::common::map();
            auto& arch_mgr = yrui::core()->use_subsys<Architecture_mgr>();
            //! ]

            //! load images
            //! [
            QPixmap raw_tiny(ui.get<std::string>("tiny_city.img").c_str());
            QPixmap raw_mid(ui.get<std::string>("mid_city.img").c_str());
            QPixmap raw_big(ui.get<std::string>("big_city.img").c_str());
            QPixmap raw_pass(ui.get<std::string>("pass.img").c_str());
            QPixmap raw_flag(ui.get<std::string>("flag.img").c_str());

            auto tiny_size = ui.get<int>("tiny_city.side");
            auto mid_size = ui.get<int>("mid_city.side");
            auto big_size = ui.get<int>("big_city.side");
            auto pass_size = ui.get<int>("pass.side");
            auto flag_size = ui.get<int>("flag.side");

            auto tiny = raw_tiny.scaled(tiny_size, tiny_size);
            auto mid = raw_mid.scaled(mid_size, mid_size);
            auto big = raw_big.scaled(big_size, big_size);
            auto pass = raw_pass.scaled(pass_size, pass_size);
            auto flag = raw_flag.scaled(flag_size, flag_size);
            //! ]

            auto select_img = [&](Architecture* city) {
                switch (city->type()) {
                case yrui::arch::pass:
                    return &pass;
                case yrui::arch::mid:
                    return &mid;
                case yrui::arch::tiny:
                    return &tiny;
                case yrui::arch::huge:
                    return &big;
                }
            };

            //! add cities
            for (auto v : map.graph().vertex_set()) {
                using namespace yrui::resource;
                auto city = arch_mgr.get(id(v));
                auto loc = map.graph()[v];

                QPixmap* img = select_img(city);

                auto color = city->fraction() ? &app->color(city->fraction()->id()) : nullptr;
                auto item = new GraphicsCity(city, *img, flag, color);

                item->setPos(loc.x() + offx, loc.y() + offy);
                item->setOffset(-img->width() / 2, -img->height() / 2);

                //! connect signals and slots
                QObject::connect(item, SIGNAL(sigDoubleClick(GraphicsCity&)), view, SLOT(centeralize(GraphicsCity&)));
                QObject::connect(item, SIGNAL(sigHoverEnter(GraphicsCity&)), view, SLOT(onHoverEnter(GraphicsCity&)));
                QObject::connect(item, SIGNAL(sigHoverLeave(GraphicsCity&)), view, SLOT(onHoverLeave(GraphicsCity&)));
                QObject::connect(item, SIGNAL(sigClick(GraphicsCity&)), view, SLOT(onClick(GraphicsCity&)));

                ret.emplace(city->id(), item);
                scene->addItem(item);
            }

            return ret;
        }
    }

    MapView::MapView(ByScreen* parent)
        : ByView(parent)
    {
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        const auto map = yrui::core()->config("ui").get<std::string>("map");
        if (!map_layer_.load(map.c_str())) {
            throw std::runtime_error { "cannot load map" };
        }

        assert(!map_layer_.isNull());

        auto scene = new QGraphicsScene(this);
        auto offx = -map_layer_.width() / 2;
        auto offy = -map_layer_.height() / 2;
        scene->setSceneRect(offx, offy, map_layer_.width(), map_layer_.height());

        scene->setItemIndexMethod(QGraphicsScene::NoIndex);
        this->setScene(scene);

        //! load architexteres
        cities_ = load_architecutre(this->app(), this, offx, offy);

        this->setRenderHint(QPainter::Antialiasing);
        this->setCacheMode(QGraphicsView::CacheBackground);

        zoom_ = 0;
        max_zoom_ = 100;
        min_zoom_ = -100;

        //! create widgets
        cityInfo_ = new CityInfo(this);
        cityInfo_->hide();

        workMenu_ = new WorkMenu(this);

        select_ = new SelectDialog(this);

        info_ = new InfoBar(this, this->app()->playerFraction());
    }

    void MapView::mouseMoveEvent(QMouseEvent* event)
    {
        if (this->cursor().shape() == Qt::OpenHandCursor) {
            center_x_ -= event->pos().x() - prev_.x();
            center_x_ = boost::algorithm::clamp(center_x_, -map_layer_.width() / 2, map_layer_.width() / 2);

            center_y_ -= event->pos().y() - prev_.y();
            center_y_ = boost::algorithm::clamp(center_y_, -map_layer_.height() / 2, map_layer_.height() / 2);

            this->centerialize();

            prev_ = event->pos();
        }

        QGraphicsView::mouseMoveEvent(event);
    }

    void MapView::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton) {
            prev_ = event->pos();
            this->setCursor(Qt::OpenHandCursor);
        }

        QGraphicsView::mousePressEvent(event);
    }

    void MapView::mouseReleaseEvent(QMouseEvent* event)
    {
        QGraphicsView::mouseReleaseEvent(event);

        if (event->button() == Qt::LeftButton) {
            this->setCursor(Qt::ArrowCursor);
        } else {
            this->screen()->showSysMenu(event->globalPos());
        }
    }

    void MapView::wheelEvent(QWheelEvent* event)
    {
        auto v = zoom_ + event->angleDelta().x() / 10;
        this->zoom(boost::algorithm::clamp(v, min_zoom_, max_zoom_));
        QGraphicsView::wheelEvent(event);
    }

    void MapView::zoom(int value)
    {
        if (zoom_ != value) {
            auto factor = std::pow(scale_factor, value - zoom_);
            this->scale(factor, factor);
            zoom_ = value;
        }
    }

    void MapView::drawBackground(QPainter* painter, const QRectF& rect)
    {
        const auto rc = this->sceneRect();
        painter->drawPixmap(rc.left(), rc.top(), map_layer_);
    }

    void MapView::centerialize() { this->centerOn(center_x_, center_y_); }

    void MapView::centeralize(GraphicsCity& item)
    {
        auto p = item.pos();

        center_x_ = p.x();
        center_y_ = p.y();

        this->centerialize();
    }

    void MapView::onHoverEnter(GraphicsCity& city)
    {
        auto player = this->app()->player();
        auto fraction = city.arch()->fraction();
        auto detail = fraction && fraction->id() == player;
        cityInfo_->showArch(city.arch(), detail);
    }

    void MapView::onHoverLeave(GraphicsCity& city) { cityInfo_->hide(); }

    void MapView::onClick(GraphicsCity& city)
    {
        auto localPos = this->mapFromScene(city.pos());
        auto pos = this->mapToGlobal(localPos);

        current_ = &city;

        if (city.arch()->fraction() == this->app()->playerFraction()) {
            workMenu_->onSelf(city.arch(), pos);
        } else {
            workMenu_->onOther(city.arch(), pos);
        }
    }

    void MapView::centeralizeCapital(yrui::id_t f)
    {
        BOOST_ASSERT_MSG(f != yrui::null_id, "Bad fraction");
        auto& frac_mgr = yrui::use_subsys<yrui::fraction::Fraction_mgr>();
        auto fraction = frac_mgr.get(f);
        auto capital = fraction->capital();
        this->centeralize(*cities_.at(capital->id()));
    }

    void MapView::changeCityColor(yrui::Architecture* arch)
    {
        auto f = arch->fraction();

        if (f) {
            YCONSTRAINT(f->is_alive());

            auto capital = cities_[f->capital()->id()];
            auto target = cities_[arch->id()];

            auto c = static_cast<QGraphicsColorizeEffect*>(capital->graphicsEffect());
            BOOST_ASSERT(c);

            auto ef = new QGraphicsColorizeEffect;
            ef->setColor(c->color());
            target->setGraphicsEffect(ef);
        } else {
            cities_[arch->id()]->setGraphicsEffect(nullptr);
        }
    }

    void MapView::updateInfoBar()
    {
        BOOST_ASSERT(info_);
        info_->updateBar();
    }
}
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
