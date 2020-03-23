#include "LYD_Player.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qmenu.h"
#include "ShaderHeader.h"

LYD_Player::LYD_Player(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.centralWidget->init(ui._gl, ui._edit, ui._line, ui._split, ui.horizontalLayoutWidget);

	QObject::connect(ui._isPause, &QPushButton::clicked, this, &LYD_Player::setPause);
	QObject::connect(ui._progress, &QSlider::sliderPressed, this, &LYD_Player::sliderPress);
	QObject::connect(ui._progress, &QSlider::sliderReleased, this, &LYD_Player::sliderRelease);

	QObject::connect(ui._volume, &QSlider::valueChanged, this, &LYD_Player::volumeChange);

	QObject::connect(ui._pitch, &QSlider::valueChanged, this, &LYD_Player::setPitch);
	QObject::connect(ui._rate, &QSlider::valueChanged, this, &LYD_Player::setRate);
	QObject::connect(ui._tempo, &QSlider::valueChanged, this, &LYD_Player::setTempo);

	QObject::connect(ui.actionOpen, &QAction::triggered, this, &LYD_Player::openVideo);
	QObject::connect(ui.actionSave, &QAction::triggered, this, &LYD_Player::exportVideo);

	_demux = new lyd::Demux();
	_demux->setGL(ui._gl);
	_demux->start();

	ui._volume->setValue(ui._volume->maximum() / 2);

	startTimer(40);
}

LYD_Player::~LYD_Player()
{
	if (_demux)
	{
		delete _demux;
		_demux = nullptr;
	}
}

void LYD_Player::addShader(lyd::Shader* shader)
{
	_demux->addShader(shader);
}

void LYD_Player::setPause()
{
	_demux->setPause(!_demux->getPause());
	if (_demux->getPause())
	{
		ui._isPause->setText("play");
	}
	else
	{
		ui._isPause->setText("pause");
	}
}

void LYD_Player::timerEvent(QTimerEvent* e)
{
	if (_isSliderPress || !_demux) return;
	if (_demux->getTotalMs() > 0)
	{
		double pos = (double)_demux->getPts() / (double)_demux->getTotalMs();
		if (pos > 0.995)
		{
			pos = 1;
			if (_isExport) _isExport = false;
		}
		ui._progress->setValue(ui._progress->maximum() * pos);
	}
}

void LYD_Player::sliderPress()
{
	_isSliderPress = true;
}
void LYD_Player::sliderRelease()
{

	_isSliderPress = false;

	double pos = (double)ui._progress->value() / (double)ui._progress->maximum();

	_demux->seek(pos);
}

void LYD_Player::volumeChange(int value)
{
	double pos = (double)value / (double)ui._volume->maximum();
	_demux->setVolume(pos*2);
}

void LYD_Player::setTempo(float speed)
{
	_demux->setTempo(speed);
}
void LYD_Player::setRate(float speed)
{
	_demux->setRate(speed);
}
void LYD_Player::setPitch(float speed)
{
	_demux->setPitch(speed);
}

void LYD_Player::openVideo()
{
	if (_isExport) return;

	QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("select file"));

	if (name.isEmpty()) return;
	this->setWindowTitle(name);

	if (!_demux->open(name.toLocal8Bit()))
	{
		QMessageBox::information(0, "error", "open file failed");
		return;
	}
	_demux->start();
	_isOpen = true;

	lyd::ContrastShader* s = new lyd::ContrastShader();
	_demux->addShader(s);
	ui._contrast->setShader(s);
	ui._contrast->init();

	resize(this->size() - QSize(1, 1));

	ui._pitch->setValue(0);
	ui._tempo->setValue(0);
	ui._rate->setValue(0);
	ui._volume->setValue(ui._volume->maximum() / 2);

	ui._isPause->setText("pause");
}
void LYD_Player::exportVideo()
{
	if (!_isOpen) return;
	QString name = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("save file"));

	if (name.isEmpty()) return;
	this->setWindowTitle(name);

	if (!_demux->exportVideo(name.toLocal8Bit()))
	{
		QMessageBox::information(0, "error", "open file failed");
		return;
	}
	_isExport = true;
}
