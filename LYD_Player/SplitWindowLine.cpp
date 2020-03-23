#include "SplitWindowLine.h"

SplitWindowLine::SplitWindowLine(QWidget *parent) : QFrame(parent)
{
	_MouseFlag = 0;
}

SplitWindowLine::~SplitWindowLine()
{
	_MouseFlag = 0;
}

void SplitWindowLine::enterEvent(QEvent *event)
{
	_MouseFlag |= SplitWindowLine::Enter;
	this->setCursor(QCursor(Qt::SplitVCursor));
}
void SplitWindowLine::leaveEvent(QEvent *event)
{
	if (_MouseFlag & SplitWindowLine::Press == 0)
	{
		_MouseFlag &= ~SplitWindowLine::Enter;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
}

void SplitWindowLine::mousePressEvent(QMouseEvent *event)
{
	if (_MouseFlag & SplitWindowLine::Enter)
	{
		_MouseFlag |= SplitWindowLine::Press;
	}
}
void SplitWindowLine::mouseReleaseEvent(QMouseEvent *event)
{
	_MouseFlag &= ~SplitWindowLine::Press;
}

bool SplitWindowLine::dragLine()
{
	return _MouseFlag & SplitWindowLine::Enter && _MouseFlag & SplitWindowLine::Press;
}

