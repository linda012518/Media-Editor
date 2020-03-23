#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LYD_Player.h"

#include "Demux.h"

namespace lyd
{
	class Shader;
}

class LYD_Player : public QMainWindow
{
    Q_OBJECT

public:
    LYD_Player(QWidget *parent = Q_NULLPTR);
	~LYD_Player();

	void addShader(lyd::Shader* shader);

public slots:
	void setPause();

	void sliderPress();
	void sliderRelease();

	void volumeChange(int value);

	void setTempo(float speed);
	void setRate(float speed);
	void setPitch(float speed);

	void openVideo();
	void exportVideo();

protected:
	void timerEvent(QTimerEvent* e);

private:
    Ui::LYD_PlayerClass ui;
	lyd::Demux* _demux;
	bool		_isSliderPress	= false;
	bool		_isOpen			= false;
	bool		_isExport		= false;
};
