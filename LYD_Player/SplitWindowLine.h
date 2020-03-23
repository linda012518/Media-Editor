#ifndef _Split_Window_Line_
#define _Split_Window_Line_

#include "qframe.h"

class SplitWindowLine : public QFrame
{
	Q_OBJECT

	enum {
		Normal = 1 << 0,
		Enter = 1 << 1,
		Press = 1 << 2,
	};
public:
	unsigned	_MouseFlag = 0;

public:
	SplitWindowLine(QWidget *parent = 0);
	~SplitWindowLine();

	bool dragLine();

protected:
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);

	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // !_Split_Window_Line_



