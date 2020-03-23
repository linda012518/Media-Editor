#include "CenterWindow.h"
#include "SplitWindowLine.h"
#include "qevent.h"

CenterWindow::CenterWindow(QWidget *parent) : QWidget(parent)
{
}

CenterWindow::~CenterWindow()
{
}

void CenterWindow::init(QWidget* gl, QWidget* edit, QFrame* line, SplitWindowLine* split, QWidget* layout)
{
	_gl		= gl;
	_edit	= edit;
	_line	= line;
	_split	= split;
	_layout = layout;

	_gl->setGeometry(		0,	0,				200,	200);
	_split->setGeometry(	0,	200,			200,	2);
	_edit->setGeometry(		0,	200 + 2,		200,	200);
	_line->setGeometry(		0,	200 * 2 + 2,	200,	2);
	_layout->setGeometry(	0,	200 * 2 + 2*2,	200,	40);
}

void CenterWindow::resizeEvent(QResizeEvent *event)
{
	int w				=	width();
	int h				=	height();
	int lineHeight		=	_split->height();
	int layoutHeihgt	=	_layout->geometry().height();

	int workSpace		=	h - layoutHeihgt - lineHeight;

	float	rate		=	_split->pos().y() / (float)(_gl->height() + _edit->height() + lineHeight);
	int		glHeight	=	rate * workSpace;
	int		editHeight	=	workSpace - glHeight - lineHeight;

	_gl->setGeometry(		0,	0,										w,	glHeight);
	_split->setGeometry(	0,	glHeight,								w,	lineHeight);
	_edit->setGeometry(		0,	glHeight + lineHeight,					w,	editHeight);
	_line->setGeometry(		0,	glHeight + lineHeight + editHeight,		w,	lineHeight);
	_layout->setGeometry(	0,	glHeight + lineHeight * 2 + editHeight,	w,	layoutHeihgt);
}
void CenterWindow::mouseMoveEvent(QMouseEvent *event)
{
	if (_split->dragLine())
	{
		int w				=	width();
		int h				=	height();
		int lineHeight		=	_split->height();
		int layoutHeihgt	=	_layout->geometry().height();

		int halfHeight		=	(h - lineHeight * 2 - layoutHeihgt) / 2;

		float	rate		=	event->pos().y() / (float)(_gl->height() + _edit->height() + lineHeight);
		int		glHeight	=	rate * (halfHeight * 2 + lineHeight);
		int		editHeight	=	halfHeight * 2 + lineHeight - glHeight;

		_gl->setGeometry(		0,	0,						w,	glHeight);
		_split->setGeometry(	0,	glHeight,				w,	lineHeight);
		_edit->setGeometry(		0,	glHeight + lineHeight,	w,	editHeight);
	}
}
