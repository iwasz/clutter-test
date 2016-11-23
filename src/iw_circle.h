/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

/* inclusion guard */
#ifndef __IW_CIRCLE_H__
#define __IW_CIRCLE_H__

/* include any dependencies */
#include <clutter/clutter.h>

#ifdef __cplusplus
extern "C" {
#endif

/* GObject implementation */

/* declare this function signature to remove compilation errors with -Wall;
 * the iw_circle_get_type() function is actually added via the
 * G_DEFINE_TYPE macro in the .c file
 */
GType iw_circle_get_type (void);

/* GObject type macros */
/* returns the class type identifier (GType) for IwCircle */
#define IW_TYPE_CIRCLE (iw_circle_get_type ())

/* cast obj to a IwCircle object structure*/
#define IW_CIRCLE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), IW_TYPE_CIRCLE, IwCircle))

/* check whether obj is a IwCircle */
#define IW_IS_CIRCLE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IW_TYPE_CIRCLE))

/* cast klass to IwCircleClass class structure */
#define IW_CIRCLE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), IW_TYPE_CIRCLE, IwCircleClass))

/* check whether klass is a member of the IwCircleClass */
#define IW_IS_CIRCLE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), IW_TYPE_CIRCLE))

/* get the IwCircleClass structure for a IwCircle obj */
#define IW_CIRCLE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), IW_TYPE_CIRCLE, IwCircleClass))

/*
 * Private instance fields; see
 * http://www.gotw.ca/gotw/024.htm for the rationale
 */
typedef struct _IwCirclePrivate IwCirclePrivate;
typedef struct _IwCircle IwCircle;
typedef struct _IwCircleClass IwCircleClass;

/* object structure */
struct _IwCircle {
        /*<private>*/
        ClutterActor parent_instance;

        /* structure containing private members */
        /*<private>*/
        IwCirclePrivate *priv;
};

/* class structure */
struct _IwCircleClass {
        /*<private>*/
        ClutterActorClass parent_class;
};

/* public API */

/* constructor - note this returns a ClutterActor instance */
ClutterActor *iw_circle_new (void);
void iw_circle_set_color (IwCircle *self, const ClutterColor *color);

/* getter */
//const gchar *iw_circle_get_text (IwCircle *self);

///* setters - these are wrappers round functions
// * which change properties of the internal actors
// */
//void iw_circle_set_text (IwCircle *self, const gchar *text);

//void iw_circle_set_background_color (IwCircle *self, const ClutterColor *color);

//void iw_circle_set_text_color (IwCircle *self, const ClutterColor *color);

#ifdef __cplusplus
}
#endif

#endif /* __IW_CIRCLE_H__ */
