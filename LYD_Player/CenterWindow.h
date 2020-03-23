#ifndef _Center_Window_
#define _Center_Window_

#include <QWidget>
#include "qframe.h"
#include "qboxlayout.h"

class SplitWindowLine;

class CenterWindow : public QWidget
{
	Q_OBJECT

public:
	CenterWindow(QWidget *parent = 0);
	~CenterWindow();

	void init(QWidget* gl, QWidget* edit, QFrame* line, SplitWindowLine* split, QWidget* layout);

protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);

private:
	QWidget*			_gl = nullptr;
	QWidget*			_edit = nullptr;
	QFrame*				_line = nullptr;
	SplitWindowLine*	_split = nullptr;
	QWidget*			_layout = nullptr;

};

#endif // !_Center_Window_


