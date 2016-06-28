#ifndef QNSTMINIMAP_H
#define QNSTMINIMAP_H

#include <QObject>

#include "StructuralView.h"

#define MINIMAP_DEFAULT_W 120*2
#define MINIMAP_DEFAULT_H 60*2

class StructuralView;

/** @brief Minimap for the current viewed structural scene
  *
  * Displays the scene of current diagram. The area of the scene, visible in the
  * EditorView, displayed on the minimap as a rectangle. Navigation on the scene is
  * possible
  * by moving that rectangle with mouse.
  *
  * This code is and adaptation of the available at:
  *           https://github.com/qreal/qreal/blob/master/qrgui/mainwindow/miniMap.h
  */
class StructuralMiniMap : public QGraphicsView
{
	Q_OBJECT
public:
	/// Drag mode is used for dragging a rectangle of the main view
	enum Mode { None, Drag };

  explicit StructuralMiniMap(QWidget *_parent = 0);

	void init(StructuralView *view);

	void setScene(QGraphicsScene *scene);

public slots:
	/// makes the minimap display only items of the scene
	void showScene();
	void ensureVisible(QList<QRectF> region);

protected:
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

	void drawForeground(QPainter *painter, QRectF const &rect);
	/// painting out the areas which aren't to be painted on the minimap (not in the scene rect)
	void drawNonExistentAreas(QPainter *painter, QRectF const &rect);
	/// @return list of areas visible on the minimap but not included in the scene rectangle
	QList<QRectF> getNonExistentAreas(QRectF const &rect);

private:
	void clear();
	/// @return a rectangle of the scene which is viewed in the editor view
	QRectF getNewRect();

	StructuralView *mEditorView;
	/// in the scene coordinates
	QRectF mEditorViewRect;

	Mode mMode;
};

#endif // QNSTMINIMAP_H
