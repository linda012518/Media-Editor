#include "ContrastSlider.h"
#include "ContrastShader.h"

ContrastSlider::ContrastSlider(QWidget *parent) : QSlider(parent)
{
	QObject::connect(this, &QSlider::valueChanged, this, &ContrastSlider::updateContrast);
}

void ContrastSlider::updateContrast(int value)
{
	double pos = (double)value / (double)maximum();
	pos *= 2;
	_shader->begin();
	_shader->setFloat("contrast", pos);
	_shader->end();
}


