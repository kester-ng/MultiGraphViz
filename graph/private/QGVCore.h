#ifndef QGVCORE_H
#define QGVCORE_H

#include <QPointF>
#include <QPolygonF>
#include <QPainterPath>
#include <QColor>

//GraphViz headers
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>

const qreal DotDefaultDPI = 72.0;

/**
 * @brief GraphViz to GraphicsScene conversions
 *
 */
class QGVCore
{
public:
    static qreal graphHeight(Agraph_t *graph);
    static QPointF toPoint(pointf p, qreal gheight);
    static QPointF toPoint(point p, qreal gheight);
    static QPointF centerToOrigin(const QPointF &p, qreal width, qreal height);
    static QPolygonF toPolygon(const polygon_t* poly, qreal width, qreal height);

    static QPainterPath toPath(const char *type, const polygon_t *poly, qreal width, qreal height);
    static QPainterPath toPath(const splines* spl, qreal gheight);

    static Qt::BrushStyle toBrushStyle(const QString &style);
    static Qt::PenStyle toPenStyle(const QString &style);
    static QColor toColor(const QString &color);

        typedef struct {
                const char *data;
                int len;
                int cur;
        } rdr_t;

        static int memiofread(void *chan, char *buf, int bufsize)
        {
                const char *ptr;
                char *optr;
                char c;
                int l;
                rdr_t *s;

                if (bufsize == 0) return 0;
                s = (rdr_t *) chan;
                if (s->cur >= s->len)
                        return 0;
                l = 0;
                ptr = s->data + s->cur;
                optr = buf;
                do {
                        *optr++ = c = *ptr++;
                        l++;
                } while (c && (c != '\n') && (l < bufsize));
                s->cur += l;
                return l;
        }

        static Agraph_t *agmemread2(const char *cp)
        {
                Agraph_t* g;
                rdr_t rdr;
                Agdisc_t disc;
                Agiodisc_t memIoDisc;

                memIoDisc.afread = memiofread;
                memIoDisc.putstr = AgIoDisc.putstr;
                memIoDisc.flush = AgIoDisc.flush;
                rdr.data = cp;
                rdr.len = strlen(cp);
                rdr.cur = 0;

                disc.mem = &AgMemDisc;
                disc.id = &AgIdDisc;
                disc.io = &memIoDisc;
                g = agread (&rdr, &disc);
                return g;
        }
};

#endif // QGVCORE_H
