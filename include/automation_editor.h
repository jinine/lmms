/*
 * automation_editor.h - declaration of class automationEditor which is a window
 *                       where you can edit dynamic values in an easy way
 *
 * Copyright (c) 2006-2007 Javier Serrano Polo <jasp00/at/users.sourceforge.net>
 * 
 * This file is part of Linux MultiMedia Studio - http://lmms.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */


#ifndef _AUTOMATION_EDITOR_H
#define _AUTOMATION_EDITOR_H

#include "qt3support.h"

#ifdef QT4

#include <QtGui/QWidget>
#include <QtCore/QVector>

#else

#include <qwidget.h>
#include <qvaluevector.h>

#endif

#include "types.h"
#include "journalling_object.h"
#include "midi_time.h"
#include "automation_pattern.h"


class QPainter;
class QPixmap;
class QScrollBar;

class comboBox;
class notePlayHandle;
class timeLine;
class toolButton;


class automationEditor : public QWidget, public journallingObject
{
	Q_OBJECT
public:
	void FASTCALL setCurrentPattern( automationPattern * _new_pattern );

	inline const automationPattern * currentPattern( void ) const
	{
		return( m_pattern );
	}

	inline bool validPattern( void ) const
	{
		return( m_pattern != NULL );
	}

	int quantization( void ) const;


	virtual void FASTCALL saveSettings( QDomDocument & _doc,
							QDomElement & _parent );
	virtual void FASTCALL loadSettings( const QDomElement & _this );
	inline virtual QString nodeName( void ) const
	{
		return( "automation-editor" );
	}


public slots:
	void update( void );


protected:
	typedef automationPattern::timeMap timeMap;

	virtual void closeEvent( QCloseEvent * _ce );
	virtual void keyPressEvent( QKeyEvent * _ke );
	virtual void leaveEvent( QEvent * _e );
	virtual void mousePressEvent( QMouseEvent * _me );
	virtual void mouseReleaseEvent( QMouseEvent * _me );
	virtual void mouseMoveEvent( QMouseEvent * _me );
	virtual void paintEvent( QPaintEvent * _pe );
	virtual void resizeEvent( QResizeEvent * _re );
	virtual void wheelEvent( QWheelEvent * _we );

	int FASTCALL getLevel( int _y );
	static inline void drawValueRect( QPainter & _p, Uint16 _x, Uint16 _y,
						Sint16 _width, Sint16 _height,
						const bool _is_selected );
	void removeSelection( void );
	void selectAll( void );
	void FASTCALL getSelectedValues( timeMap & _selected_values );


protected slots:
	void play( void );
	void stop( void );

	void horScrolled( int _new_pos );
	void verScrolled( int _new_pos );

	void drawButtonToggled( void );
	void eraseButtonToggled( void );
	void selectButtonToggled( void );
	void moveButtonToggled( void );

	void copySelectedValues( void );
	void cutSelectedValues( void );
	void pasteValues( void );
	void deleteSelectedValues( void );

	void updatePosition( const midiTime & _t );

	void zoomingXChanged( const QString & _zfac );
	void zoomingYChanged( const QString & _zfac );


private:

	enum editModes
	{
		DRAW,
		ERASE,
		SELECT,
		MOVE
	} ;

	enum actions
	{
		NONE,
		MOVE_VALUE,
		SELECT_VALUES,
		MOVE_SELECTION
	} ;

	// some constants...
	static const int INITIAL_WIDTH = 640;
	static const int INITIAL_HEIGHT = 480;

	static const int SCROLLBAR_SIZE = 16;
	static const int TOP_MARGIN = 48;

	static const int DEFAULT_Y_DELTA = 6;
	static const int DEFAULT_STEPS_PER_TACT = 16;
	static const int DEFAULT_PPT = 12 * DEFAULT_STEPS_PER_TACT;

	static const int VALUES_WIDTH = 64;

	automationEditor( void );
	automationEditor( const automationEditor & );
	virtual ~automationEditor();


	static QPixmap * s_toolDraw;
	static QPixmap * s_toolErase;
	static QPixmap * s_toolSelect;
	static QPixmap * s_toolMove;


	QWidget * m_toolBar;

	toolButton * m_playButton;
	toolButton * m_stopButton;

	toolButton * m_drawButton;
	toolButton * m_eraseButton;
	toolButton * m_selectButton;
	toolButton * m_moveButton;

	toolButton * m_cutButton;
	toolButton * m_copyButton;
	toolButton * m_pasteButton;

	comboBox * m_zoomingXComboBox;
	comboBox * m_zoomingYComboBox;
	comboBox * m_quantizeComboBox;


	automationPattern * m_pattern;
	int m_min_level;
	int m_max_level;
	int m_scroll_level;
	int m_bottom_level;
	int m_top_level;

	void updatePaintPixmap( QPixmap & _p );
	void updateTopBottomLevels( void );

	QScrollBar * m_leftRightScroll;
	QScrollBar * m_topBottomScroll;

	midiTime m_currentPosition;

	actions m_action;

	Uint32 m_selectStartTact64th;
	int m_selectedTact64th;
	int m_selectStartLevel;
	int m_selectedLevels;

	int m_moveStartLevel;
	int m_moveStartTact64th;
	int m_moveXOffset;

	int m_ppt;
	int m_y_delta;
	bool m_y_auto;

	timeMap m_valuesToCopy;
	timeMap m_selValuesForMove;


	editModes m_editMode;


	timeLine * m_timeLine;
	bool m_scrollBack;

	void drawCross( QPainter & _p );
	bool inBBEditor( void );



	friend class engine;


signals:
	void positionChanged( const midiTime & );

} ;


#endif

