/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013 Mattia Basaglia

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "color_dialog.hpp"

Color_Dialog::Color_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    QVector<QColor> rainbow;
    for ( int i = 0; i < 360; i+= 360/6 )
        rainbow.push_back(QColor::fromHsv(i,255,255));
    rainbow.push_back(Qt::red);
    slide_hue->setBackground(rainbow);
}

QColor Color_Dialog::color() const
{
    QColor col = wheel->color();
    col.setAlpha(slide_alpha->value());
    return col;
}

void Color_Dialog::setColor(QColor c)
{
    wheel->setColor(c);
    slide_alpha->setValue(c.alpha());
    update_widgets();
}


void Color_Dialog::update_widgets()
{
    blockSignals(true);

    QColor col = color();

    //slide_red->blockSignals(true);
    slide_red->setValue(col.red());
    slide_red->setFirstColor(QColor(0,col.green(),col.blue()));
    slide_red->setLastColor(QColor(255,col.green(),col.blue()));
    //slide_red->blockSignals(false);

    //slide_green->blockSignals(true);
    slide_green->setValue(col.green());
    slide_green->setFirstColor(QColor(col.red(),0,col.blue()));
    slide_green->setLastColor(QColor(col.red(),255,col.blue()));
    //slide_green->blockSignals(false);

    //slide_blue->blockSignals(true);
    slide_blue->setValue(col.blue());
    slide_blue->setFirstColor(QColor(col.red(),col.green(),0));
    slide_blue->setLastColor(QColor(col.red(),col.green(),255));
    //slide_blue->blockSignals(false);

    slide_hue->setValue(qRound(wheel->hue()*360.0));

    slide_saturation->setValue(qRound(wheel->saturation()*255.0));
    slide_saturation->setFirstColor(QColor::fromHsvF(wheel->hue(),0,wheel->value()));
    slide_saturation->setLastColor(QColor::fromHsvF(wheel->hue(),1,wheel->value()));

    slide_value->setValue(qRound(wheel->value()*255.0));
    slide_value->setFirstColor(QColor::fromHsvF(wheel->hue(),wheel->saturation(),0));
    slide_value->setLastColor(QColor::fromHsvF(wheel->hue(),wheel->saturation(),1));


    edit_hex->setText(QString("%1%2%3%4")
                      .arg(col.red(),2,16,QChar('0'))
                      .arg(col.green(),2,16,QChar('0'))
                      .arg(col.blue(),2,16,QChar('0'))
                      .arg(col.alpha(),2,16,QChar('0'))
                     );

    preview->setColor(col);

    blockSignals(false);

    emit colorChanged(col);
}

void Color_Dialog::set_hsv()
{
    if ( !signalsBlocked() )
    {
        wheel->setColor(QColor::fromHsv(
                slide_hue->value(),
                slide_saturation->value(),
                slide_value->value()
            ));
        update_widgets();
    }
}

void Color_Dialog::set_rgb()
{
    if ( !signalsBlocked() )
    {
        QColor col ( slide_red->value(), slide_green->value(), slide_blue->value() );
        if ( col.saturation() == 0 )
            col = QColor::fromHsv(slide_hue->value(),0,col.value());
        wheel->setColor(col);
        update_widgets();
    }
}


void Color_Dialog::on_edit_hex_editingFinished()
{
    QString xs = edit_hex->text().trimmed();
    if ( xs.size() == 3 )
    {
        slide_red->setValue(QString(2,xs[0]).toInt(0,16));
        slide_green->setValue(QString(2,xs[1]).toInt(0,16));
        slide_blue->setValue(QString(2,xs[2]).toInt(0,16));
        slide_alpha->setValue(255);
    }
    else
    {
        if ( xs.size() < 8 )
        {
            xs += QString(8-xs.size(),'f');
        }
        slide_red->setValue(xs.mid(0,2).toInt(0,16));
        slide_green->setValue(xs.mid(2,2).toInt(0,16));
        slide_blue->setValue(xs.mid(4,2).toInt(0,16));
        slide_alpha->setValue(xs.mid(6,2).toInt(0,16));
    }

    set_rgb();

}

void Color_Dialog::on_edit_hex_textEdited(const QString &arg1)
{

    QString xs = arg1.trimmed();
    if ( xs.size() == 8 )
    {
        slide_red->setValue(xs.mid(0,2).toInt(0,16));
        slide_green->setValue(xs.mid(2,2).toInt(0,16));
        slide_blue->setValue(xs.mid(4,2).toInt(0,16));
        slide_alpha->setValue(xs.mid(6,2).toInt(0,16));
    }

    set_rgb();
}