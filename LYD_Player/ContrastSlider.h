#ifndef _Contrast_Slider_
#define _Contrast_Slider_

#include <QSlider>
#include "ContrastShader.h"

namespace lyd 
{
	class ContrastShader;
}

class ContrastSlider : public QSlider
{
	Q_OBJECT

public:
	ContrastSlider(QWidget *parent = 0);

	inline void setShader(lyd::ContrastShader* s) {
		_shader = s;
	}

	inline void init() {
		setValue(maximum() / 2);
	}

public slots:
	void updateContrast(int value);

private:
	lyd::ContrastShader* _shader;
};

#endif // !_Contrast_Slider_



