#ifndef _Lyd_Slider_
#define _Lyd_Slider_

#include "qslider.h"

class LydSlider : public QSlider
{
	Q_OBJECT

public:
	LydSlider(QWidget *parent = 0);
	virtual ~LydSlider();
};

#endif // !_Lyd_Slider_

