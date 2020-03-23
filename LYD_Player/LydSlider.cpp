#include "LydSlider.h"
#include <QGraphicsOpacityEffect> 

LydSlider::LydSlider(QWidget *parent) : QSlider(parent)
{
	//setAttribute(Qt::WA_TranslucentBackground);


	//QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
	//setGraphicsEffect(opacityEffect);
	//opacityEffect->setOpacity(0.7);

}

LydSlider::~LydSlider()
{
}
