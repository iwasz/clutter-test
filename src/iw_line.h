/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

/* inclusion guard */
#ifndef __IW_LINE_H__
#define __IW_LINE_H__

/* include any dependencies */
#include <clutter/clutter.h>

#ifdef __cplusplus
extern "C" {
#endif

/* GObject implementation */

/* declare this function signature to remove compilation errors with -Wall;
 * the iw_line_get_type() function is actually added via the
 * G_DEFINE_TYPE macro in the .c file
 */
GType iw_line_get_type (void);

/* GObject type macros */
/* returns the class type identifier (GType) for IwLine */
#define IW_TYPE_LINE (iw_line_get_type ())

/* cast obj to a IwLine object structure*/
#define IW_LINE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), IW_TYPE_LINE, IwLine))

/* check whether obj is a IwLine */
#define IW_IS_LINE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IW_TYPE_LINE))

/* cast klass to IwLineClass class structure */
#define IW_LINE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), IW_TYPE_LINE, IwLineClass))

/* check whether klass is a member of the IwLineClass */
#define IW_IS_LINE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), IW_TYPE_LINE))

/* get the IwLineClass structure for a IwLine obj */
#define IW_LINE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), IW_TYPE_LINE, IwLineClass))

/*
 * Private instance fields; see
 * http://www.gotw.ca/gotw/024.htm for the rationale
 */
typedef struct _IwLinePrivate IwLinePrivate;
typedef struct _IwLine IwLine;
typedef struct _IwLineClass IwLineClass;

/* object structure */
struct _IwLine {
        /*<private>*/
        ClutterActor parent_instance;

        /* structure containing private members */
        /*<private>*/
        IwLinePrivate *priv;
};

/* class structure */
struct _IwLineClass {
        /*<private>*/
        ClutterActorClass parent_class;
};

/* public API */

/* constructor - note this returns a ClutterActor instance */
ClutterActor *iw_line_new (void);
void iw_line_set_color (IwLine *self, const ClutterColor *color);


#ifdef __cplusplus
}
#endif

#endif /* __IW_LINE_H__ */
